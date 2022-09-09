#ifndef MAILNAGTRAY_UTIL_H
#define MAILNAGTRAY_UTIL_H

#include <QString>

class Util
{
public:
    static void openMailnagConfig();
    static void openOnlineAccounts();
    static void showNotification(
        QString title, QString message, QString icon="mailnag", int timeout=10
    );
};

#endif //MAILNAGTRAY_TRAY_H
