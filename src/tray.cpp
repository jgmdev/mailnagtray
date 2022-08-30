#include "tray.h"

#include <QDialog>
#include <QCoreApplication>
#include <QDBusArgument>
#include <QDebug>
#include <QStringList>
#include <QMenu>
#include <QProcess>
#include <QTimer>
#include <QFileInfo>
#include <QMessageBox>

#include "emailsdialog.h"

Tray::Tray() : QSystemTrayIcon() {
    loadIcons();

    setupSettings();
    setupMenu();

    MailnagDBus::checkForNewMail();

    setupTimer();
    setIcon(_mailReadIcon);
    setToolTip(tr(QString::asprintf("%d new messages", 0).toUtf8()));

    connect(this, &QSystemTrayIcon::activated, this, &Tray::iconActivated);

    MailnagDBus::connectSignalMailsAdded(
        this,
        SLOT(onMailsAdded(QDBusMessage))
    );

    MailnagDBus::connectSignalMailsRemoved(
        this,
        SLOT(onMailsRemoved(QDBusMessage))
    );

    show();
}

void Tray::loadIcons() {
    _mailReadIcon = QIcon::fromTheme("mail-read");
    _mailUnreadIcon = QIcon::fromTheme("mail-unread");
}

void Tray::setupSettings() {
    _emailsDialog = new EmailsDialog;
}

void Tray::setupMenu() {
    _iconMenu = new QMenu();

    _settingsAction = new QAction(QIcon::fromTheme("mail-unread"), tr("0 new messages"), this);
    connect(_settingsAction, &QAction::triggered, this, &Tray::onSettingsClicked);
    _iconMenu->addAction(_settingsAction);

    _iconMenu->addSeparator();

    _mailNagConfigAction = new QAction(QIcon::fromTheme("mailnag"), tr("&Mailnag Configuration"), this);
    connect(_mailNagConfigAction, &QAction::triggered, this, &Tray::onMailnagClicked);
    _iconMenu->addAction(_mailNagConfigAction);

    _onlineAccountsAction = new QAction(QIcon::fromTheme("org.gnome.Settings-online-accounts-symbolic"), tr("&Online Accounts"), this);
    connect(_onlineAccountsAction, &QAction::triggered, this, &Tray::onOnlineAccountsClicked);
    _iconMenu->addAction(_onlineAccountsAction);

    _iconMenu->addSeparator();

    _quitAction = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this);
    connect(_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    _iconMenu->addAction(_quitAction);

    setContextMenu(_iconMenu);
}

void Tray::setupTimer() {
    _mailTimer = new QTimer(this);
    connect(_mailTimer, &QTimer::timeout, this, &Tray::onMailTimer);
    _mailTimer->start(3000);
}

void Tray::registerMessages(QList<MailnagMessage> messages) {
    _emailsDialog->clearMessages();
    for (const MailnagMessage &message : messages) {
        _emailsDialog->addMessage(message);
    }

    if (messages.count() > 0)
        setIcon(_mailUnreadIcon);
    else
        setIcon(_mailUnreadIcon);

    QString messagesText = tr(
        QString::asprintf("%lld new messages", messages.count()).toUtf8()
    );

    setToolTip(messagesText);
    _settingsAction->setText(messagesText);
}

void Tray::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            onSettingsClicked();
            break;
        default:
            break;
    }
}

void Tray::onMailnagClicked() {
    if (QFileInfo::exists("/usr/bin/mailnag-config"))
        QProcess::execute("mailnag-config", QStringList());
    else
        QMessageBox::critical(
            nullptr,
            tr("mailnag-config not found"),
            tr(
                "The Mailnag Configuration tool was not found.\n\n"
                "Install Mailnag for this application to work properly."
            )
        );
}

void Tray::onOnlineAccountsClicked() {
    if (QFileInfo::exists("/usr/bin/gnome-control-center"))
        QProcess::execute("gnome-control-center", QStringList("online-accounts"));
    else
        QMessageBox::critical(
            nullptr,
            tr("gnome-control-center not found"),
            tr(
                "Gnome Control Center was not found.\n\n"
                "Install gnome-control-center to easily setup email accounts "
                "with missing or sketchy imap and pop access like gmail ones."
            )
        );
}

void Tray::onSettingsClicked() {
    if (!_emailsDialog->isVisible())
        _emailsDialog->showMaximized();
    else
        _emailsDialog->hide();
}

void Tray::onMailTimer() {
    int messagesCount = MailnagDBus::getMailCount();

    if (messagesCount > 0) {
        QList<MailnagMessage> messages = MailnagDBus::getMessages();

        Tray::registerMessages(messages);

        QList<EmailStats> stats = _emailsDialog->getStats();
        for (const EmailStats &stat : stats) {
            showMessage(
                tr("New E-mails"),
                stat.account + " (" + QString::number(stat.messages) + ")",
                QSystemTrayIcon::MessageIcon::Information
            );
        }

        _mailTimer->stop();
    }
}

void Tray::onMailsAdded(QDBusMessage message) {
    QDBusArgument newMessages = message.arguments()[0].value<QDBusArgument>();
    QDBusArgument allMessages = message.arguments()[1].value<QDBusArgument>();

    QList<MailnagMessage> new_messages = MailnagDBus::toMessages(&newMessages);
    QList<MailnagMessage> all_messages = MailnagDBus::toMessages(&allMessages);

    Tray::registerMessages(all_messages);

    int count = 0;
    for (const MailnagMessage &message : new_messages) {
        showMessage(
            tr("New e-mail") + " (" + message.account_name + ")"
            , message.sender_name + ":\n"
              + message.subject
            , QSystemTrayIcon::MessageIcon::Information
        );

        count++;

        if (count == 10)
            break;
    }
}

void Tray::onMailsRemoved(QDBusMessage message) {
    QDBusArgument remainingMessages = message.arguments()[0].value<QDBusArgument>();

    QList<MailnagMessage> remaining_messages = MailnagDBus::toMessages(
        &remainingMessages
    );

    Tray::registerMessages(remaining_messages);
}
