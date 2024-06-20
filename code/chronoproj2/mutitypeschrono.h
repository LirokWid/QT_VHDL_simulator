#ifndef MUTITYPESCHRONO_H
#define MUTITYPESCHRONO_H

#include <QApplication>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QVector>
#include <QDebug>


class MutiTypesChrono : public QWidget
{
    Q_OBJECT
public:
    MutiTypesChrono(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const int maxStep = 10;

    int width, height;
    int currentOffset = 0;

    const int visibleRange = 10; // Number of points visible at a time
    const int fixedGraphWidth = 600; // Fixed width for the graph area


    QSlider *slider;
    QVBoxLayout *Vlayout;
    void drawBackScale(QPainter *painter);
    QVector<QPointF> dataPoints;

    void updateSliderRange();
};

#endif // MUTITYPESCHRONO_H
