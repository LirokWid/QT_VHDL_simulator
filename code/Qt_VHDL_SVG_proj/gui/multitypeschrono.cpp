#include "multitypeschrono.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QPainter>
#include <QPushButton>

//@todo Add y axis steps if needed
//add 0 line if needed
// correct the popup value visibility

template <typename T>
QVector<double> convertToDoubleVector(const QVector<T>& inputVector)
{
    QVector<double> doubleVector;
    doubleVector.reserve(inputVector.size()); // Reserve space to avoid multiple allocations

    for (const T& value : inputVector)
    {
        if constexpr (std::is_arithmetic_v<T>)
        {
            doubleVector.append(static_cast<double>(value));
        }
        else
        {
            qDebug() << "Type cannot be converted to double.";
        }
    }

    return doubleVector;
}


MultiTypesChrono::MultiTypesChrono(QVector<bool> startList, QWidget *parent)
    : QWidget(parent)
{
    initType = BOOL;

    dataPoints = convertToDoubleVector(startList);

    getMinMaxSize();
    initGraph();
}

MultiTypesChrono::MultiTypesChrono(QVector<int> startList, QWidget *parent)
    : QWidget(parent)
{
    initType = INT;

    dataPoints = convertToDoubleVector(startList);

    getMinMaxSize();
    initGraph();
}

MultiTypesChrono::MultiTypesChrono(QVector<float> startList, QWidget *parent)
    : QWidget(parent)
{
    initType = FLOAT;

    dataPoints = convertToDoubleVector(startList);

    getMinMaxSize();
    initGraph();
}

MultiTypesChrono::MultiTypesChrono(QVector<double> startList, QWidget *parent)
    : QWidget(parent)
{
    initType = DOUBLE;

    dataPoints = startList;

    getMinMaxSize();
    initGraph();
}

void MultiTypesChrono::initGraph()
{
    setMinimumSize(800, 200);
    setAutoFillBackground(true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, backgrounColor);
    this->setPalette(palette);

    // Sliders to move the graph
    slider_X = new QSlider(Qt::Horizontal, this);
    slider_X->setRange(0, 100);
    connect(slider_X, &QSlider::valueChanged, this, [=](int value)
    {
        offset_X = value;
        update();
    });

    slider_Y = new QSlider(Qt::Vertical, this);
    slider_Y->setRange(0, 100);
    connect(slider_Y, &QSlider::valueChanged, this, [=](int value)
    {
        offset_Y = value;
        update();
    });

    // Widen and shrink graph area buttons
    // X axis
    plusButton_X = new QPushButton("+", this);
    connect(plusButton_X, &QPushButton::clicked, this, &MultiTypesChrono::handlePlusButton_X);
    plusButton_X->setFixedSize(25, 30);

    minusButton_X = new QPushButton("-", this);
    connect(minusButton_X, &QPushButton::clicked, this, &MultiTypesChrono::handleMinusButton_X);
    minusButton_X->setFixedSize(25, 30);

    fitButton_X = new QPushButton("fit", this);
    connect(fitButton_X, &QPushButton::clicked, this, &MultiTypesChrono::handleFitButton_X);
    fitButton_X->setFixedSize(25, 30);

    buttonLayout_X = new QHBoxLayout();
    buttonLayout_X->addWidget(plusButton_X);
    buttonLayout_X->addWidget(minusButton_X);
    buttonLayout_X->addWidget(fitButton_X);
    buttonLayout_X->addStretch(1);

    //Y axis
    plusButton_Y = new QPushButton("+", this);
    connect(plusButton_Y, &QPushButton::clicked, this, &MultiTypesChrono::handlePlusButton_X);
    plusButton_Y->setFixedSize(30, 25);

    minusButton_Y = new QPushButton("-", this);
    connect(minusButton_Y, &QPushButton::clicked, this, &MultiTypesChrono::handleMinusButton_X);
    minusButton_Y->setFixedSize(30, 25);

    fitButton_Y = new QPushButton("fit", this);
    connect(fitButton_Y, &QPushButton::clicked, this, &MultiTypesChrono::handleFitButton_X);
    fitButton_Y->setFixedSize(30, 25);

    minusButton_Y->setStyleSheet(buttonStyle);

    buttonLayout_Y = new QVBoxLayout();
    buttonLayout_Y->addWidget(plusButton_Y);
    buttonLayout_Y->addWidget(minusButton_Y);
    buttonLayout_Y->addWidget(fitButton_Y);
    buttonLayout_Y->addStretch(1);


    // Popup displaying point information
    popupLabel = new QLabel(this);
    popupLabel->setStyleSheet("QLabel { background-color: rgba(255, 255, 255, 180); border: 2px solid black; padding: 3px; border-radius: 3px; }");
    popupLabel->setVisible(false);

    Vlayout = new QVBoxLayout(this);
    Vlayout->setContentsMargins(0, 0, 0, 0);
    Vlayout->addLayout(buttonLayout_X);
    Vlayout->addLayout(buttonLayout_Y);
    Vlayout->addWidget(slider_Y);
    Vlayout->addStretch(1);
    Vlayout->addWidget(slider_X);

    this->setMouseTracking(true);

    updateSliderRanges();
}

