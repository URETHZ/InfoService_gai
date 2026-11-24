#ifndef IDICTIONARYBACKEND_H
#define IDICTIONARYBACKEND_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QtQml/qqmlregistration.h>
#include <QAbstractItemModel>

// контракт для .dll модулей, реализущих работу со справочниками
class IDictionaryBackend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *model READ model CONSTANT)
    Q_PROPERTY(bool canAdd    READ canAdd    CONSTANT)
    Q_PROPERTY(bool canEdit   READ canEdit   CONSTANT)
    Q_PROPERTY(bool canDelete READ canDelete CONSTANT)
public:
    virtual QAbstractItemModel *model() const = 0;
    virtual bool canAdd() const = 0;
    virtual bool canEdit() const = 0;
    virtual bool canDelete() const = 0;

    Q_INVOKABLE virtual bool addRow(const QVariantMap &values) = 0;
    Q_INVOKABLE virtual bool editRow(int row, const QVariantMap &values) = 0;
    Q_INVOKABLE virtual bool removeRow(int row) = 0;
};

#endif // IDICTIONARYBACKEND_H
