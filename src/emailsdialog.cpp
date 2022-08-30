#include "emailsdialog.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QDate>

EmailsDialog::EmailsDialog() : QDialog() {
    setWindowTitle(tr("New E-mails"));

    _layout = new QVBoxLayout(this);
    setLayout(_layout);

    QLabel *emailsLabel = new QLabel(tr("E-mail Accounts:"), this);
    _layout->addWidget(emailsLabel);

    _emailsList = new QListWidget(this);
    _emailsList->setIconSize(QSize(48, 48));
    _layout->addWidget(_emailsList);
    _emailsList->setMaximumHeight(200);
    connect(_emailsList, &QListWidget::itemClicked, this, &EmailsDialog::accountActivated);

    QLabel *messagesLabel = new QLabel(tr("Messages:"), this);
    _layout->addWidget(messagesLabel);

    _messagesList = new QTreeWidget(this);
    _messagesList->setRootIsDecorated(false);
    _messagesList->setIndentation(0);
    _messagesList->setColumnCount(4);
    _messagesList->headerItem()->setText(0, tr("Date"));
    _messagesList->setColumnWidth(0, 300);
    _messagesList->headerItem()->setText(1, tr("From"));
    _messagesList->setColumnWidth(1, 200);
    _messagesList->headerItem()->setText(2, tr("E-mail"));
    _messagesList->setColumnWidth(2, 300);
    _messagesList->headerItem()->setText(3, tr("Subject"));
    _layout->addWidget(_messagesList);

    setMinimumWidth(600);
}

void EmailsDialog::addMessage(MailnagMessage message) {
    if (!_accountsMap[message.account_name]) {
        QListWidgetItem *item = new QListWidgetItem(
            QIcon::fromTheme("email"),
            message.account_name
        );
        item->setWhatsThis(message.account_name);
        _accountsMap[message.account_name] = item;
        _emailsList->addItem(item);
    }
    _messagesMap[message.account_name].append(message);

    QString emailText = message.account_name;
    emailText += " ("
        + QString::number(_messagesMap[message.account_name].size())
        + ")"
    ;
    _accountsMap[message.account_name]->setText(emailText);
}

void EmailsDialog::clearMessages() {
    _messagesMap.clear();
}

void EmailsDialog::removeMessage(QString account, QString id) {
    size_t pos = 0;
    for (const MailnagMessage &message : _messagesMap[account]) {
        if (message.id == id) {
            _messagesMap[account].removeAt(pos);
            break;
        }
        pos++;
    }
}

void EmailsDialog::accountActivated(QListWidgetItem *item) {
    QString account = item->whatsThis();
    _messagesList->clear();
    for (const MailnagMessage &message : _messagesMap[account]) {
        QDateTime time = QDateTime::fromSecsSinceEpoch(message.datetime);
        time.toString("%a %_d %b, %I:%M %p");
        QStringList columns = {
            time.toString("ddd d MMMM yyyy, h:m ap"),
            message.sender_name,
            message.sender_addr,
            message.subject
        };
        QTreeWidgetItem *item = new QTreeWidgetItem(columns);
        item->setIcon(0, QIcon::fromTheme("mail-unread"));
        _messagesList->addTopLevelItem(item);
    }
}

QList<EmailStats> EmailsDialog::getStats() {
    QList<EmailStats> stats;
    for (const QListWidgetItem *item : _accountsMap) {
        stats.append({
            .account = item->whatsThis(),
            .messages = _messagesMap[item->whatsThis()].count()
        });
    }
    return stats;
}
