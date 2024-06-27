#include "mutitypeschrono.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QPainter>
#include <QPushButton>

/*
 *
 *                         TODO                            *
ok - fit button
ok - drag zoom clic droit
ok - décaler les valeurs de 1
affficher moins de chiffres en ordonnées si beaucoup de chiffres affichés
ajouter fenetre au survol donnant info sur le point
ajouter bandeau stats sur valeurs
ajouter boutons x axis
parametriser les marges et couleurs
Que faire si pixel step < 1 ?
le - ne fonctionne pas à tout les coups
 *
 *
 *
 */


MutiTypesChrono::MutiTypesChrono(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800, 200);
    setAutoFillBackground(true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(20, 20, 20));
    this->setPalette(palette);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    connect(slider, &QSlider::valueChanged, this, [=](int value)
            {
                currentOffset = value;
                qDebug() << "Slider value changed to " << value;
                update();
            });

    // Widen and shrink graph area buttons
    buttonLayout = new QHBoxLayout();

    minusButton = new QPushButton("-", this);
    plusButton = new QPushButton("+", this);
    fitButton = new QPushButton("fit", this);

    //Set the size of the buttons
    minusButton->setFixedSize(25, 30);
    plusButton->setFixedSize(25, 30);
    fitButton->setFixedSize(25, 30);

    buttonLayout->addWidget(minusButton);
    buttonLayout->addWidget(plusButton);
    buttonLayout->addWidget(fitButton);
    buttonLayout->addStretch(1);


    connect(plusButton  , &QPushButton::clicked, this, &MutiTypesChrono::handlePlusButton);
    connect(minusButton , &QPushButton::clicked, this, &MutiTypesChrono::handleMinusButton);
    connect(fitButton   , &QPushButton::clicked, this, &MutiTypesChrono::handleFitButton);


    Vlayout = new QVBoxLayout(this);
    Vlayout->setContentsMargins(30, 0, 30, 0);  // TODO parametrize the margins
    Vlayout->addLayout(buttonLayout);
    Vlayout->addStretch(1);
    Vlayout->addWidget(slider);

    initializeBoolDataPoints(50); // Temp debug
    updateSliderRange();
}

void MutiTypesChrono::addPoint(bool point)
{
    boolDataPoints.append(point);
    updateSliderRange();
    update();
}

void MutiTypesChrono::updateSliderRange()
{
    calculateVisibleRange();
    if (boolDataPoints.size() > visibleRange)
    {
        slider->setVisible(true);
        slider->setRange(0, boolDataPoints.size() - visibleRange);
        slider->setValue(currentOffset);
    }
    else
    {
        slider->setVisible(false);
        slider->setRange(0, 0);
    }
}

void MutiTypesChrono::calculateVisibleRange()
{
    this->visibleRange = (width() - margin) / stepPixelNb;

    qDebug() << "Range changed to: " << this->visibleRange;
}

void MutiTypesChrono::initializeBoolDataPoints(int nbPoints)
{
    for (int i = 0; i < nbPoints; ++i)
    {
        boolDataPoints.append(rand() % 2); // Random boolean values
    }
}

