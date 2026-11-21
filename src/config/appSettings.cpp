#include "appSettings.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QJSEngine>

AppSettings::AppSettings(QObject *parent) : QObject(parent) { load(); }

AppSettings& AppSettings::instance()
{
    static AppSettings inst;
    return inst;
}


AppSettings* AppSettings::create(QQmlEngine *, QJSEngine *) {
    QJSEngine::setObjectOwnership(&instance(), QJSEngine::CppOwnership);
    return &instance();   // QML получает тот же объект, что и C++
}

QString AppSettings::filePath() const {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(dir);
    return dir + "/settings.ini";
}

void AppSettings::load() {
    QSettings s(filePath(), QSettings::IniFormat);
    m_serverIp       = s.value("server/ip", "localhost").toString();
    m_serverPort     = s.value("server/port", 5432).toInt();
    m_loginImagePath = s.value("ui/loginImage", "images/authFrameImage/i1.svg").toString();
}

void AppSettings::save() {
    QSettings s(filePath(), QSettings::IniFormat);
    s.setValue("server/ip", m_serverIp);
    s.setValue("server/port", m_serverPort);
    s.setValue("ui/loginImage", m_loginImagePath);
}

void AppSettings::setServerIp(const QString &v) {
    if (m_serverIp == v) return;
    m_serverIp = v; emit serverIpChanged();
}
void AppSettings::setServerPort(int v) {
    if (m_serverPort == v) return;
    m_serverPort = v; emit serverPortChanged();
}
void AppSettings::setLoginImagePath(const QString &v) {
    if (m_loginImagePath == v) return;
    m_loginImagePath = v; emit loginImagePathChanged();
}