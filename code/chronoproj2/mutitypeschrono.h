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
#include <QPushButton>


class MutiTypesChrono : public QWidget
{
    Q_OBJECT
public:
    MutiTypesChrono(QWidget *parent = nullptr);

    void addPoint(bool point);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    const int maxStep = 10;
    int widgetWidth, widgetHeight;
    int currentOffset = 0;
    int visibleRange = 10; // Init value for points seen at the same time
    int stepPixelSize = 60; // New variable to store the width of a step in pixels
    int margin = 40;

    QSlider *slider;
    QVBoxLayout *Vlayout;
    void drawBackScale(QPainter *painter);
    QVector<bool> boolDataPoints;
    QPushButton *plusButton;
    QPushButton *minusButton;
    QPushButton *fitButton;
    QHBoxLayout *buttonLayout;

    void updateSliderRange();
    void calculateVisibleRange(); // New method to calculate visible range
    void initializeBoolDataPoints(); // New method to initialize boolean data points

private slots:

    void handlePlusButton();
    void handleMinusButton();
    void handleFitButton();
};

#endif // MUTITYPESCHRONO_H
