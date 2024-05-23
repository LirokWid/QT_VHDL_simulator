#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QWidget>
#include <QDebug>
#include <QEvent>

class EventFilter : public QWidget
{
    Q_OBJECT
public:
    explicit EventFilter(QWidget *parent = 0);
signals:
protected:
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // EVENTFILTER_H
