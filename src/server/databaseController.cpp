#include "databaseController.h"

DataBaseController& DataBaseController::instance() {
    static DataBaseController obj;
    return obj;
}

void DataBaseController::setConnectionParams(const std::string &ip, int port, const std::string &dbName,
                                             const std::string &user, const std::string &pass) {

}