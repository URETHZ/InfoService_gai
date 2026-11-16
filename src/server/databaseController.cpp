#include "databaseController.h"
#include <QSqlDatabase>


DataBaseController& DataBaseController::instance() {
    static DataBaseController obj;
    return obj;
}

QString DataBaseController::getConnNameByUsername(const QString &user) {
    QStringList list;
    list << dbName << "-" << user;
    return list.join("");
}

bool DataBaseController::addDatabaseName(const QString &user){
    QString name = getConnNameByUsername(user);
    connections.insert(name, QSqlDatabase::addDatabase("QPSQL", name));
    if(!connections.contains(name)){
        return false;
    }
    return true;
}

bool DataBaseController::setConnectionParams(const QString &user, const QString &pass, const QString &ip, int port) {
    QString name = getConnNameByUsername(user);
    if(!connections.contains(name)){
        return false;
    }
    connections.value(name).setHostName(ip);
    connections.value(name).setPort(port);
    connections.value(name).setDatabaseName(dbName);
    connections.value(name).setUserName(user);
    connections.value(name).setPassword(pass);
    return true;
}

bool DataBaseController::ConnectToDB(const QString &user)
{
    QString name = getConnNameByUsername(user);
    if(!connections.contains(name)){
        return false;
    }
    return connections.value(name).open();
}

QSqlDatabase DataBaseController::getConnectionByName(const QString &user) {
    return connections.value(getConnNameByUsername(user));
}

DataBaseController::~DataBaseController() {
    QStringList names = connections.keys();
    connections.clear();
    for (const QString &name : names) {
        QSqlDatabase::removeDatabase(name);
    }
}