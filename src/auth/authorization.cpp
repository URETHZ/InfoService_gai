#include <QRandomGenerator>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCoreApplication>
#include <QThread>
#include "appSettings.h"
#include "authorization.h"
#include "hexByArgon2.h"

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

bool Authorization::haveRightsToModule(int moduleNumber)
{
    return true;
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
Authorization& Authorization::create(QQmlEngine *, QJSEngine *) {
    return Authorization::instance();   // QML получает тот же объект, что и C++
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
void Authorization::executeQueryAsync(int requestId, const QString &sql, const QVariantMap &params) {
    // Отправляем запрос в фоновый поток
    QMetaObject::invokeMethod(m_dbWorker, "executeSelect", Qt::QueuedConnection,
                              Q_ARG(int, requestId),
                              Q_ARG(QString, "gai_app_role"),
                              Q_ARG(QString, sql),
                              Q_ARG(QVariantMap, params)
                              );
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