

#include <QRandomGenerator>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCoreApplication>
#include <QThread>
#include "appSettings.h"
#include "authorization.h"
#include "hexByArgon2.h"
#include <QJSEngine>

// Метод авторизации после запроса в БД
Authorization::AuthStatus Authorization::authorize(const QString &password, const QString &DBHashpassword) {
    if(!DBHashpassword.isEmpty()) {
        QStringList parts = DBHashpassword.split(':');
        if (parts.size() != 2) {
            return Authorization::AuthStatus::UNKOWN_ERROR;
        }

        QString dbSalt = parts[0]; // Соль из базы
        QString dbHash = parts[1]; // Эталонный хеш из базы
        QString newHash = gethashPasswordbySalt(password, dbSalt);

        if (newHash == dbHash) {
            return Authorization::AuthStatus::OK;
        }
        return Authorization::AuthStatus::PASSWORD_INCORRECT;
    }
    else {
        return Authorization::AuthStatus::LOGIN_NOT_FOUND;
    }
}

bool Authorization::haveRightsToModule(int moduleNumber, int type)
{
    int rights = userRights.value(moduleNumber);
    return (rights & type) != 0;
}

// Конструктор для инициализации потоков и сигналов
Authorization::Authorization(QObject *parent) : QObject(parent) {
    m_dbThread = new QThread(this);
    // Создаем воркер (пока он в главном потоке)
    m_dbWorker = new DatabaseWorker();

    // воркер в фоновый поток
    m_dbWorker->moveToThread(m_dbThread);
    // подписка на уведомления от фонового потока
    connect(m_dbWorker, &DatabaseWorker::queryFinished, this, &Authorization::queryFinished);
    connect(m_dbWorker, &DatabaseWorker::queryError, this, &Authorization::queryError);
    connect(m_dbWorker, &DatabaseWorker::connectionSuccess, this, &Authorization::connectionSuccess);
    connect(m_dbWorker, &DatabaseWorker::connectionFailed, this, &Authorization::connectionFailed);
    // Безопасное удаление воркера при завершении потока
    connect(m_dbThread, &QThread::finished, m_dbWorker, &QObject::deleteLater);
    // Сносим все соединения если уже вышли из программы
    connect(qApp, &QCoreApplication::aboutToQuit, this, &Authorization::shutdown);
}

// Singleton
Authorization& Authorization::instance() {
    static Authorization auth;
    return auth;
}

// Singleton для QML
Authorization *Authorization::create(QQmlEngine *, QJSEngine *)
{
    QJSEngine::setObjectOwnership(&instance(), QJSEngine::CppOwnership);
    return &instance();
}

Authorization::~Authorization() {
    if (m_dbThread->isRunning()) closeAsyncConnection();
    m_dbThread->quit();
    m_dbThread->wait();
}

// Устнановка соединеня с БД для окна авторизации

void Authorization::initConnectionAsync(const QString &ip, int port) {
    m_dbThread->start();
    QMetaObject::invokeMethod(m_dbWorker, "initConnection", Qt::QueuedConnection,
                              Q_ARG(QString, "gai_app_role"),
                              Q_ARG(QString, "h4fr1sdpcg4s33"),
                              Q_ARG(QString, ip),
                              Q_ARG(int, port)
                              );
}

// Вызов SQL выражения в потоке
void Authorization::executeSelectQueryAsync(int requestId, const QString &sql, const QVariantMap &params) {
    // Отправляем запрос в фоновый поток
    QMetaObject::invokeMethod(m_dbWorker, "executeSelect", Qt::QueuedConnection,
                              Q_ARG(int, requestId),
                              Q_ARG(QString, "gai_app_role"),
                              Q_ARG(QString, sql),
                              Q_ARG(QVariantMap, params)
                              );
}

void Authorization::takePassword(int requestId, const QVariantMap &params) {
    Authorization::executeSelectQueryAsync(requestId,
                                        "SELECT password "
                                        "FROM \"systemval\".users "
                                        "WHERE username = :login", params);
}

void Authorization::takeMenuList(int requestId) {
    Authorization::executeSelectQueryAsync(requestId,
                                                                     "SELECT * FROM systemval.menu ORDER BY parent_id, sort_order", {});
}

void Authorization::takeRights(int requestId, const QVariantMap &params) {
    Authorization::executeSelectQueryAsync(requestId,
                                                                     "SELECT r.menu_id, "
                                                                     "COALESCE(r.r, false) AS r,"
                                                                     "COALESCE(r.w, false) AS w, "
                                                                     "COALESCE(r.e, false) AS e, "
                                                                     "COALESCE(r.d, false) AS d "
                                                                     "FROM systemval.rights AS r "
                                                                     "RIGHT JOIN systemval.users AS u ON r.user_id = u.user_id "
                                                                     "WHERE u.username = :login ", params);
    m_currentLogin = params.value(":login").toString();
    emit currentLoginChanged();
}

static QVariantMap buildNode(int id,
                             const QHash<int, QVariantMap> &nodes,
                             const QHash<int, QList<int>> &childIds)
{
    QVariantMap node = nodes.value(id);
    QVariantList children;
    for (int childId : childIds.value(id))
        children.append(buildNode(childId, nodes, childIds));
    node["childrens"] = children;
    return node;
}

void Authorization::setMenuList(const QVariantList &data)
{
    QHash<int, QVariantMap> nodes;
    QHash<int, QList<int>> childIds;   // parent_id -> id детей (порядок как в запросе, т.е. по sort_order)

    for (const QVariant &item : data) {
        QVariantMap row = item.toMap();
        int id = row.value("menu_id").toInt();
        nodes.insert(id, row);
        childIds[row.value("parent_id").toInt()].append(id);
    }

    QVariantList tree;
    for (int rootId : childIds.value(0))
        tree.append(buildNode(rootId, nodes, childIds));

    m_menuTree = tree;
    emit menuTreeChanged();
}

void Authorization::setRights(const QVariantList &data) {
    for(const QVariant &item : data ) {
        QVariantMap query = item.toMap();
        int mask = 0;
        if (query.value("r").toBool()) mask |= 1; // Read
        if (query.value("w").toBool()) mask |= 2; // Write
        if (query.value("e").toBool()) mask |= 4; // Edit
        if (query.value("d").toBool()) mask |= 8; // Delete
        userRights.insert(query.value("menu_id").toInt(), mask);
    }
}

void Authorization::closeAsyncConnection() {
    QMetaObject::invokeMethod(m_dbWorker, "closeConnection" , Qt::QueuedConnection,
                               Q_ARG(QString, "gai_app_role"));
}

void Authorization::shutdown() {
    if (!m_dbThread->isRunning())
        return;

    m_dbThread->quit();
    if (!m_dbThread->wait(6000))
        qWarning() << "DB thread did not stop in time";
}