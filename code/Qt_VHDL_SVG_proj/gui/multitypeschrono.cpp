#include "multitypeschrono.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QPainter>
#include <QPushButton>

template <typename T>
MultiTypesChrono<T>::MultiTypesChrono(QVector<T> startList, QWidget *parent)
    : QWidget(parent)
{
    if (typeid(T) == typeid(bool))
    {
        initType = BOOL;
    }
    else if (typeid(T) == typeid(int))
    {
        initType = INT;
    }
    else if (typeid(T) == typeid(float))
    {
        initType = FLOAT;
    }
    else if (typeid(T) == typeid(double))
    {
        initType = DOUBLE;
    }
    else
    {
        initType = ERROR;
    }

    Q_ASSERT(initType != ERROR);

    dataPoints = startList;

    getMinMaxSize();
    initGraph();
}

template <typename T>
void MultiTypesChrono<T>::initGraph()
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
    connect(plusButton, &QPushButton::clicked, this, &MultiTypesChrono<T>::handlePlusButton);
    plusButton->setFixedSize(25, 30);

    minusButton = new QPushButton("-", this);
    connect(minusButton, &QPushButton::clicked, this, &MultiTypesChrono<T>::handleMinusButton);
    minusButton->setFixedSize(25, 30);

    fitButton = new QPushButton("fit", this);
    connect(fitButton, &QPushButton::clicked, this, &MultiTypesChrono<T>::handleFitButton);
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

