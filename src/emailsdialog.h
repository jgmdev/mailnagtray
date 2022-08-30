#ifndef MAILNAGTRAY_EMAILSDIALOG_H
#define MAILNAGTRAY_EMAILSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QList>
#include <QString>

#include "mailnagdbus.h"

struct EmailStats {
    QString account;
    long long messages;
};

QT_BEGIN_NAMESPACE
class QListWidget;
class QVBoxLayout;
class QTreeWidget;
class QListWidgetItem;
QT_END_NAMESPACE

class EmailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmailsDialog();
    void addMessage(MailnagMessage message);
    void clearMessages();
    void removeMessage(QString account, QString id);
    QList<EmailStats> getStats();

public slots:
    void accountActivated(QListWidgetItem *item);

private:
    QMap<QString, QListWidgetItem*> _accountsMap;
    QMap<QString, QList<MailnagMessage>> _messagesMap;
    QVBoxLayout *_layout;
    QListWidget *_emailsList;
    QTreeWidget *_messagesList;
};

#endif //MAILNAGTRAY_EMAILSDIALOG_H
