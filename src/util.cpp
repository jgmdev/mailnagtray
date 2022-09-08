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