template <typename T>
void MultiTypesChrono<T>::paintEvent(QPaintEvent* event)
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
    QString statsStr = "Data Points: " + QString::number(nbPoints);
    drawTextInBox(
        painter,
        statsStr,
        width/2, textHeight);
    //////////////////////////////////////

    /*      Draw data line         */
    painter.setPen(graphColor);

    if (initType == BOOL)
    {
        drawBoolData(painter);
    }
    else
    {
        drawData(painter);
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

template<typename T>
void MultiTypesChrono<T>::drawBoolData(QPainter *painter)
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

template<typename T>
void MultiTypesChrono<T>::drawData(QPainter *painter)
{
    if (nbPoints != 0)
    {
        int startIndex = currentOffset;
        int endIndex = qMin(startIndex + visibleRange + 1, nbPoints);

        // Draw first point
        double xPrev = marginLeft;

        double yTop = marginTop;
        double yBottom = height - marginBottom;

        double yPrev = (dataPoints[startIndex] - dataMin) * (yTop - yBottom) / (dataMax - dataMin) + yBottom;

        //double yPrev = dataPoints[startIndex] ? marginTop : height - marginBottom;
        painter->drawEllipse(QPointF(xPrev, yPrev), pointRadius, pointRadius);

        for (int i = startIndex + 1; i < endIndex; ++i)
        {
            double x = marginLeft + (i - startIndex) * stepPixelNb;
            double y = dataPoints[i] ? marginTop : height - marginBottom;
            painter->drawEllipse(QPointF(x, y), pointRadius, pointRadius);

            // If value changes
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

template <typename T>
double MultiTypesChrono<T>::calculatePointHeight(T point)
{
    return (point - dataMin) * (marginTop - marginBottom) / (dataMax - dataMin) + marginBottom;
}


template <typename T>
void MultiTypesChrono<T>::addPoint(T point)
{
    dataPoints.append(point);
    updateSliderRange();
    update();
}


template <typename T>
void MultiTypesChrono<T>::initializeBoolDataPoints(int nbPoints)
{
    for (int i = 0; i < nbPoints; ++i)
    {
        dataPoints.append(rand() % 2); // Random boolean values
    }
}

template <typename T>
void MultiTypesChrono<T>::drawText(QPainter& painter, const QString& text, int x, int y)
{
    QRect boundingRect = painter.boundingRect(QRect(), Qt::AlignCenter, text);
    boundingRect.moveTo(
        x - boundingRect.width() / 2,
        y - boundingRect.height() / 2); //center the rectangle
    painter.drawText(boundingRect, Qt::AlignCenter, text);
}

template <typename T>
void MultiTypesChrono<T>::drawTextInBox(QPainter& painter, const QString& text, int x, int y)
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

template <typename T>
void MultiTypesChrono<T>::calculateVisibleRange()
{
    this->visibleRange = (width() - marginLeft) / stepPixelNb;

    qDebug() << "Range changed to: " << this->visibleRange;
}

template <typename T>
void MultiTypesChrono<T>::calculateStepPixelNb()
{
    stepPixelNb = static_cast<double>((width() - marginLeft)) / static_cast<double>(visibleRange);
}

template <typename T>
void MultiTypesChrono<T>::updateSliderRange()
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

template <typename T>
void MultiTypesChrono<T>::handlePlusButton()
{
    if (visibleRange < boolDataPoints.size() - 1)
    {
        visibleRange++;
        calculateStepPixelNb();
        updateSliderRange();
        update();
    }
}

template <typename T>
void MultiTypesChrono<T>::handleMinusButton()
{
    if (visibleRange > minDisplayedSteps)
    {
        visibleRange--;
        calculateStepPixelNb();
        updateSliderRange();
        update();
    }
}

template <typename T>
void MultiTypesChrono<T>::handleFitButton()
{
    if (boolDataPoints.size() > 2)
    {
        visibleRange = boolDataPoints.size();
        calculateStepPixelNb();
        updateSliderRange();
        update();
    }
}

template <typename T>
void MultiTypesChrono<T>::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int width = this->width();
    qDebug() << "width :" << width;
    updateSliderRange(); // Update the slider range when resized
    update(); // Repaint the widget
}

template <typename T>
void MultiTypesChrono<T>::showPopupAtCursor(QPoint cursorPos)
{
    int height = QWidget::height();
    int width = QWidget::width();
    int x = cursorPos.x();
    int y = cursorPos.y();

    for (int i = 0; i <= visibleRange-1; ++i)
    {
        int pointX = marginLeft + static_cast<int>(i * stepPixelNb);
        int pointY = boolDataPoints[currentOffset + i] ? marginTop : height - marginBottom;

        if (qAbs(pointX - x) < popupDisplayRadius && qAbs(pointY - y) < popupDisplayRadius)
        {
            QString coords = QString("step: %1, val: %2")
                                 .arg(currentOffset + i)
                                 .arg(boolDataPoints[currentOffset + i]);
            popupLabel->setText(coords);
            popupLabel->move(x + 10, y + 10);
            popupLabel->setVisible(true);
            return;
        }
    }

    popupLabel->setVisible(false);
}

template <typename T>
int MultiTypesChrono<T>::getStepFromX(int x)
{
    int closest_step = ((x - marginLeft + stepPixelNb / 2) / stepPixelNb) + currentOffset;

    if (closest_step < 0)
        closest_step = 0;
    if (closest_step > boolDataPoints.size())
        closest_step = boolDataPoints.size();

    return closest_step;
}

template <typename T>
void MultiTypesChrono<T>::mousePressEvent(QMouseEvent *event)
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

template <typename T>
void MultiTypesChrono<T>::mouseMoveEvent(QMouseEvent *event)
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

        qDebug() << "Right dragging to " << newPosition;

        currentOffset = newPosition;
        updateSliderRange();
        update();
    }

    showPopupAtCursor(event->pos());
}

template <typename T>
void MultiTypesChrono<T>::mouseReleaseEvent(QMouseEvent *event)
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

template <typename T>
void MultiTypesChrono<T>::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    popupLabel->setVisible(false);
}


template <typename T>
T MultiTypesChrono<T>::getMax(const QVector<T> &vec)
{
    Q_ASSERT(!vec.isEmpty());
    return *std::max_element(vec.begin(), vec.end());
}

template <typename T>
T MultiTypesChrono<T>::getMin(const QVector<T> &vec)
{
    Q_ASSERT(!vec.isEmpty());
    return *std::min_element(vec.begin(), vec.end());
}

template<typename T>
void MultiTypesChrono<T>::getMinMaxSize()
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

template<typename T>
QString MultiTypesChrono<T>::getTypeString(e_initType type)
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
