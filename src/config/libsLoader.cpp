#include "libsLoader.h"

LibsLoader&  LibsLoader::instance(){

}
LibsLoader*  LibsLoader::create(QQmlEngine *, QJSEngine *) { }
bool  LibsLoader::isLoaded(QString dllName) {

}
bool  LibsLoader::load(QString dllName) {

}
// Выполняем из обработчика метод создания объекта - результат передаем в контекст окна
bool  LibsLoader::run(QString dllName, QString funcName, void* context) {

}