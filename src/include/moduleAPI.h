
#ifndef MODULEAPI_H
#define MODULEAPI_H
#include <QString>

#define MODULE_API_VERSION 1

#ifdef _WIN32
#define MODULE_EXPORT extern "C" __declspec(dllexport)
#else
#define MODULE_EXPORT extern "C"
#endif

using ResultFn = void (*)(void *user, int requestId,
                          const QVariantList &rows, const char *error);

using ExecuteQueryFn = int (*)(void *app, const char *sql,
                               const QVariantMap *params, ResultFn callback, void *user);

using CancelRequestsFn = void (*)(void *app);

struct Context {
    int userRights;
    // Для запроса
    int (*executeQuery)(void *app, const char *sql, const QVariantMap *params,
                        ResultFn callback);
    // Для отмены
    void (*cancelRequests)(void *app);
};



#endif // MODULEAPI_H
