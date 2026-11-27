//#include "moduleAPI.h"
#include "IDictionaryBackend.h"
#include "tableModel_Dictionary.h"

MODULE_EXPORT int moduleApiVersion()
{
    return MODULE_API_VERSION;    // значение зашивается при компиляции модуля
}

MODULE_EXPORT void* City(Context ctx){
    return static_cast<void*>(new CityModule(ctx));
}

class CityModule : IDictionaryBackend {
public:
    CityModule(Context ctx) : base(ctx) {
        m_data = new TableModel_Dictionary();
        loadData();
    }

    QAbstractItemModel *model() const override {
        return m_model;
    }
    Q_INVOKABLE bool addRow(const QVariantMap &values) const override {
        const QString insertStr = "INSERT INTO mainschenma.towns (\"Название\") VALUES (:value);";
        QVariantMap params;
        QString val = "\'" + values.value("Название") + "\'";
        params.insert(":value", val);

        int id = getContext().executeQuery(this, insertStr, &params, &CityModule::callback, this);

        if ( id != -1 ) return true;
        return false;
    }
    Q_INVOKABLE bool editRow(int row, const QVariantMap &values) const override {
        const QString updateStr = "UPDATE mainschenma.towns SET (\"Название\" = :value) WHERE id = :term";
        QVariantMap params;
        QString val = "\'" + values.value("Название") + "\'";
        params.insert(":value", val);
        params.insert(":term", row);

        int id = getContext().executeQuery(this, updateStr, &params, &CityModule::callback, this);
        if ( id != -1 ) return true;
        return false;
    }
    Q_INVOKABLE bool removeRow(int row) const override {
        const QString deleteStr = "DELETE FROM mainschenma.towns WHERE id = :term";
        QVariantMap params;
        params.insert(":term", row);
        int id = getContext().executeQuery(this, deleteStr, &params, &CityModule::callback, this);
        if ( id != -1 ) return true;
        return false;
    }
    static void callback (void* user, int requestId,
                  const QVariantList &rows, const char *error) {
        CityModule* self = static_cast<CityModule*>(user);
        if (error=="") {
            self->setModelData(rows);
            loadData();
        }
        else {

        }
        return;
    }
    void loadData() override {
        QString sql = "SELECT id, \"Название\" FROM mainschenma.\"Город\" ORDER BY id";
        QVariantMap params;
        getContext().executeQuery(this, sql.toUtf8().constData(), &params, staticCallback);
    }
    void setModelData(const QVariantList &rows) {
        m_data.setData(rows);
    }
private:
    TableModel_Dictionary m_data;
};




