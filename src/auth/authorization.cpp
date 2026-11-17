#include <QRandomGenerator>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "authorization.h"
#include "databaseController.h"
#include "hexByArgon2.h"

// Метод авторизации
Authorization::AuthStatus Authorization::authorize(const QString &login, const QString &password)
{
    QSqlQuery query(DataBaseController::instance().getConnectionByName("gai_app_role"));
    //Заполняем запрос
    query.prepare("SELECT password FROM systemval.users WHERE \"username\" = :login");
    query.bindValue(":login",login);

    //Обрабатываем результат
    if (query.exec()){
        if (query.next())
        {
            QString dbStoredString = query.value(0).toString();

            QStringList parts = dbStoredString.split(':');
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
    else{
        return Authorization::AuthStatus::SERVER_ERROR;
    }

}

bool Authorization::haveRightsToModule(int moduleNumber)
{
    return true;
}