#ifndef MAILNAGTRAY_MAILNAGDBUS_H
#define MAILNAGTRAY_MAILNAGDBUS_H

#include <QString>
#include <QList>

struct MailnagMessage
{
    int datetime;
    QString subject;
    QString sender_name;
    QString sender_addr;
    QString account_name;
    QString id;
};

QT_BEGIN_NAMESPACE
class QObject;
class QDBusArgument;
QT_END_NAMESPACE

class MailnagDBus
{
public:
    static bool running();
    static void checkForNewMail();
    static void markMessageRead(QString id);
    static void markMessagesRead(QList<MailnagMessage> messages);
    static int getMailCount();
    static QList<MailnagMessage> getMessages();
    static QList<MailnagMessage> toMessages(const QDBusArgument *arg);
    static bool connectSignalMailsAdded(QObject *receiver, const char *slot);
    static bool connectSignalMailsRemoved(QObject *receiver, const char *slot);

private:
    static bool connectSignal(QString method, QObject *receiver, const char *slot);
};

#endif //MAILNAGTRAY_MAILNAGDBUS_H
