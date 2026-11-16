#ifndef HEXBYARGON2_H
#define HEXBYARGON2_H
#include <QString>

QString hashPassword(const QString& password);
QString gethashPasswordbySalt(const QString& password, const QString& hexSalt);

#endif // HEXBYARGON2_H
