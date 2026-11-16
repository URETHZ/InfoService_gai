#include <QRandomGenerator>
#include <QString>
#include "argon2.h"

// Для регистрации нового пароля
QString hashPassword(const QString& password) {
    // случайная соль (16 байт)
    QByteArray salt(16, 0);
    QRandomGenerator::global()->generate(salt.begin(), salt.end());

    // буфер для хеша (32 байт)
    size_t hashLen = 32;
    QByteArray hash(hashLen, 0);

    argon2id_hash_raw(
        3,              // t_cost (3 итерации)
        1 << 16,        // m_cost (65536 КБ = 64 МБ памяти)
        1,              // 1 поток
        password.toUtf8().constData(), password.toUtf8().length(),
        salt.constData(), salt.length(),
        hash.data(), hashLen
        );

    // В БД нужно сохранить И хеш, И соль
    return salt.toHex() + ":" + hash.toHex();
}
// Для проверки введенного пароля
QString gethashPasswordbySalt(const QString& password, const QString& hexSalt) {
    QByteArray salt = QByteArray::fromHex(hexSalt.toUtf8());
    size_t hashLen = 32;
    QByteArray hash(hashLen, 0);
    QByteArray passBytes = password.toUtf8();

    argon2id_hash_raw(
        3,
        1 << 16,
        1,
        passBytes.constData(), passBytes.length(),
        salt.constData(), salt.length(),
        hash.data(), hashLen
        );

    return hash.toHex();
}