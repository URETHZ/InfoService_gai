#ifndef TABLEMODEL_DICTIONARY_H
#define TABLEMODEL_DICTIONARY_H

#include <QAbstractTableModel>
#include <QVariantList>
#include <QVariantMap>
#include <QStringList>

class TableModel_Dictionary : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit TableModel_Dictionary(QObject *parent = nullptr) : QAbstractTableModel(parent) {}

    // ОБЯЗАТЕЛЬНЫЕ МЕТОДЫ
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_data.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_headers.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_data.size()) return QVariant();

        // Если QML просит отображаемый текст
        if (role == Qt::DisplayRole) {
            QString header = m_headers.at(index.column());
            return m_data.at(index.row()).value(header);
        }

        // Если QML просит данные по имени роли (например, model.name)
        if (role == Qt::UserRole) {
            QString header = m_headers.at(index.column());
            return m_data.at(index.row()).value(header);
        }
        return QVariant();
    }

    // Возвращаем имена колонок (чтобы QML знал, к чему обращаться)
    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        // Мы используем одну универсальную роль, а имя колонки берем из заголовков
        // В QML это будет выглядеть как model[roleName]
        for (int i = 0; i < m_headers.size(); ++i) {
            roles[Qt::UserRole + i] = m_headers[i].toUtf8();
        }
        return roles;
    }

    // Заголовки для отображения в шапке таблицы
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            return m_headers.at(section);
        }
        return QVariant();
    }

    void setData(const QVariantList &newData) {
        beginResetModel(); // Говорим QML: "Сейчас всё изменится, подожди"

        m_data = newData;
        m_headers.clear();

        // Если данные пришли, берем имена колонок из первого словаря (ключа)
        if (!m_data.isEmpty()) {
            m_headers = m_data.first().toMap().keys();
        }

        endResetModel();   // Говорим QML: "Я закончил, перерисуй таблицу"
    }

private:
    QVariantList m_data;     // Сами данные
    QStringList m_headers;   // Названия колонок
};

#endif // TABLEMODEL_DICTIONARY_H
