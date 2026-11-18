#include "databaseController.h"
#include <QSqlDatabase>
#include <QThread>


DataBaseController& DataBaseController::instance() {
    static DataBaseController obj;
    return obj;
}

QString DataBaseController::getConnNameByUsername(const QString &user) {
    // Получаем указатель на текущий поток
    QThread *currentThread = QThread::currentThread();

    return QString("%1-%2-%3")
        .arg(m_dbName)
        .arg(user)
        .arg(reinterpret_cast<quintptr>(currentThread));
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
    connections.value(name).setDatabaseName(m_dbName);
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

// Универсальный метод для установки соединений
bool DataBaseController::setNamedConnection(const QString &user, const QString &userPswd, const QString &ip, int port) {
    // Если таких соедиений еще нет:
    closeConnectionByName(user);
    DataBaseController::instance().addDatabaseName(user);
    // Устанавлививаем параметры
    DataBaseController::instance().setConnectionParams(user, userPswd, ip, port);
    if (DataBaseController::instance().ConnectToDB(user)) return true;
    return false;
}

// Поиск соединения по ключу, если такого нету, возвращает  пустой объект
QSqlDatabase DataBaseController::getConnectionByName(const QString &user) {
    return connections.value(getConnNameByUsername(user), QSqlDatabase());
}

void DataBaseController::closeConnectionByName(const QString &user)
{
    if(DataBaseController::getConnectionByName(user).isValid())
    {
        connections.value(getConnNameByUsername(user)).close();
        connections.remove(getConnNameByUsername(user));
    }
}

// Закрываем соединения
DataBaseController::~DataBaseController() {
    QStringList names = connections.keys();
    connections.clear();
    for (const QString &name : names) {
        QSqlDatabase::removeDatabase(name);
    }
}