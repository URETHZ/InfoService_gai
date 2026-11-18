#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <qsqlerror.h>
#include <QSqlRecord>

class DatabaseWorker : public QObject {
    Q_OBJECT
public:
    explicit DatabaseWorker(QObject *parent = nullptr) : QObject(parent) {}
    bool isConnectionAlive(const QString &role);
    ~DatabaseWorker();

public slots:
    // 1. Инициализация подключения (вызывается в фоновом потоке)
    void initConnection(const QString &role, const QString &password, const QString &ip, int port);

    // 2. Выполнение запроса (вызывается в фоновом потоке)
    void executeSelect(int requestId, const QString &role, const QString &sql, const QVariantMap &params);

    void closeConnection(const QString &role);
signals:
    void connectionSuccess();
    void connectionFailed(const QString &error);
    void queryFinished(int requestId, const QVariantList &data);
    void queryError(int requestId, const QString &error);
};
#endif // DATABASEWORKER_H
