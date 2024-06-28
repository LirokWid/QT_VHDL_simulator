#include "multitypeschrono.h"
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
ok - ajouter bandeau stats sur valeurs
ajouter boutons x axis
ok -parametriser les marges et couleurs
 *
 *
 *
 */


MultiTypesChrono::MultiTypesChrono(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800, 200);
    setAutoFillBackground(true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, backgrounColor);
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

    connect(plusButton  , &QPushButton::clicked, this, &MultiTypesChrono::handlePlusButton);
    connect(minusButton , &QPushButton::clicked, this, &MultiTypesChrono::handleMinusButton);
    connect(fitButton   , &QPushButton::clicked, this, &MultiTypesChrono::handleFitButton);

    Vlayout = new QVBoxLayout(this);
    Vlayout->setContentsMargins(30, 0, 30, 0);  // TODO parametrize the margins
    Vlayout->addLayout(buttonLayout);
    Vlayout->addStretch(1);
    Vlayout->addWidget(slider);

    initializeBoolDataPoints(50); // Temp debug to dislay random values
    updateSliderRange();
}

void MultiTypesChrono::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int height = QWidget::height();
    int width = QWidget::width();

    QPoint topLeft      (marginLeft, marginTop);
    QPoint topRight     (width, marginTop);
    QPoint bottomLeft   (marginLeft, height - marginBottom);
    QPoint bottomRight  (width, height - marginBottom);

    painter.setBrush(axisColor);
    painter.setPen(axisColor);

    /*      Draw axes       */
    // Draw y-axis
    painter.drawLine(topLeft, bottomLeft);      // y-axis line
    // y-axis max value label
    drawText(painter, "1", marginLeft - labelOffset, marginTop);
    // y-axis min value label
    drawText(painter, "0", marginLeft - labelOffset, height - marginBottom);

    // Draw x-axis
    painter.drawLine(bottomLeft, bottomRight);
    // Draw ticks and labels for x-axis
    int labelInterval = (stepPixelNb < textWidth + 2 * xLabelDensity) ? (textWidth + 2 * xLabelDensity) / stepPixelNb + 1 : 1;

    for (int i = 0; i <= visibleRange; ++i)
    {
        int x = marginLeft + static_cast<int>(i * stepPixelNb);

        //Draw tick
        painter.drawLine(x, height - marginBottom - tickSize/2,
                         x, height - marginBottom + tickSize);

        // Display x-axis number labels with density depending on the numbers to display
        if (i % labelInterval == 0)
        {
            drawText(
                painter,
                QString::number(currentOffset + i),
                x, height - marginBottom + tickSize + textOffset);
        }
    }
    //////////////////////////

    /*      Draw data points stats       */
    // Draw current number of data points
    QString statsStr = "Data Points: " + QString::number(boolDataPoints.size());
    drawTextInBox(
        painter,
        statsStr,
        width/2, textHeight);
    //////////////////////////////////////

    /*      Draw graph line         */
    painter.setPen(graphColor);
    if (!boolDataPoints.isEmpty())
    {
        int startIndex = currentOffset;
        int endIndex = qMin(startIndex + visibleRange + 1, boolDataPoints.size());

        // Draw first point
        double xPrev = marginLeft;
        double yPrev = boolDataPoints[startIndex] ? marginTop : height - marginBottom;
        painter.drawEllipse(QPointF(xPrev, yPrev), pointRadius, pointRadius);

        for (int i = startIndex + 1; i < endIndex; ++i)
        {
            double x = marginLeft + (i - startIndex) * stepPixelNb;

            double y = boolDataPoints[i] ? marginTop : height - marginBottom;
            painter.drawEllipse(QPointF(x, y), pointRadius, pointRadius);

            // If boolean value changes
            if (boolDataPoints[i] != boolDataPoints[i - 1])
            {// Draw horizontal then vertical line
                painter.drawLine(QPointF(xPrev, yPrev), QPointF(x, yPrev));
                painter.drawLine(QPointF(x, yPrev), QPointF(x, y));
            }
            else
            {// Draw horizontal line
                painter.drawLine(QPointF(xPrev, yPrev), QPointF(x, y));
            }
            xPrev = x;
            yPrev = y;
        }
        if (endIndex < boolDataPoints.size())
        {
            //Draw horizontal line to show there is more data right
            painter.drawLine(QPointF(xPrev, yPrev), QPointF(xPrev + stepPixelNb, yPrev));
        }
    }
    //////////////////////////////////

    // Draw the zoom rectangle if zoom-dragging
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

void MultiTypesChrono::addPoint(bool point)
{
    boolDataPoints.append(point);
    updateSliderRange();
    update();
}