void MutiTypesChrono::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Define constants for margins and labels
    const int labelMargin = 10; // Assuming label_margin is a constant
    const int tickSize = 5;
    const int textOffset = 10;
    const int textWidth = 30;
    const int textHeight = 10;
    const int xLabelDensity = 1;

    int height = QWidget::height();
    int width = QWidget::width();

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);

    // Draw axes
    int graphHeight = height - margin;
    int graphWidth = width - margin;

    // Draw y-axis
    painter.drawLine(margin, margin, margin, height - margin);
    painter.drawText(margin - textWidth - labelMargin, margin - textOffset, textWidth, textHeight, Qt::AlignRight, "1"); // High state
    painter.drawText(margin - textWidth - labelMargin, height - margin - textOffset, textWidth, textHeight, Qt::AlignRight, "0"); // Low state

    // Draw x-axis
    painter.drawLine(margin, height - margin, width - margin, height - margin);

    // Draw ticks and labels for x-axis
    double xScale = static_cast<double>(graphWidth) / visibleRange;

    int labelInterval = (stepPixelNb < textWidth + 2 * xLabelDensity) ? (textWidth + 2 * xLabelDensity) / stepPixelNb + 1 : 1;

    for (int i = 0; i <= visibleRange; ++i)
    {
        int x = margin + i * xScale;
        painter.drawLine(x, height - margin, x, height - margin + tickSize);

        // Display label if within interval
        if (i % labelInterval == 0)
        {
            painter.drawText(
                x - textOffset, height - margin + textOffset,   // x, y
                textWidth, textHeight,                          // width, height
                Qt::AlignCenter,                                // alignment
                QString::number(currentOffset + i));            // value
        }
    }

    // Draw current number of data points
    painter.drawText(width - 100, margin - textOffset, 100, 20, Qt::AlignLeft, "Data Points: " + QString::number(boolDataPoints.size()));

    painter.setPen(Qt::green);

    // Draw boolean data points as a square signal with vertical lines between changes
    if (!boolDataPoints.isEmpty())
    {
        int startIndex = currentOffset;
        int endIndex = qMin(startIndex + visibleRange + 1, boolDataPoints.size());

        // Draw initial point
        double xPrev = margin;
        double yPrev = boolDataPoints[startIndex] ? margin : height - margin;
        painter.drawEllipse(QPointF(xPrev, yPrev), 3, 3);

        for (int i = startIndex + 1; i < endIndex; ++i)
        {
            double x = margin + (i - startIndex) * xScale;
            double y = boolDataPoints[i] ? margin : height - margin;
            painter.drawEllipse(QPointF(x, y), 2, 2);

            // Draw vertical line if boolean value changes
            if (boolDataPoints[i] != boolDataPoints[i - 1])
            {
                // Draw horizontal then vertical line
                painter.drawLine(QPointF(xPrev, yPrev), QPointF(x, yPrev));
                painter.drawLine(QPointF(x, yPrev), QPointF(x, y));
            }
            else
            {
                // Draw horizontal line
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

    // Draw the zoom rectangle if dragging
    if (isDragging)
    {
        QPen pen;
        pen.setColor(Qt::white);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.setBrush(QBrush(Qt::transparent));
        QRect zoomRect(dragStartPoint, dragEndPoint);
        painter.drawRect(zoomRect);
    }
}

void MutiTypesChrono::updateStepPixelNb()
{
    stepPixelNb = qRound(static_cast<float>(width() - margin)) / static_cast<float>(visibleRange);
}

void MutiTypesChrono::handlePlusButton()
{
    if (visibleRange < boolDataPoints.size() - 1)
    {
        visibleRange++;
        updateStepPixelNb();
        updateSliderRange();
        update();
    }
}

void MutiTypesChrono::handleMinusButton()
{
    if (visibleRange > 2)
    {
        visibleRange--;
        updateStepPixelNb();
        updateSliderRange();
        update();
    }
}

void MutiTypesChrono::handleFitButton()
{
    if (boolDataPoints.size() > 2)
    {
        visibleRange = boolDataPoints.size();
        updateStepPixelNb();
        updateSliderRange();
        update();
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

void MutiTypesChrono::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        dragStartPoint = event->pos();
        dragEndPoint = dragStartPoint;
        update();
    }

    if(event->button() == Qt::RightButton)
    {
        isRightClicking = true;
        rightClickStartPoint = event->pos();
    }
}

void MutiTypesChrono::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        dragEndPoint = event->pos();
        update();
    }

    if (isRightClicking)
    {
        static int savedOffset;

        if (isFirstRightClick == false)
        {
            isFirstRightClick = true;
            savedOffset = currentOffset;
        }

        int range = rightClickStartPoint.x() - event->pos().x();
        int newPosition = savedOffset + static_cast<int>(range / stepPixelNb);

        if (newPosition < 0)
            newPosition = 0;
        if (newPosition + visibleRange > boolDataPoints.size())
            newPosition = boolDataPoints.size() - visibleRange;

        qDebug() << "Right draging to " << newPosition;

        currentOffset = newPosition;
        updateSliderRange();
        update();
    }
}

void MutiTypesChrono::mouseReleaseEvent(QMouseEvent *event)
{// TODO update to handle integer values for y axis
    if (event->button() == Qt::LeftButton && isDragging)
    {
        isDragging = false;
        dragEndPoint = event->pos();

        int startIndex = qMin(dragStartPoint.x(), dragEndPoint.x());
        int endIndex = qMax(dragStartPoint.x(), dragEndPoint.x());

        int graphWidth = width() - 2 * margin;

        // Calculate the new visible range based on the zoom area
        int newStartIndex = startIndex - margin;
        int newEndIndex = endIndex - margin;

        int dragWidth = newEndIndex - newStartIndex;

        qDebug() << "Drag start: " << newStartIndex << " End: " << newEndIndex;

        int startStepIndex = (newStartIndex / stepPixelNb) + 1;
        int endStepIndex = newEndIndex / stepPixelNb;

        if (startStepIndex < 0)
            startStepIndex = 0;
        if (endStepIndex > boolDataPoints.size())
            endStepIndex = boolDataPoints.size();

        int stepsToDisplay = endStepIndex - startStepIndex;
        qDebug() << "Start step: " << startStepIndex << " End step: " << endStepIndex;

        if (stepsToDisplay >= 2)            // Drag width is at least 2 steps
        {
            visibleRange = stepsToDisplay;
            currentOffset = startStepIndex;
            stepPixelNb = (width() - 2 * margin) / visibleRange;
            updateSliderRange();
            update();
        }
        else
        {
            update();
        }
    }

    if (event->button() == Qt::RightButton && isRightClicking)
    {
        isRightClicking = false;
        isFirstRightClick = false;
    }
}
