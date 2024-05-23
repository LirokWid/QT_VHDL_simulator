#include "eventfilter.h"

#include <QApplication>

EventFilter::EventFilter(QWidget *parent):
    QWidget(parent)
{
}
bool EventFilter::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QPoint p = QCursor::pos();
        QWidget *w = QApplication::widgetAt(p);
        qDebug() << w;
    }
}