void MultiTypesChrono::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    height = QWidget::height();
    width = QWidget::width();

    QPoint topLeft      (marginLeft, marginTop);
    QPoint topRight     (width, marginTop);
    QPoint bottomLeft   (marginLeft, height - marginBottom);
    QPoint bottomRight  (width, height - marginBottom);

    painter.setBrush(axisColor);
    painter.setPen(axisColor);

    /*      Draw axes       */
    drawYaxis(topLeft, bottomLeft, &painter);
    drawXaxis(topLeft, bottomLeft, &painter);


    /*      Draw data points stats       */
    // Draw current number of data points
    QString statsStr = "Type : "+ getTypeString(initType) +" | Data Points: " + QString::number(nbPoints);
    drawTextInBox(
        painter,
        statsStr,
        width/2, textHeight);
    //////////////////////////////////////

    /*      Draw data line         */
    painter.setPen(graphColor);

    if (initType == BOOL)
    {
        drawBoolData(&painter);
    }
    else
    {
        drawData(&painter);
    }


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

void MultiTypesChrono::drawBoolData(QPainter *painter)
{
    if (nbPoints != 0)
    {
        int startIndex = offset_X;
        int endIndex = qMin(startIndex + visibleRange_X + 1, nbPoints);

        // Draw first point
        double xPrev = marginLeft;
        double yPrev = dataPoints[startIndex] ? marginTop : height - marginBottom;
        painter->drawEllipse(QPointF(xPrev, yPrev), pointRadius, pointRadius);

        for (int i = startIndex + 1; i < endIndex; ++i)
        {
            double x = marginLeft + (i - startIndex) * stepPixelNb_X;
            double y = dataPoints[i] ? marginTop : height - marginBottom;
            painter->drawEllipse(QPointF(x, y), pointRadius, pointRadius);

            // If boolean value changes
            if (dataPoints[i] != dataPoints[i - 1])
            {// Draw horizontal then vertical line
                painter->drawLine(QPointF(xPrev, yPrev), QPointF(x, yPrev));
                painter->drawLine(QPointF(x, yPrev), QPointF(x, y));
            }
            else
            {// Draw horizontal line
                painter->drawLine(QPointF(xPrev, yPrev), QPointF(x, y));
            }
            xPrev = x;
            yPrev = y;
        }
        if (endIndex < nbPoints)
        {
            //Draw horizontal line to show there is more data right
            painter->drawLine(QPointF(xPrev, yPrev), QPointF(xPrev + stepPixelNb_X, yPrev));
        }
    }
}

