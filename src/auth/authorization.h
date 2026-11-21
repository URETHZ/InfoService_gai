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

    enum ModuleRight {
        R = 0,
        W = 1,
        E = 2,
        D = 3
    };
    Q_ENUM(ModuleRight)
    static Authorization& instance();
    explicit Authorization(QObject *parent = nullptr);
    static Authorization& create(QQmlEngine *, QJSEngine *);
    void closeAsyncConnection();

    ~Authorization();

    Q_INVOKABLE AuthStatus authorize(const QString &login, const QString &password);

    Q_INVOKABLE bool haveRightsToModule(int moduleNumber);

    Q_INVOKABLE bool isLogined() const { return !currentLogin.isEmpty(); }

    Q_INVOKABLE void initConnectionAsync(const QString &ip, int port);

    void executeSelectQueryAsync(int requestId, const QString &sql, const QVariantMap &params);

    Q_INVOKABLE void takePassword(int requestId, const QVariantMap &params);

    Q_INVOKABLE void takeMenuList(int requestId, const QVariantMap &params);

    Q_INVOKABLE void takeRights(int requestId, const QVariantMap &params);

    Q_INVOKABLE void setMenuList(const QVariantList &data);

    Q_INVOKABLE void setRights(const QVariantList &data);
signals:
    void connectionSuccess();
    void connectionFailed(const QString &error);
    void queryFinished(int requestId, const QVariantList &data);
    void queryError(int requestId, const QString &error);
private:
    int currentUserId = -1;
    QString currentLogin = "";
    QHash<int, ModuleRight> UserRights;
    QThread *m_dbThread;
    DatabaseWorker *m_dbWorker;
};

#endif