#include "tray.h"

#include <QCoreApplication>
#include <QDBusArgument>
#include <QMenu>
#include <QTimer>

#include "util.h"
#include "emailswindow.h"

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
    _emailsWindow = new EmailsWindow;
}

void Tray::setupMenu() {
    _iconMenu = new QMenu();

    _messagesAction = new QAction(QIcon::fromTheme("mail-unread"), tr("0 new messages"), this);
    connect(_messagesAction, &QAction::triggered, this, &Tray::onSettingsClicked);
    _iconMenu->addAction(_messagesAction);

    _iconMenu->addSeparator();

    _mailNagConfigAction = new QAction(QIcon::fromTheme("mailnag"), tr("&Mailnag Configuration"), this);
    connect(_mailNagConfigAction, &QAction::triggered, &Util::openMailnagConfig);
    _iconMenu->addAction(_mailNagConfigAction);

    _onlineAccountsAction = new QAction(QIcon::fromTheme("org.gnome.Settings-online-accounts-symbolic"), tr("&Online Accounts"), this);
    connect(_onlineAccountsAction, &QAction::triggered, &Util::openOnlineAccounts);
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
    _emailsWindow->clearMessages();
    for (const MailnagMessage &message : messages) {
        _emailsWindow->addMessage(message);
    }
    _emailsWindow->updateMessages();

    if (messages.count() > 0)
        setIcon(_mailUnreadIcon);
    else
        setIcon(_mailUnreadIcon);

    QString messagesText = tr(
        QString::asprintf("%lld new messages", messages.count()).toUtf8()
    );

    setToolTip(messagesText);
    _messagesAction->setText(messagesText);
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

void Tray::onSettingsClicked() {
    if (!_emailsWindow->isVisible())
        _emailsWindow->showMaximized();
    else
        _emailsWindow->hide();
}

void Tray::onMailTimer() {
    int messagesCount = MailnagDBus::getMailCount();

    if (messagesCount > 0) {
        QList<MailnagMessage> messages = MailnagDBus::getMessages();

        Tray::registerMessages(messages);

        QList<EmailStats> stats = _emailsWindow->getStats();
        for (const EmailStats &stat : stats) {
            Util::showNotification(
                tr("New E-mails"),
                stat.account + " (" + QString::number(stat.messages) + ")"
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
        Util::showNotification(
            tr("New e-mail") + " (" + message.account_name + ")"
            , message.sender_name + ":\n"
              + message.subject
            , "mail-unread"
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
