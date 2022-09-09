// we include libnotify first otherwise causes compilation issues
#include <libnotify/notify.h>

#include "util.h"

#include <QFileInfo>
#include <QProcess>
#include <QStringList>
#include <QMessageBox>
#include <QObject>

void Util::openMailnagConfig() {
    if (QFileInfo::exists("/usr/bin/mailnag-config"))
        QProcess::execute("mailnag-config", QStringList());
    else
        QMessageBox::critical(
            nullptr,
            QObject::tr("mailnag-config not found"),
            QObject::tr(
                "The Mailnag Configuration tool was not found.\n\n"
                "Install Mailnag for this application to work properly."
            )
        );
}

void Util::openOnlineAccounts() {
    if (QFileInfo::exists("/usr/bin/gnome-control-center"))
        QProcess::execute("gnome-control-center", QStringList("online-accounts"));
    else
        QMessageBox::critical(
            nullptr,
            QObject::tr("gnome-control-center not found"),
            QObject::tr(
                "Gnome Control Center was not found.\n\n"
                "Install gnome-control-center to easily setup email accounts "
                "with missing or sketchy imap and pop access like gmail ones."
            )
        );
}

void Util::showNotification(
    QString title, QString message, QString icon, int timeout
) {
    notify_init("mailnagtray");
	NotifyNotification *notification = notify_notification_new(
        title.toUtf8(), message.toUtf8(), icon.toUtf8()
    );
    notify_notification_set_timeout(notification, timeout * 1000);
	notify_notification_show(notification, NULL);
	g_object_unref(G_OBJECT(notification));
	notify_uninit();
}
