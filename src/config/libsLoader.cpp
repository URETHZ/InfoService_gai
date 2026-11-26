#include "libsLoader.h"
#include "authorization.h"
#include "moduleAPI.h"
#include <QDir>
#include <QJSEngine>

// Анонимное пространство имен — функции видны ТОЛЬКО в этом файле
namespace {
    int executeQueryImpl(void *app, const char *sql, const QVariantMap *params, ResultFn callback, void *user) {
        return 1;
    }

    void cancelRequestsImpl(void *app, void *user) {
        // Отмена
    }
    }

    LibsLoader&  LibsLoader::instance() {
        static LibsLoader loader;
        return loader;
}

LibsLoader*  LibsLoader::create(QQmlEngine *, QJSEngine *) {
    QJSEngine::setObjectOwnership(&instance(), QJSEngine::CppOwnership);
    return &instance();
}
bool LibsLoader::isLoaded(QString dllName) {
    return LibsLoader::loaded.value(dllName, 0);
}
bool LibsLoader::loadModule(QString dllName) {
    if(!LibsLoader::isLoaded(dllName)) {
        QString path = libsPath + "\\" + dllName;
        QLibrary *lib = new QLibrary(path, this);
        if (!lib->load()) {
            QString err = lib->errorString();
            delete lib;
            return false;
        }
        loaded.insert(dllName, lib);
    }
    return true;
}

Context createContext(int uR, ExecuteQueryFn eQ, CancelRequestsFn cR) {
    Context ctx = {
        uR,
        eQ,
        cR
    };
    return ctx;
}
// Выполняем из обработчика метод создания объекта - результат передаем в контекст окна
QObject* LibsLoader::runMouduleFunc(QString dllName, QString funcName, int menu_id) {

    QLibrary *lib = loaded.value(dllName);
    typedef void* (*CreateFunc)(Context);
    CreateFunc createModule = (CreateFunc)lib->resolve(funcName.toUtf8());

    if (createModule) {
        // Получаем права,выданные на данный модуль
        int rights = Authorization::instance().getRightsToModule(menu_id);

        Context ctx = createContext(rights, executeQueryImpl, cancelRequestsImpl);
        // Получаем void* и приводим его к нашему интерфейсу
        void* rawPtr = createModule(ctx);

        return static_cast<QObject*>(rawPtr);
    }
    m_error = "функция не найдена";
    return nullptr;
}