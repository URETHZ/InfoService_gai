#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <qsqlerror.h>
#include <QSqlRecord>
#include "databaseController.h"
#include "databaseWorker.h"

// 1. Инициализация подключения (вызывается в фоновом потоке)
void DatabaseWorker::initConnection(const QString &role, const QString &password, const QString &ip, int port) {
    bool tryConn = DataBaseController::instance().setNamedConnection(role, password, ip, port);
    if (tryConn) {
        emit DatabaseWorker::connectionSuccess();
    } else {
        emit DatabaseWorker::connectionFailed("Не удалось подключиться к серверу БД.");
    }
}

// 2. Выполнение запроса (вызывается в фоновом потоке)
void DatabaseWorker::executeSelect(int requestId, const QString &role, const QString &sql, const QVariantMap &params) {
    // Получаем соединение ДЛЯ ЭТОГО ПОТОКА (контроллер сделает это автоматически)
    QSqlDatabase db = DataBaseController::instance().getConnectionByName(role);
    if (!db.isOpen()) {
        emit queryError(requestId, "Соединение с БД потеряно.");
        return;
    }

    QSqlQuery query(db);
    query.prepare(sql);

    // Привязываем параметры
    for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
        query.bindValue(it.key(), it.value());
    }

    if (!query.exec()) {
        emit DatabaseWorker::queryError(requestId, query.lastError().text());
        return;
    }

    // Собираем результат в список словарей для передачи в GUI
    QVariantList result;
    while (query.next()) {
        QVariantMap row;
        for (int i = 0; i < query.record().count(); ++i) {
            row[query.record().fieldName(i)] = query.value(i);
        }
        result.append(row);
    }

    emit DatabaseWorker::queryFinished(requestId, result);
}