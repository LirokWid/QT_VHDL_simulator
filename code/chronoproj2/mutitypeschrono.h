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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    const int maxStep = 10;
    int widgetWidth, widgetHeight;
    int currentOffset = 0;
    int visibleRange = 10; // Init value for points seen at the same time
    int stepPixelNb = 60; // New variable to store the width of a step in pixels
    int margin = 40;
    const int label_margin = 20;

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

    bool isDragging = false;
    QPoint dragStartPoint;
    QPoint dragEndPoint;

    bool isRightClicking = false;
    bool isFirstRightClick = false;
    QPoint rightClickStartPoint;

    void getStepPixelSize();
    void updateStepPixelNb();
    void initializeBoolDataPoints(int nbPoints);
private slots:

    void handlePlusButton();
    void handleMinusButton();
    void handleFitButton();
};

#endif // MUTITYPESCHRONO_H
