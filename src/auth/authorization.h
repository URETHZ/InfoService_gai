#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QtQml/qqmlregistration.h>
#include "DatabaseWorker.h"

class QQmlEngine;
class QJSEngine;

class Authorization : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QVariantList menuTree READ menuTree NOTIFY menuTreeChanged)
    Q_PROPERTY(QString currentLogin READ currentLogin)
private:
    void shutdown();
public:
    enum AuthStatus {
        OK = 0,
        LOGIN_NOT_FOUND = 1,
        PASSWORD_INCORRECT = 2,
        BLOCKED = 3,
        SERVER_ERROR= 4,
        UNKOWN_ERROR = 5,
    };
    Q_ENUM(AuthStatus)

    static Authorization& instance();
    static Authorization* create(QQmlEngine *, QJSEngine *);
    void closeAsyncConnection();

    ~Authorization();

    Q_INVOKABLE AuthStatus authorize(const QString &login, const QString &password);

    Q_INVOKABLE bool haveRightsToModule(int moduleNumber, int type); // 1 - r , 2 - w, 4 - e, 8 - d

    Q_INVOKABLE void initConnectionAsync(const QString &ip, int port);

    void executeSelectQueryAsync(int requestId, const QString &sql, const QVariantMap &params);

    Q_INVOKABLE void takePassword(int requestId, const QVariantMap &params);

    Q_INVOKABLE void takeMenuList(int requestId);

    Q_INVOKABLE void takeRights(int requestId, const QVariantMap &params);

    Q_INVOKABLE void setMenuList(const QVariantList &data);

    Q_INVOKABLE void setRights(const QVariantList &data);

    QVariantList menuTree() const { return m_menuTree; }

    QString currentLogin() const { return m_currentLogin; }

signals:
    void connectionSuccess();
    void connectionFailed(const QString &error);
    void queryFinished(int requestId, const QVariantList &data);
    void queryError(int requestId, const QString &error);
    void menuTreeChanged();
private:
    explicit Authorization(QObject *parent = nullptr);

    QString m_currentLogin = "";
    QVariantList m_menuTree;
    QHash<int, int> userRights; // id_menu + битовая маска прав
    QThread *m_dbThread;
    DatabaseWorker *m_dbWorker;
};

#endif