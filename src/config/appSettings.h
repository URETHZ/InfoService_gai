#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>
class QQmlEngine;
class QJSEngine;

class AppSettings : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString loginImagePath READ loginImagePath WRITE setLoginImagePath NOTIFY loginImagePathChanged)

public:
    static AppSettings& instance();                                  // для C++
    static AppSettings* create(QQmlEngine *, QJSEngine *);           // для QML

    QString serverIp() const { return m_serverIp; }
    int serverPort() const { return m_serverPort; }
    QString loginImagePath() const { return m_loginImagePath; }


    void setServerIp(const QString &v);
    void setServerPort(int v);
    void setLoginImagePath(const QString &v);

    Q_INVOKABLE void save();   // вызываем из QML по кнопке "Сохранить"
    void load();


signals:
    void serverIpChanged();
    void serverPortChanged();
    void loginImagePathChanged();

private:
    explicit AppSettings(QObject *parent = nullptr);

    QString filePath() const;
    QString m_serverIp = "localhost";
    int m_serverPort = 5432;
    QString m_loginImagePath = "images/authFrameImage/i1.svg";
};