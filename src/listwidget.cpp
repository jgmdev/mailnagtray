#include "listwidget.h"

#include <QMouseEvent>

ListWidget::ListWidget(): QListWidget() {}

void ListWidget::mousePressEvent(QMouseEvent *e)
{
    QListWidget::mousePressEvent(e);

    if(e->button()==Qt::RightButton)
        emit rightClick();
    else
        emit leftClick();
}
