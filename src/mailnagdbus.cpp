#include "mailnagdbus.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusMessage>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusArgument>
#include <QDebug>
#include <QStringList>
#include <QVariantMap>
#include <QObject>

bool MailnagDBus::running() {
    QDBusReply<QStringList> reply = QDBusConnection::sessionBus()
        .interface()->registeredServiceNames()
    ;

    if (!reply.isValid()) {
        qDebug() << QObject::tr("Error:") << " " << reply.error().message();
        return false;
    }

    const QStringList values = reply.value();
    for (const QString &name : values) {
        if (name == "mailnag.MailnagService")
            return true;
    }

    return false;
}

void MailnagDBus::checkForNewMail() {
    QDBusInterface interface(
        "mailnag.MailnagService",
        "/mailnag/MailnagService"
    );

    interface.call("CheckForMails");
}

void MailnagDBus::markMessageRead(QString id) {
    QDBusInterface interface(
        "mailnag.MailnagService",
        "/mailnag/MailnagService"
    );

    interface.call("MarkMailAsRead", id);
}

void MailnagDBus::markMessagesRead(QList<MailnagMessage> messages) {
    for (const MailnagMessage &message : messages) {
        MailnagDBus::markMessageRead(message.id);
    }
}

int MailnagDBus::getMailCount() {
    QDBusInterface interface(
        "mailnag.MailnagService",
        "/mailnag/MailnagService"
    );

    QDBusReply<unsigned int> reply = interface.call("GetMailCount");

    if (reply.isValid()) {
        return reply.value();
    } else {
        qDebug() << QObject::tr("Error:") << " " << reply.error().message();
    }

    return 0;
}

QList<MailnagMessage> MailnagDBus::getMessages() {
    if (MailnagDBus::running()) {
        QDBusInterface interface(
            "mailnag.MailnagService",
            "/mailnag/MailnagService"
        );

        QDBusMessage reply = interface.call("GetMails");
        QDBusArgument messages = reply.arguments()[0].value<QDBusArgument>();
        QList<MailnagMessage> list = MailnagDBus::toMessages(&messages);

        return list;
    } else {
        qDebug() << QObject::tr("Error: mailnag service not running");
    }

    return QList<MailnagMessage>();
}

bool MailnagDBus::connectSignal(QString method, QObject *receiver, const char *slot) {
    static QDBusConnection connection = QDBusConnection::connectToBus(
        QDBusConnection::SessionBus,
        "mailnag.MailnagService"
    );
    if (connection.isConnected()) {
        bool connected = connection.connect(
            "mailnag.MailnagService",
            "/mailnag/MailnagService",
            "",
            method,
            receiver,
            slot
        );

        if (!connected) {
            qDebug() << QObject::tr(
                QString::asprintf(
                    "Error: no connected to %s signal",
                    method.toStdString().c_str()\
                ).toUtf8()
            );
        } else {
            return true;
        }
    } else {
        qDebug() << QObject::tr("Error: session bus connection failed");
    }

    return false;
}

bool MailnagDBus::connectSignalMailsAdded(QObject *receiver, const char *slot) {
    return MailnagDBus::connectSignal("MailsAdded", receiver, slot);
}

bool MailnagDBus::connectSignalMailsRemoved(QObject *receiver, const char *slot) {
    return MailnagDBus::connectSignal("MailsRemoved", receiver, slot);
}

QList<MailnagMessage> MailnagDBus::toMessages(const QDBusArgument *messages) {
    MailnagMessage message;
    QVariantMap map;
    QList<MailnagMessage> list;

    messages->beginArray();
    while(!messages->atEnd()) {
        *messages >> map;
        message.datetime = map["datetime"].toInt();
        message.subject = map["subject"].toString();
        message.sender_name = map["sender_name"].toString();
        message.sender_addr = map["sender_addr"].toString();
        message.account_name = map["account_name"].toString();
        message.id = map["id"].toString();
        list.append(message);
    }

    if (list.count() <= 0) {
        qDebug() << QObject::tr("Error: failed converting argument to mail messages");
    }

    return list;
}
