#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QtQml/qqmlregistration.h>

class Authorization : public QObject {
    Q_OBJECT
    QML_ELEMENT

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

    explicit Authorization(QObject *parent = nullptr) { }

    Q_INVOKABLE AuthStatus authorize(const QString &login, const QString &password, const QString &ip = "localhost", int port = 5432);

    Q_INVOKABLE bool haveRightsToModule(int moduleNumber);
    Q_INVOKABLE bool isLogined() const { return !currentLogin.isEmpty(); }

private:
    int currentUserId = -1;
    QString currentLogin = "";
    QHash<int, ModuleRight> UserRights;
};

#endif