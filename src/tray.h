#ifndef MAILNAGTRAY_TRAY_H
#define MAILNAGTRAY_TRAY_H

#include <QSystemTrayIcon>
#include <QDBusMessage>

#include "mailnagdbus.h"

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

class EmailsDialog;

class Tray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    Tray();
    void loadIcons();
    void setupSettings();
    void setupMenu();
    void setupTimer();
    void registerMessages(QList<MailnagMessage> messages);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void onMailnagClicked();
    void onOnlineAccountsClicked();
    void onSettingsClicked();
    void onMailTimer();
    void onMailsAdded(QDBusMessage message);
    void onMailsRemoved(QDBusMessage message);

private:
    EmailsDialog *_emailsDialog;

    QIcon _mailUnreadIcon;
    QIcon _mailReadIcon;

    QMenu *_iconMenu;
    QAction *_settingsAction;
    QAction *_mailNagConfigAction;
    QAction *_onlineAccountsAction;
    QAction *_quitAction;

    QTimer *_mailTimer;
};

#endif //MAILNAGTRAY_TRAY_H
