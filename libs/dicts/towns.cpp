//#include "moduleAPI.h"
#include "IDictionaryBackend.h"
#include "tableModel_Dictionary.h"
#include <QString>

MODULE_EXPORT int moduleApiVersion()
{
    return MODULE_API_VERSION;    // значение зашивается при компиляции модуля
}

class CityModule : public IDictionaryBackend {
public:
    CityModule(Context& ctx) : IDictionaryBackend(ctx) ,
        m_model(std::make_unique<TableModel_Dictionary>())
    {
        loadData();
    }

    QAbstractItemModel* model() const override {
        return m_model.get();
    }
    Q_INVOKABLE bool addRow(const QVariantMap &values) override {
        const QString insertStr = "INSERT INTO mainschenma.towns (\"Название\") VALUES (:value);";
        QVariantMap params;
        QString val = "\'";
        val.append(values.value("Название").toString());
        val.append("\'");
        params.insert(":value", val);

        int id = getContext().executeQuery(this, insertStr.toUtf8(), &params, &CityModule::callback, this);

        if ( id != -1 ) return true;
        return false;
    }
    Q_INVOKABLE bool editRow(int row, const QVariantMap &values) override {
        const QString updateStr = "UPDATE mainschenma.towns SET (\"Название\" = :value) WHERE id = :term";
        QVariantMap params;
        QString val = "\'";
        val.append(values.value("Название").toString());
        val.append("\'");
        params.insert(":value", val);
        params.insert(":term", row);

        int id = getContext().executeQuery(this, updateStr.toUtf8(), &params, &CityModule::callback, this);
        if ( id != -1 ) return true;
        return false;
    }
    Q_INVOKABLE bool removeRow(int row) override {
        const QString deleteStr = "DELETE FROM mainschenma.towns WHERE id = :term";
        QVariantMap params;
        params.insert(":term", row);
        int id = getContext().executeQuery(this, deleteStr.toUtf8(), &params, &CityModule::callback, this);
        if ( id != -1 ) return true;
        return false;
    }
    static void callback (void* user, int requestId,
                  const QVariantList &rows, const char *error) {
        CityModule* self = static_cast<CityModule*>(user);
        if (error) {
            self->setModelData(rows);
            self->loadData();
        }
        else {

        }
        return;
    }
    void loadData() override {
        QString sql = "SELECT id, \"Название\" FROM mainschenma.\"Город\" ORDER BY id";
        QVariantMap params;
        getContext().executeQuery(this, sql.toUtf8().constData(), &params, callback, this);
    }
    void setModelData(const QVariantList &rows) {
        m_model->setData(rows);
    }
private:
    std::unique_ptr<TableModel_Dictionary> m_model;
};


MODULE_EXPORT void* City(Context ctx){
    return static_cast<void*>(new CityModule(ctx));
}

