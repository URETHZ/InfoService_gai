#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <string>

class DataBaseController {
public:
    // Статический метод получения единственного экземпляра
    static DataBaseController& instance();

    // Метод для установки параметров
    void setConnectionParams(const std::string &ip, int port, const std::string &dbName, const std::string &user, const std::string &pass);

    // Получение базы данных
    //SqlDatabase getDatabase();

    // Запрещаем копирование
    DataBaseController(const DataBaseController&) = delete;
    DataBaseController& operator=(const DataBaseController&) = delete;

private:
    DataBaseController() = default;


    std::string m_ipAddress = "localhost";
    int m_port = 5432;
};

#endif // DATABASECONTROLLER_H
