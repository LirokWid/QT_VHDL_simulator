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
    void resizeEvent(QResizeEvent *event) override;

private:
    const int maxStep = 10;

    int widgetWidth, widgetHeight;
    int currentOffset = 0;

    const int fixedGraphWidth = 600; // Fixed width for the graph area

    QSlider *slider;
    QVBoxLayout *Vlayout;
    void drawBackScale(QPainter *painter);
    QVector<bool> boolDataPoints;

    void updateSliderRange();
    int calculateVisibleRange() const; // New method to calculate visible range
    void initializeBoolDataPoints(); // New method to initialize boolean data points
};

#endif // MUTITYPESCHRONO_H
