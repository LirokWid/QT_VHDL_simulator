#include "mutitypeschrono.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QPainter>

MutiTypesChrono::MutiTypesChrono(QWidget *parent)
    :QWidget(parent)
{
    slider = new QSlider(Qt::Horizontal, this);

    setMinimumSize(800, 200);
    setAutoFillBackground(true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(20, 20, 20));
    this->setPalette(palette);
    slider->setRange(0,100);

    Vlayout = new QVBoxLayout(this);

    Vlayout->addStretch(1);
    Vlayout->addWidget(slider);

    // Draw the time scale and the value axis


    connect(slider, &QSlider::valueChanged, this, [=](int value)
    {
        //TODO : update the offset and handle scrolling to left and right
        currentOffset = value;
        qDebug() << "Slider value changed to " << value;
        update();
    });
    // Populate data points
    for (int i = 0; i < 50; ++i)
    {
        dataPoints.append(QPointF(i, rand() % 100));
    }

    updateSliderRange();
}

void MutiTypesChrono::updateSliderRange()
{
    if (dataPoints.size() > visibleRange)
    {
        slider->setRange(0, dataPoints.size() - visibleRange);
    }
    else
    {
        slider->setRange(0, 0);
    }
}

/*
void MutiTypesChrono::drawBackScale(QPainter *painter)
{
    // Draw the time scale
    painter->setPen(QColor(255, 255, 255));
    painter->drawLine(0, height() - 20, width, height() - 20);
    for (int i = 0; i <= 10; ++i)
    {
        int xPos = i * width / 10;
        painter->drawLine(xPos, height() - 20, xPos, height() - 15);
        painter->drawText(xPos, height() - 10, QString::number(i));
    }
}
*/

void MutiTypesChrono::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int height = QWidget::height();
    int width = QWidget::width();


    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);

    // Draw axes
    int margin = 30;
    int graphHeight = height - 2 * margin;

    // Draw y-axis
    painter.drawLine(margin, margin, margin, height - margin);
    painter.drawText(margin - 30, margin, 30, 10, Qt::AlignRight, "max");
    painter.drawText(margin - 30, height - margin, 30, 10, Qt::AlignRight, "min");

    // Draw x-axis
    painter.drawLine(margin, height - margin, width - margin, height - margin);

    painter.setBrush(Qt::green);
    painter.setPen(Qt::green);

    // Draw data points
    if (!dataPoints.isEmpty()) {
        int startIndex = currentOffset;
        int endIndex = qMin(startIndex + visibleRange, dataPoints.size());

        double xScale = static_cast<double>(fixedGraphWidth) / visibleRange;
        double yScale = static_cast<double>(graphHeight) / 100;

        for (int i = startIndex; i < endIndex; ++i) {
            double x = margin + (i - startIndex) * xScale;
            double y = height - margin - dataPoints[i].y() * yScale;
            painter.drawEllipse(QPointF(x, y), 3, 3);

            if (i > startIndex) {
                double prevX = margin + (i - startIndex - 1) * xScale;
                double prevY = height - margin - dataPoints[i - 1].y() * yScale;
                painter.drawLine(QPointF(prevX, prevY), QPointF(x, y));
            }
        }
    }
}
