#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H
#define DATABASE_NAME "gai_db"
#include <QSqlDatabase>
#include <QSqlQuery>


class DataBaseController {
public:
    // Статический метод получения единственного экземпляра
    static DataBaseController& instance();
    // Добавляет в список connections соединение
    bool addDatabaseName(const QString &user);

    // Метод для установки параметров
    bool setConnectionParams(const QString &user, const QString &pass,
                             const QString &ip = "localhost", int port= 5432);

    // Подключение к бд
    bool ConnectToDB(const QString &user);

    // Получение базы данных
    QSqlDatabase getConnectionByName(const QString &user);

    void closeConnectionByName(const QString &user) { connections.value(getConnNameByUsername(user)).close(); }
    bool isOpenConnection(const QString &user) { return connections.value(getConnNameByUsername(user)).isOpen(); }
    QString getConnNameByUsername(const QString &user);

    // Запрещаем копирование
    DataBaseController(const DataBaseController&) = delete;
    DataBaseController& operator=(const DataBaseController&) = delete;
    // Переопределенный деструктор для удаления пула соединений
    ~DataBaseController();

private:
    DataBaseController() = default;
    QHash<QString, QSqlDatabase> connections;
    QString dbName = DATABASE_NAME;
};

#endif // DATABASECONTROLLER_H
