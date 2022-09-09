#ifndef MAILNAGTRAY_EMAILSWINDOW_H
#define MAILNAGTRAY_EMAILSWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QList>
#include <QString>

#include "mailnagdbus.h"

struct EmailStats {
    QString account;
    long long messages;
};

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QTreeWidget;
class QListWidgetItem;
class QToolBar;
class QAction;
class QMouseEvent;
class ListWidget;
QT_END_NAMESPACE

class EmailsWindow : public QMainWindow
{
    Q_OBJECT

public:
    EmailsWindow();
    void addMessage(MailnagMessage message);
    void clearMessages();
    void updateMessages();
    void removeMessage(QString account, QString id);
    QList<EmailStats> getStats();

private:
    void onAbout();
    void accountActivated(QListWidgetItem *item);

private slots:
    void onEmailsRClick();
    void onEmailsLClick();

private:
    QMap<QString, QListWidgetItem*> _accountsMap;
    QMap<QString, QList<MailnagMessage>> _messagesMap;

    QWidget *_centralWidget;
    QVBoxLayout *_layout;
    ListWidget *_emailsList;
    QTreeWidget *_messagesList;

    QToolBar *_toolBar;
    QAction *_checkEmailsAction;
    QAction *_mailNagConfigAction;
    QAction *_onlineAccountsAction;
    QAction *_aboutAction;
};

#endif //MAILNAGTRAY_EMAILSWINDOW_H
