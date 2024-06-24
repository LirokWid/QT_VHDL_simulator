#include "mutitypeschrono.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QPainter>

MutiTypesChrono::MutiTypesChrono(QWidget *parent)
    : QWidget(parent)
{
    slider = new QSlider(Qt::Horizontal, this);

    setMinimumSize(800, 200);
    setAutoFillBackground(true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(20, 20, 20));
    this->setPalette(palette);
    slider->setRange(0, 100);

    Vlayout = new QVBoxLayout(this);

    // Add margin to the layout to lower the slider
    Vlayout->setContentsMargins(30, 0, 30, 0); // Adjust bottom margin as needed
    Vlayout->addStretch(1);
    Vlayout->addWidget(slider);

    connect(slider, &QSlider::valueChanged, this, [=](int value)
            {
                currentOffset = value;
                qDebug() << "Slider value changed to " << value;
                update();
            });

    initializeBoolDataPoints();

    updateSliderRange();
}

void MutiTypesChrono::updateSliderRange()
{
    int visibleRange = calculateVisibleRange();
    if (boolDataPoints.size() > visibleRange)
    {
        slider->setRange(0, boolDataPoints.size() - visibleRange);
    }
    else
    {
        slider->setRange(0, 0);
    }
}

int MutiTypesChrono::calculateVisibleRange() const
{
    int margin = 30; // The margin used in the paintEvent method
    return (width() - 2 * margin) / (fixedGraphWidth / 10);
}

void MutiTypesChrono::initializeBoolDataPoints()
{
    for (int i = 0; i < 50; ++i)
    {
        boolDataPoints.append(rand() % 2); // Random boolean values
    }
}

void MutiTypesChrono::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int height = QWidget::height();
    int width = QWidget::width();
    const int label_margin = 20;

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);

    // Draw axes
    int margin = 40;
    int graphHeight = height - 2 * margin;
    int graphWidth = width - 2 * margin;

    // Draw y-axis
    painter.drawLine(margin, margin, margin, height - margin);
    painter.drawText(margin - 30 - label_margin , margin - 10, 30, 10, Qt::AlignRight, "1"); // High state
    painter.drawText(margin - 30 - label_margin , height - margin - 10, 30, 10, Qt::AlignRight, "0"); // Low state

    // Draw x-axis
    painter.drawLine(margin, height - margin, width - margin, height - margin);

    // Draw ticks and labels for x-axis
    int visibleRange = calculateVisibleRange();
    double xScale = static_cast<double>(graphWidth) / visibleRange;
    for (int i = 0; i <= visibleRange; ++i)
    {
        int x = margin + i * xScale;
        painter.drawLine(x, height - margin, x, height - margin + 5);
        painter.drawText(x - 10, height - margin + 15, 20, 10, Qt::AlignCenter, QString::number(currentOffset + i));
    }

    painter.setPen(Qt::green);

    // Draw boolean data points as a square wave with vertical lines between changes
    if (!boolDataPoints.isEmpty())
    {
        int startIndex = currentOffset;
        int endIndex = qMin(startIndex + visibleRange, boolDataPoints.size());

        double yScale = static_cast<double>(graphHeight);

        // Draw initial point
        double xPrev = margin;
        double yPrev = boolDataPoints[startIndex] ? margin : height - margin;
        painter.drawEllipse(QPointF(xPrev, yPrev), 3, 3);

        for (int i = startIndex + 1; i < endIndex; ++i) {
            double x = margin + (i - startIndex) * xScale;
            double y = boolDataPoints[i] ? margin : height - margin;
            painter.drawEllipse(QPointF(x, y), 3, 3);

            // Draw vertical line if boolean value changes
            if (boolDataPoints[i] != boolDataPoints[i - 1]) {
                painter.drawLine(QPointF(xPrev, yPrev), QPointF(xPrev, y));
                painter.drawLine(QPointF(xPrev, y), QPointF(x, y));
            } else {
                painter.drawLine(QPointF(xPrev, yPrev), QPointF(x, y));
            }

            xPrev = x;
            yPrev = y;
        }

        // Draw final vertical line if necessary
        if (boolDataPoints[endIndex - 1] != boolDataPoints[endIndex - 2])
        {
            painter.drawLine(QPointF(xPrev, yPrev), QPointF(xPrev, boolDataPoints[endIndex - 1] ? margin : height - margin));
        }
    }
}

void MutiTypesChrono::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int width = this->width();
    qDebug() << "width :" << width;
    updateSliderRange(); // Update the slider range when resized
    update(); // Repaint the widget
}