void MultiTypesChrono::initializeBoolDataPoints(int nbPoints)
{
    for (int i = 0; i < nbPoints; ++i)
    {
        boolDataPoints.append(rand() % 2); // Random boolean values
    }
}

void MultiTypesChrono::drawText(QPainter& painter, const QString& text, int x, int y)
{
    QRect boundingRect = painter.boundingRect(QRect(), Qt::AlignCenter, text);
    boundingRect.moveTo(
        x - boundingRect.width() / 2,
        y - boundingRect.height() / 2); //center the rectangle
    painter.drawText(boundingRect, Qt::AlignCenter, text);
}

void MultiTypesChrono::drawTextInBox(QPainter& painter, const QString& text, int x, int y)
{    
    // Save the current pen
    QPen oldPen = painter.pen();

    // Calculate the bounding rectangle for the text
    QRect textRect = painter.boundingRect(QRect(), Qt::AlignCenter, text);

    // Add textRectPadding to the bounding rectangle
    QRect paddedRect = textRect.adjusted(-textRectPadding, -textRectPadding, textRectPadding, textRectPadding);
    paddedRect.moveTo(
        x - paddedRect.width() / 2,
        y - paddedRect.height() / 2); // Center the rectangle

    // Set the new pen color for the border
    QPen newPen(rectBorderColor);
    painter.setPen(newPen);

    // Draw the rounded rectangle with a transparent brush
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(paddedRect, textRectRadius, textRectRadius);

    // Draw the text inside the rounded rectangle
    painter.drawText(paddedRect, Qt::AlignCenter, text);

    // Restore the old pen
    painter.setPen(oldPen);
}

void MultiTypesChrono::calculateVisibleRange()
{
    this->visibleRange = (width() - marginLeft) / stepPixelNb;

    qDebug() << "Range changed to: " << this->visibleRange;
}

void MultiTypesChrono::calculateStepPixelNb()
{
    stepPixelNb = static_cast<double>((width() - marginLeft)) / static_cast<double>(visibleRange);
}

void MultiTypesChrono::updateSliderRange()
{
    calculateStepPixelNb();
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

void MultiTypesChrono::handlePlusButton()
{
    if (visibleRange < boolDataPoints.size() - 1)
    {
        visibleRange++;
        calculateStepPixelNb();
        updateSliderRange();
        update();
    }
}

void MultiTypesChrono::handleMinusButton()
{
    if (visibleRange > minDisplayedSteps)
    {
        visibleRange--;
        calculateStepPixelNb();
        updateSliderRange();
        update();
    }
}

void MultiTypesChrono::handleFitButton()
{
    if (boolDataPoints.size() > 2)
    {
        visibleRange = boolDataPoints.size();
        calculateStepPixelNb();
        updateSliderRange();
        update();
    }
}

void MultiTypesChrono::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int width = this->width();
    qDebug() << "width :" << width;
    updateSliderRange(); // Update the slider range when resized
    update(); // Repaint the widget
}

void MultiTypesChrono::mousePressEvent(QMouseEvent *event)
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

    /*          DEBUG                       */
    if(event->button() == Qt::MiddleButton)
    {
        qDebug() << "x: " << event->pos().x() << "y: " << event->pos().y();
    }
    //////////////////////////////////////////
}

void MultiTypesChrono::mouseMoveEvent(QMouseEvent *event)
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

void MultiTypesChrono::mouseReleaseEvent(QMouseEvent *event)
{// TODO update to handle integer values for y axis
    if (event->button() == Qt::LeftButton && isDragging)
    {
        isDragging = false;
        dragEndPoint = event->pos();

        int startIndex = qMin(dragStartPoint.x(), dragEndPoint.x());
        int endIndex = qMax(dragStartPoint.x(), dragEndPoint.x());

        // Calculate the new visible range based on the zoom area
        int newStartIndex = startIndex - marginLeft;
        int newEndIndex = endIndex - marginLeft;

        qDebug() << "Drag start: " << newStartIndex << " End: " << newEndIndex;

        int startStepIndex = (newStartIndex / stepPixelNb);
        int endStepIndex   = (newEndIndex / stepPixelNb) + 1; //+1 to show have a bigger viewing window

        if (startStepIndex < 0)
            startStepIndex = 0;
        if (endStepIndex > boolDataPoints.size())
            endStepIndex = boolDataPoints.size();

        int stepsToDisplay = endStepIndex - startStepIndex;
        qDebug() << "Start step: " << startStepIndex << " End step: " << endStepIndex;

        if (stepsToDisplay >= minDisplayedSteps)            // Drag width is at least 2 steps
        {
            visibleRange = stepsToDisplay;
            currentOffset = startStepIndex;
            calculateStepPixelNb();
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
