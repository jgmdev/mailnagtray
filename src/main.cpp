#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QString>

#ifndef QT_NO_SYSTEMTRAYICON

#include "tray.h"
#include "mailnagdbus.h"

void print_help() {
    printf("Mailnag Tray - cross DE systray notifications implemented in QT.\n");
    printf("\n");
    printf("Usage: mailnagtray [OPTION]\n");
    printf("\n");
    printf("  -h, --help       Show command-line help.\n");
    printf("  -s, --startup    Indicates the application is running at\n");
    printf("                   startup to perform a sleep delay for proper\n");
    printf("                   initialization.\n");
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        for (int i=1; i<argc; i++) {
            QString argument(argv[i]);
            if (argument == "-h" || argument == "--help") {
                print_help();
                return 0;
            } else if (argument == "-s" || argument == "--startup") {
                QThread::sleep(3);
            } else {
                print_help();
                return 0;
            }
        }
    }

    QApplication app(argc, argv);

    QString title = QObject::tr("Mailnag Tray");

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        int runs = 0;
        bool tray_found = true;
        do {
            QThread::sleep(1);
            runs++;
            if (runs == 5) {
                tray_found = false;
                break;
            }
        } while(!QSystemTrayIcon::isSystemTrayAvailable());
        if (!tray_found) {
            QString message = QObject::tr(
                "No system tray detected on this system."
            );
            QMessageBox::critical(nullptr, title, message);
            qDebug() << message;
            return 1;
        }
    }

    if (!MailnagDBus::running()) {
        int runs = 0;
        bool running = true;
        do {
            QThread::sleep(1);
            runs++;
            if (runs == 10) {
                running = false;
                break;
            }
        } while(!MailnagDBus::running());
        if (!running) {
            QString message = QObject::tr(
                "Mailnag daemon not running."
            );
            QMessageBox::critical(nullptr, title, message);
            qDebug() << message;
            return 1;
        }
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
