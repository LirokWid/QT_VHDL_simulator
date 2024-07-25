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

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    connect(slider, &QSlider::valueChanged, this, [=](int value)
            {
                currentOffset = value;
                qDebug() << "Slider value changed to " << value;
                update();
            });

    // Widen and shrink graph area buttons
    plusButton = new QPushButton("+", this);
    connect(plusButton, &QPushButton::clicked, this, &MultiTypesChrono::handlePlusButton);
    plusButton->setFixedSize(25, 30);

    minusButton = new QPushButton("-", this);
    connect(minusButton, &QPushButton::clicked, this, &MultiTypesChrono::handleMinusButton);
    minusButton->setFixedSize(25, 30);

    fitButton = new QPushButton("fit", this);
    connect(fitButton, &QPushButton::clicked, this, &MultiTypesChrono::handleFitButton);
    fitButton->setFixedSize(25, 30);

    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(plusButton);
    buttonLayout->addWidget(minusButton);
    buttonLayout->addWidget(fitButton);
    buttonLayout->addStretch(1);

    // Popup displaying point information
    popupLabel = new QLabel(this);
    popupLabel->setStyleSheet("QLabel { background-color: rgba(255, 255, 255, 180); border: 2px solid black; padding: 3px; border-radius: 3px; }");
    popupLabel->setVisible(false);

    Vlayout = new QVBoxLayout(this);
    Vlayout->setContentsMargins(30, 0, 30, 0);  // TODO parametrize the margins
    Vlayout->addLayout(buttonLayout);
    Vlayout->addStretch(1);
    Vlayout->addWidget(slider);

    this->setMouseTracking(true);

    updateSliderRange();
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
    // Draw y-axis
    painter.drawLine(topLeft, bottomLeft);


    // y-axis max value label
    drawText(painter, QString::number(dataMax), marginLeft - labelOffset, marginTop);
    // y-axis min value label
    drawText(painter, QString::number(dataMin), marginLeft - labelOffset, height - marginBottom);


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
        int startIndex = currentOffset;
        int endIndex = qMin(startIndex + visibleRange + 1, nbPoints);

        // Draw first point
        double xPrev = marginLeft;
        double yPrev = dataPoints[startIndex] ? marginTop : height - marginBottom;
        painter->drawEllipse(QPointF(xPrev, yPrev), pointRadius, pointRadius);

        for (int i = startIndex + 1; i < endIndex; ++i)
        {
            double x = marginLeft + (i - startIndex) * stepPixelNb;
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
            painter->drawLine(QPointF(xPrev, yPrev), QPointF(xPrev + stepPixelNb, yPrev));
        }
    }
}

void MultiTypesChrono::drawData(QPainter *painter)
{
    if (nbPoints != 0)
    {
        int startIndex = currentOffset;
        int endIndex = qMin(startIndex + visibleRange + 1, nbPoints);

        // Draw first point
        double xPrev = marginLeft;
        double yPrev = calculatePointHeight(dataPoints[startIndex]);

        //double yPrev = dataPoints[startIndex] ? marginTop : height - marginBottom;
        painter->drawEllipse(QPointF(xPrev, yPrev), pointRadius, pointRadius);

        for (int i = startIndex + 1; i < endIndex; ++i)
        {
            double x = marginLeft + (i - startIndex) * stepPixelNb;
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
            painter->drawLine(QPointF(xPrev, yPrev), QPointF(xPrev + stepPixelNb, yPrev));
        }
    }
}

double MultiTypesChrono::calculatePointHeight(double point)
{
    return (point - dataMin) * (marginTop - (height - marginBottom)) / (dataMax - dataMin) + (height - marginBottom);
}


void MultiTypesChrono::addPoint(double point)
{
    dataPoints.append(point);
    updateSliderRange();
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
    this->visibleRange = (QWidget::width() - marginLeft) / stepPixelNb;

    qDebug() << "Range changed to: " << this->visibleRange;
}

void MultiTypesChrono::calculateStepPixelNb()
{
    stepPixelNb = static_cast<double>((QWidget::width() - marginLeft)) / static_cast<double>(visibleRange);
}

void MultiTypesChrono::updateSliderRange()
{
    calculateStepPixelNb();
    if (dataPoints.size() > visibleRange)
    {
        slider->setVisible(true);
        slider->setRange(0, dataPoints.size() - visibleRange);
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
    if (visibleRange < dataPoints.size() - 1)
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
    if (dataPoints.size() > 2)
    {
        visibleRange = dataPoints.size();
        calculateStepPixelNb();
        updateSliderRange();
        update();
    }
}

void MultiTypesChrono::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateSliderRange(); // Update the slider range when resized
    update(); // Repaint the widget
}

void MultiTypesChrono::showPopupAtCursor(QPoint cursorPos)
{
    int height = QWidget::height();
    int width = QWidget::width();
    int x = cursorPos.x();
    int y = cursorPos.y();

    for (int i = 0; i <= visibleRange-1; ++i)
    {
        int pointX = marginLeft + static_cast<int>(i * stepPixelNb);
        int pointY = dataPoints[currentOffset + i] ? marginTop : height - marginBottom;

        if (qAbs(pointX - x) < popupDisplayRadius && qAbs(pointY - y) < popupDisplayRadius)
        {
            QString coords = QString("step: %1, val: %2")
                                 .arg(currentOffset + i)
                                 .arg(dataPoints[currentOffset + i]);
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
    int closest_step = ((x - marginLeft + stepPixelNb / 2) / stepPixelNb) + currentOffset;

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
            savedOffset = currentOffset;
        }

        int range = rightClickStartPoint.x() - event->pos().x();
        int newPosition = savedOffset + static_cast<int>(range / stepPixelNb);

        if (newPosition < 0)
            newPosition = 0;
        if (newPosition + visibleRange > dataPoints.size())
            newPosition = dataPoints.size() - visibleRange;

        qDebug() << "Right dragging to " << newPosition;

        currentOffset = newPosition;
        updateSliderRange();
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

        int startStepIndex = (newStartIndex / stepPixelNb);
        int endStepIndex   = (newEndIndex / stepPixelNb) + 1; //+1 to show have a bigger viewing window

        if (startStepIndex < 0)
            startStepIndex = 0;
        if (endStepIndex > dataPoints.size())
            endStepIndex = dataPoints.size();

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
        dataMax = getMax(dataPoints);
        dataMin = getMin(dataPoints);
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
