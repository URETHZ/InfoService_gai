#ifndef LIBSLOADER_H
#define LIBSLOADER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QtQml/qqmlregistration.h>
#include <QLibrary>
#include <QDir>
#include <QJSEngine>
#include <QCoreApplication>

class QQmlEngine;
class QJSEngine;
// Загрузчик dll принимает на вход параметр: имя .dll файла
class LibsLoader : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
public:
    static LibsLoader& instance();
    static LibsLoader* create(QQmlEngine *, QJSEngine *);
    bool isLoaded(QString dllName);
    Q_INVOKABLE bool loadModule(QString dllName);
    Q_INVOKABLE QObject* runMouduleFunc(QString dllName, const QString funcName, int menu_id);
    QString error() const { return m_error; }
signals:
    void errorChanged();
private:
    explicit LibsLoader(QObject *parent = nullptr) : QObject(parent) {}
    QHash<QString,QLibrary*> loaded;
    QString libsPath = QDir(QCoreApplication::applicationDirPath()).filePath("libs");
    QString m_error = "";
};

#endif // LIBSLOADER_H
