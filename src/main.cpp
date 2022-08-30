#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#ifndef QT_NO_SYSTEMTRAYICON

#include "tray.h"
#include "mailnagdbus.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString title = QObject::tr("Mailnag Tray");

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QString message = QObject::tr(
            "No system tray detected on this system."
        );
        QMessageBox::critical(nullptr, title, message);
        qDebug() << message;
        return 1;
    } else if (!MailnagDBus::running()) {
        QString message = QObject::tr(
            "Mailnag daemon not running."
        );
        QMessageBox::critical(nullptr, title, message);
        qDebug() << message;
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    Tray tray;

    return app.exec();
}

#else // not QT_NO_SYSTEMTRAYICON

#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString text = QObject::tr(
        "QSystemTrayIcon is not supported on this platform"
    );

    QMessageBox::critical(nullptr, QObject::tr("Mailnag Tray"), text);

    qDebug() << text;

    return 1;
}

#endif // QT_NO_SYSTEMTRAYICON
