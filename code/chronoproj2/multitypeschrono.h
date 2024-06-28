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


class MultiTypesChrono : public QWidget
{
    Q_OBJECT

public:
    MultiTypesChrono(QWidget *parent = nullptr);
    void addPoint(bool point);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Constants the graph
    const int labelOffset = 15;
    const int tickSize = 5;
    const int textOffset = 10;
    const int textWidth = 30;
    const int textHeight = 10;
    const int xLabelDensity = 1;

    const int marginLeft = 40;
    const int marginTop = 50;
    const int marginBottom = 50;

    const int textRectPadding = 5;
    const int textRectRadius = 2;

    int visibleRange = 10;
    const int pointRadius = 2;
    const int minDisplayedSteps = 2;

    const QColor backgrounColor = Qt::black;
    const QColor axisColor = Qt::green;
    const QColor graphColor = Qt::red;
    const QColor rectBorderColor = Qt::red;


    int currentOffset = 0;
    double stepPixelNb = 60.f;

    QSlider *slider;
    QVBoxLayout *Vlayout;
    void drawBackScale(QPainter *painter);
    QVector<bool> boolDataPoints;
    QPushButton *plusButton;
    QPushButton *minusButton;
    QPushButton *fitButton;
    QHBoxLayout *buttonLayout;

    void updateSliderRange();
    void calculateVisibleRange();
    void initializeBoolDataPoints();

    bool isDragging = false;
    QPoint dragStartPoint;
    QPoint dragEndPoint;

    bool isRightClicking = false;
    bool isFirstRightClick = false;
    QPoint rightClickStartPoint;

    void getStepPixelSize();
    void calculateStepPixelNb();
    void initializeBoolDataPoints(int nbPoints);
    void drawText(QPainter &painter, const QString &text, int x, int y);
    void drawTextInBox(QPainter &painter, const QString &text, int x, int y);

private slots:
    void handlePlusButton();
    void handleMinusButton();
    void handleFitButton();
};

#endif // MUTITYPESCHRONO_H