void MultiTypesChrono::drawData(QPainter *painter)
{
    if (nbPoints != 0)
    {
        int startIndex = offset_X;
        int endIndex = qMin(startIndex + visibleRange_X + 1, nbPoints);

        // Draw first point
        double xPrev = marginLeft;
        double yPrev = calculatePointHeight(dataPoints[startIndex]);

        //double yPrev = dataPoints[startIndex] ? marginTop : height - marginBottom;
        painter->drawEllipse(QPointF(xPrev, yPrev), pointRadius, pointRadius);

        for (int i = startIndex + 1; i < endIndex; ++i)
        {
            double x = marginLeft + (i - startIndex) * stepPixelNb_X;
            double y =calculatePointHeight(dataPoints[i]);

            //Draw point
            painter->drawEllipse(QPointF(x, y), pointRadius, pointRadius);

            // Draw line connecting with the last point
            painter->drawLine(QPointF(xPrev, yPrev), QPointF(x, y));

            xPrev = x;
            yPrev = y;
        }
        if (endIndex < nbPoints)
        {
            //Draw horizontal line to show there is more data right
            painter->drawLine(QPointF(xPrev, yPrev), QPointF(xPrev + stepPixelNb_X, yPrev));
        }
    }
}

double MultiTypesChrono::calculatePointHeight(double point)
{
    return (point - dataMin) * (marginTop - (height - marginBottom)) / (dataMax - dataMin) + (height - marginBottom);
}

int MultiTypesChrono::getZeroPxHeight()
{
    int zeroOffset = qAbs(dataMin);
    return zeroOffset * stepPixelNb_Y + marginBottom;
}

void MultiTypesChrono::getYcurrentMinMax()
{
    Y_dataRange = (dataMin + visibleRange_Y + offset_Y) - (dataMin + offset_Y);
    yAxisCurrentMin = dataMin + offset_Y;
    yAxisCurrentMax = yAxisCurrentMin + offset_Y;
}

void MultiTypesChrono::drawXaxis(const QPoint leftPoint, const QPoint rightPoint, QPainter *painter)
{
    int zeroHeight;
    getYcurrentMinMax();
    if (yAxisCurrentMax <= 0 && yAxisCurrentMin >= 0)
    {// 0 in range, display the line
        zeroHeight = getZeroPxHeight() + marginTop;
        painter->drawLine(QPoint(marginLeft,zeroHeight), QPoint(width, zeroHeight));
    }
    else
    {// Trace value to the bottom, no axis displayed
        zeroHeight = leftPoint.y();
    }

    // Draw ticks and labels for x-axis
    int labelInterval = (stepPixelNb_X < textWidth + 2 * xLabelDensity) ? (textWidth + 2 * xLabelDensity) / stepPixelNb_X + 1 : 1;

    for (int i = 0; i <= visibleRange_X; ++i)
    {
        int x = marginLeft + static_cast<int>(i * stepPixelNb_X);

        //Draw tick
        painter->drawLine(x, zeroHeight - marginBottom - tickSize/2,
                         x, zeroHeight - marginBottom + tickSize);

        // Display x-axis number labels with density depending on the numbers to display
        if (i % labelInterval == 0)
        {
            drawText(
                *painter,
                QString::number(offset_X + i),
                x, height - marginBottom + tickSize + textOffset);
        }
    }
}

