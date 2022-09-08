#ifndef MAILNAGTRAY_LISTWIDGET_H
#define MAILNAGTRAY_LISTWIDGET_H

#include <QListWidget>

QT_BEGIN_NAMESPACE
class QMouseEvent;
QT_END_NAMESPACE

class ListWidget : public QListWidget
{
    Q_OBJECT

public:
    ListWidget();

public slots:
    void mousePressEvent(QMouseEvent* event);

signals:
    void rightClick();
    void leftClick();
};

#endif //MAILNAGTRAY_LISTWIDGET_H
