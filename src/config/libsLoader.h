#ifndef LIBSLOADER_H
#define LIBSLOADER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QtQml/qqmlregistration.h>
#include <QLibrary>
class QQmlEngine;
class QJSEngine;
// Загрузчик dll принимает на вход параметр: имя .dll файла
class LibsLoader : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    static LibsLoader& instance();
    static LibsLoader* create(QQmlEngine *, QJSEngine *);
    bool isLoaded(QString dllName);
    bool load(QString dllName);
    Q_INVOKABLE bool run(QString dllName, QString funcName, void* context);
signals:
private:
    explicit LibsLoader(QObject *parent = nullptr);
    QHash<QString,LibsLoader*> loaded;
    QVariantList dllDescriotion;
};

#endif // LIBSLOADER_H