void MultiTypesChrono::drawYaxis(const QPoint topPoint, const QPoint bottomPoint, QPainter *painter)
{
    int yRange = dataMax - dataMin;

    // Draw axis
    painter->drawLine(topPoint, bottomPoint);
    // y-axis max value label
    drawText(*painter, QString::number(dataMax), marginLeft - labelOffset, marginTop);
    // y-axis min value label
    drawText(*painter, QString::number(dataMin), marginLeft - labelOffset, height - marginBottom);
    // Draw ticks
    double step = static_cast<double>((bottomPoint.y() - topPoint.y())) / static_cast<double>(yRange);
    int labelInterval = (step < textWidth + 2 * yLabelDensity) ? (textWidth + 2 * yLabelDensity) / step + 1 : 1;

    for (int i = 0; i < yRange; ++i)
    {
        int y = bottomPoint.y() - static_cast<int>(i * step);

        //Draw tick
        painter->drawLine(
            bottomPoint.x() - tickSize,     y,
            bottomPoint.x() + tickSize/2,   y
            );

        // Draw y-axis number labels with density depending on the numbers to display
        if (i % labelInterval == 0)
        {
            drawText(
                *painter,
                QString::number(dataMin + i),
                bottomPoint.x() - tickSize - textOffset, y
            );
        }
    }

}


void MultiTypesChrono::addPoint(double point)
{
    dataPoints.append(point);
    updateSliderRanges();
    update();
}


