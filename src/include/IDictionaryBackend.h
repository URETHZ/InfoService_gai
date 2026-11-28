#ifndef IDICTIONARYBACKEND_H
#define IDICTIONARYBACKEND_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QtQml/qqmlregistration.h>
#include <QAbstractItemModel>
#include "moduleAPI.h"

// контракт для .dll модулей, реализущих работу со справочниками
class IDictionaryBackend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *model READ model CONSTANT)
    Q_PROPERTY(bool canAdd    READ canAdd    CONSTANT)
    Q_PROPERTY(bool canEdit   READ canEdit   CONSTANT)
    Q_PROPERTY(bool canDelete READ canDelete CONSTANT)
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorOccurred)

public:
    Context& getContext() { return context; }
    IDictionaryBackend(Context ctx) { context=ctx; }
    virtual QAbstractItemModel *model() const = 0;
    bool canAdd() const {
        return (context.userRights & 2 ) != 0;
    }
    bool canEdit() const {
        return (context.userRights & 4 ) != 0;
    }
    bool canDelete() const {
        return (context.userRights & 8 ) != 0;
    }
    virtual void loadData() = 0;

    Q_INVOKABLE virtual bool addRow(const QVariantMap &values) = 0;
    Q_INVOKABLE virtual bool editRow(int row, const QVariantMap &values) = 0;
    Q_INVOKABLE virtual bool removeRow(int row) = 0;

    QString lastError() const { return m_lastError; }

    void setError(const QString &error) {
        m_lastError = error;
        emit errorOccurred();
    }

signals:
    void errorOccurred();
private:
    QString m_lastError = "";
    Context context;
};

#endif // IDICTIONARYBACKEND_H
