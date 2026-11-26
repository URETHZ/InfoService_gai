#include "moduleAPI.h"
#include "IDictionaryBackend.h"

class Inad : IDictionaryBackend {
public:
    QAbstractItemModel *model() const override {
        return 0;
    }
    bool canAdd() const override {
        return true;
    }
    bool canEdit() const override {
        return true;
    }
    bool canDelete() const override {
        return true;
    }

    Q_INVOKABLE bool addRow(const QVariantMap &values) override {
        return true;
    }
    Q_INVOKABLE bool editRow(int row, const QVariantMap &values) override {
        return true;
    }
    Q_INVOKABLE bool removeRow(int row) override {
        return true;
    }
};

MODULE_EXPORT int moduleApiVersion()
{
    return MODULE_API_VERSION;    // значение зашивается при компиляции модуля
}

MODULE_EXPORT void* City(){
    return static_cast<void*>(new Inad());
}