void MultiTypesChrono::initializeBoolDataPoints(int nbPoints)
{
    for (int i = 0; i < nbPoints; ++i)
    {
        dataPoints.append(rand() % 2); // Random boolean values
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
    this->visibleRange_X = (QWidget::width() - marginLeft) / stepPixelNb_X;
    qDebug() << "Range changed to: " << this->visibleRange_X;
}

void MultiTypesChrono::calculatePixelPerStep()
{
    stepPixelNb_X = static_cast<double>((QWidget::width() - marginLeft)) / static_cast<double>(visibleRange_X);
    stepPixelNb_Y = static_cast<double>((QWidget::height() - marginTop - marginBottom)) / static_cast<double>(visibleRange_Y);
}

void MultiTypesChrono::updateSliderRanges()
{
    calculatePixelPerStep();

    // X-axis slider
    if (dataPoints.size() > visibleRange_X)
    {
        slider_X->setVisible(true);
        slider_X->setRange(0, dataPoints.size() - visibleRange_X);
        slider_X->setValue(offset_X);
    }
    else
    {
        slider_X->setVisible(false);
        slider_X->setRange(0, 0);
    }

    // Y-axis slider
    if (dataPoints.size() > visibleRange_Y)
    {// TODO Change this ! data point won't work
        slider_Y->setVisible(true);
        slider_Y->setRange(0, dataPoints.size() - visibleRange_Y);
        slider_Y->setValue(offset_Y);
    }
    else
    {
        slider_Y->setVisible(false);
        slider_Y->setRange(0, 0);
    }
}

void MultiTypesChrono::handlePlusButton_X()
{
    if (visibleRange_X < dataPoints.size() - 1)
    {
        visibleRange_X++;
        calculatePixelPerStep();
        updateSliderRanges();
        update();
    }
}

void MultiTypesChrono::handleMinusButton_X()
{
    if (visibleRange_X > minDisplayedSteps)
    {
        visibleRange_X--;
        calculatePixelPerStep();
        updateSliderRanges();
        update();
    }
}

void MultiTypesChrono::handleFitButton_X()
{
    if (dataPoints.size() > 2)
    {
        visibleRange_X = dataPoints.size();
        calculatePixelPerStep();
        updateSliderRanges();
        update();
    }
}

void MultiTypesChrono::handlePlusButton_Y()
{// TODO: UPDATE Y AXIS SIZE LIMIT
    if (visibleRange_Y < dataPoints.size() - 1)
    {
        visibleRange_Y++;
        calculatePixelPerStep();
        updateSliderRanges();
        update();
    }
}

void MultiTypesChrono::handleMinusButton_Y()
{
    if (visibleRange_Y > minDisplayedSteps)
    {
        visibleRange_Y--;
        calculatePixelPerStep();
        updateSliderRanges();
        update();
    }
}

void MultiTypesChrono::handleFitButton_Y()
{
    if (dataPoints.size() > 2)
    {
        visibleRange_Y = dataPoints.size();
        calculatePixelPerStep();
        updateSliderRanges();
        update();
    }
}

void MultiTypesChrono::resizeEvent(QResizeEvent *event)
{
    height = QWidget::height();
    width = QWidget::width();
    QWidget::resizeEvent(event);
    updateSliderRanges(); // Update the slider range when resized
    update(); // Repaint the widget
}

void MultiTypesChrono::showPopupAtCursor(QPoint cursorPos)
{

    int x = cursorPos.x();
    int y = cursorPos.y();

    for (int i = 0; i <= visibleRange_X-1; ++i)
    {
        int pointX = marginLeft + static_cast<int>(i * stepPixelNb_X);
        int pointY = dataPoints[offset_X + i] ? marginTop : height - marginBottom;

        if (qAbs(pointX - x) < popupDisplayRadius && qAbs(pointY - y) < popupDisplayRadius)
        {
            QString coords = QString("step: %1, val: %2")
                                 .arg(offset_X + i)
                                 .arg(dataPoints[offset_X + i]);
            popupLabel->setText(coords);
            popupLabel->move(x + 10, y + 10);
            popupLabel->setVisible(true);
            return;
        }
    }

    popupLabel->setVisible(false);
}

int MultiTypesChrono::getStepFromX(int x)
{
    int closest_step = ((x - marginLeft + stepPixelNb_X / 2) / stepPixelNb_X) + offset_X;

    if (closest_step < 0)
        closest_step = 0;
    if (closest_step > dataPoints.size())
        closest_step = dataPoints.size();

    return closest_step;
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
            savedOffset = offset_X;
        }

        int range = rightClickStartPoint.x() - event->pos().x();
        int newPosition = savedOffset + static_cast<int>(range / stepPixelNb_X);

        if (newPosition < 0)
            newPosition = 0;
        if (newPosition + visibleRange_X > dataPoints.size())
            newPosition = dataPoints.size() - visibleRange_X;

        qDebug() << "Right dragging to " << newPosition;

        offset_X = newPosition;
        updateSliderRanges();
        update();
    }

    showPopupAtCursor(event->pos());
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

        int startStepIndex = (newStartIndex / stepPixelNb_X);
        int endStepIndex   = (newEndIndex / stepPixelNb_X) + 1; //+1 to show have a bigger viewing window

        if (startStepIndex < 0)
            startStepIndex = 0;
        if (endStepIndex > dataPoints.size())
            endStepIndex = dataPoints.size();

        int stepsToDisplay = endStepIndex - startStepIndex;
        qDebug() << "Start step: " << startStepIndex << " End step: " << endStepIndex;

        if (stepsToDisplay >= minDisplayedSteps)            // Drag width is at least 2 steps
        {
            visibleRange_X = stepsToDisplay;
            offset_X = startStepIndex;
            calculatePixelPerStep();
            updateSliderRanges();
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

void MultiTypesChrono::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    popupLabel->setVisible(false);
}


double MultiTypesChrono::getMax(const QVector<double> &vec)
{
    Q_ASSERT(!vec.isEmpty());
    return *std::max_element(vec.begin(), vec.end());
}

double MultiTypesChrono::getMin(const QVector<double> &vec)
{
    Q_ASSERT(!vec.isEmpty());
    return *std::min_element(vec.begin(), vec.end());
}


void MultiTypesChrono::getMinMaxSize()
{
    if (initType == BOOL)
    {
        dataMax = 1;
        dataMin = 0;
    }
    else
    {
        dataMax = qCeil(getMax(dataPoints));
        dataMin = qFloor(getMin(dataPoints));
    }
    nbPoints = dataPoints.size();
}

QString MultiTypesChrono::getTypeString(e_initType type)
{
    switch (type)
    {
        case BOOL:
            return "BOOL";
        case INT:
            return "INT";
        case FLOAT:
            return "FLOAT";
        case DOUBLE:
            return "DOUBLE";
        case ERROR:
        default:
            return "UNKNOWN";
    }
}
