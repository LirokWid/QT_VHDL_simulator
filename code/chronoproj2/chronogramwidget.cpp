#include "chronogramwidget.h"
#include <QPainter>
#include <QSlider>

ChronogramWidget::ChronogramWidget(QWidget *parent)
    : QWidget(parent), m_slider(new QSlider(Qt::Horizontal, this)), m_maxSteps(10), m_currentOffset(0), m_numSeries(0)
{
    setMinimumSize(800, 200);
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    this->setPalette(palette);

    // Initialize slider
    m_slider->setRange(0, 0);  // Initially disabled until we know the actual range
    connect(m_slider, &QSlider::valueChanged, this, [=](int value) {
        m_currentOffset = value;
        update();  // Redraw the widget with the new offset
    });
}

ChronogramWidget::ChronogramWidget(const QVector<QVector<int>>& intDataArrays,
                                   const QVector<QVector<float>>& floatDataArrays,
                                   const QVector<QVector<bool>>& boolDataArrays,
                                   const QVector<QString>& varNames,
                                   QWidget *parent)
    : QWidget(parent), m_intDataArrays(intDataArrays), m_floatDataArrays(floatDataArrays),
    m_boolDataArrays(boolDataArrays), m_varNames(varNames),
    m_slider(new QSlider(Qt::Horizontal, this)), m_maxSteps(10), m_currentOffset(0),
    m_numSeries(intDataArrays.size() + floatDataArrays.size() + boolDataArrays.size())
{
    setMinimumSize(800, 200);
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    this->setPalette(palette);

    // Initialize slider
    updateMaxSteps();  // Calculate the maximum steps based on input data
    m_slider->setRange(0, qMax(0, m_maxSteps - 10));  // Set range based on maximum steps
    connect(m_slider, &QSlider::valueChanged, this, [=](int value) {
        m_currentOffset = value;
        update();  // Redraw the widget with the new offset
    });

    calculateGlobalMinMax();  // Calculate global min and max values in data arrays
}

void ChronogramWidget::addSeries(const QVector<int>& intDataArray,
                                 const QVector<float>& floatDataArray,
                                 const QVector<bool>& boolDataArray,
                                 const QString& varName)
{
    m_intDataArrays.append(intDataArray);
    m_floatDataArrays.append(floatDataArray);
    m_boolDataArrays.append(boolDataArray);
    m_varNames.append(varName);
    updateMaxSteps();  // Recalculate maximum steps based on new series
    m_slider->setRange(0, qMax(0, m_maxSteps - 10));  // Adjust slider range if needed
    calculateGlobalMinMax();  // Recalculate global min and max values
    ++m_numSeries;
    update();  // Redraw the widget
}

void ChronogramWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int width = this->width();
    int height = this->height();
    int seriesHeight = height / (m_numSeries ? m_numSeries : 1);
    int fixedNumSteps = 10;  // Fixed number of steps to display
    int step = width / fixedNumSteps;  // Fixed step size for fixedNumSteps

    // Iterate through each series
    int currentSeriesIndex = 0;
    for (int series = 0; series < m_intDataArrays.size(); ++series)
    {
        // Draw variable name
        painter.drawText(10, seriesHeight * currentSeriesIndex + 20, m_varNames[series]);

        // Determine min and max values for current series
        int localMinInt = (m_intDataArrays[series].isEmpty()) ? 0 : m_intDataArrays[series][0];
        int localMaxInt = (m_intDataArrays[series].isEmpty()) ? 0 : m_intDataArrays[series][0];
        for (int i = 1; i < m_intDataArrays[series].size(); ++i)
        {
            int value = m_intDataArrays[series][i];
            if (value < localMinInt)
                localMinInt = value;
            if (value > localMaxInt)
                localMaxInt = value;
        }

        float localMinFloat = (m_floatDataArrays[series].isEmpty()) ? 0.0f : m_floatDataArrays[series][0];
        float localMaxFloat = (m_floatDataArrays[series].isEmpty()) ? 0.0f : m_floatDataArrays[series][0];
        for (int i = 1; i < m_floatDataArrays[series].size(); ++i)
        {
            float value = m_floatDataArrays[series][i];
            if (value < localMinFloat)
                localMinFloat = value;
            if (value > localMaxFloat)
                localMaxFloat = value;
        }

        bool localMinBool = (m_boolDataArrays[series].isEmpty()) ? false : m_boolDataArrays[series][0];
        bool localMaxBool = (m_boolDataArrays[series].isEmpty()) ? false : m_boolDataArrays[series][0];
        for (int i = 1; i < m_boolDataArrays[series].size(); ++i)
        {
            bool value = m_boolDataArrays[series][i];
            if (value < localMinBool)
                localMinBool = value;
            if (value > localMaxBool)
                localMaxBool = value;
        }

        // Determine overall min and max for the current series
        double overallMin = qMin(static_cast<double>(localMinInt), localMinFloat);
        overallMin = qMin(overallMin, static_cast<double>(localMinBool ? 1 : 0));

        double overallMax = qMax(static_cast<double>(localMaxInt), localMaxFloat);
        overallMax = qMax(overallMax, static_cast<double>(localMaxBool ? 1 : 0));

        // Map values to heights
        double minHeight = seriesHeight * currentSeriesIndex + 10;
        double maxHeight = seriesHeight * (currentSeriesIndex + 1) - 10;
        double valueRange = overallMax - overallMin;
        double valueToHeightRatio = (valueRange > 0) ? (maxHeight - minHeight) / valueRange : 1.0;

        for (int i = 0; i < fixedNumSteps; ++i)
        {
            int dataIndex = i + m_currentOffset;
            if (series < m_intDataArrays.size() && dataIndex < m_intDataArrays[series].size())
            {
                double value = static_cast<double>(m_intDataArrays[series][dataIndex]);
                double mappedHeight = mapValueToHeight(value, minHeight, maxHeight);

                // Calculate rectangle position and size
                int rectX = i * step;
                int rectY = mappedHeight;
                int rectWidth = step;
                int rectHeight = seriesHeight - 20; // Adjust the height as needed

                // Determine color based on value
                QColor rectColor = (value > 0) ? Qt::green : Qt::black;

                // Draw filled rectangle
                painter.fillRect(rectX, rectY, rectWidth, rectHeight, rectColor);
            }
        }

        // Draw legends
        painter.drawText(width - 100, seriesHeight * currentSeriesIndex + 20, "Min: " + QString::number(localMinInt));
        painter.drawText(width - 100, seriesHeight * (currentSeriesIndex + 1) - 10, "Max: " + QString::number(localMaxInt));

        // Draw separation line between series
        if (series < m_intDataArrays.size() - 1 || series < m_floatDataArrays.size() - 1 || series < m_boolDataArrays.size() - 1)
        {
            painter.setPen(Qt::yellow);
            painter.drawLine(0, seriesHeight * (currentSeriesIndex + 1), width, seriesHeight * (currentSeriesIndex + 1));
        }

        ++currentSeriesIndex;
    }

    // Draw time scale
    for (int i = 0; i < fixedNumSteps; ++i)
    {
        int x = i * step;
        painter.setPen(Qt::white);
        painter.drawLine(x, height - 10, x, height);
        painter.drawText(x + 2, height - 2, QString::number(i + m_currentOffset));
    }
}

void ChronogramWidget::updateMaxSteps()
{
    int maxSteps = 0;
    for (int series = 0; series < m_intDataArrays.size(); ++series)
    {
        maxSteps = qMax(maxSteps, m_intDataArrays[series].size());
    }
    for (int series = 0; series < m_floatDataArrays.size(); ++series)
    {
        maxSteps = qMax(maxSteps, m_floatDataArrays[series].size());
    }
    for (int series = 0; series < m_boolDataArrays.size(); ++series)
    {
        maxSteps = qMax(maxSteps, m_boolDataArrays[series].size());
    }
    m_maxSteps = maxSteps;
}

void ChronogramWidget::calculateGlobalMinMax()
{
    // Initialize min and max values
    int globalMinInt = 0, globalMaxInt = 0;
    float globalMinFloat = 0.0f, globalMaxFloat = 0.0f;
    bool globalMinBool = false, globalMaxBool = false;

    // Iterate through all series to find global min and max values
    for (int series = 0; series < m_intDataArrays.size(); ++series)
    {
        for (int i = 0; i < m_intDataArrays[series].size(); ++i)
        {
            int value = m_intDataArrays[series][i];
            if (value < globalMinInt)
                globalMinInt = value;
            if (value > globalMaxInt)
                globalMaxInt = value;
        }
    }

    for (int series = 0; series < m_floatDataArrays.size(); ++series)
    {
        for (int i = 0; i < m_floatDataArrays[series].size(); ++i)
        {
            float value = m_floatDataArrays[series][i];
            if (value < globalMinFloat)
                globalMinFloat = value;
            if (value > globalMaxFloat)
                globalMaxFloat = value;
        }
    }

    for (int series = 0; series < m_boolDataArrays.size(); ++series)
    {
        for (int i = 0; i < m_boolDataArrays[series].size(); ++i)
        {
            bool value = m_boolDataArrays[series][i];
            if (value && !globalMinBool)
                globalMinBool = true;
            if (!value && !globalMaxBool)
                globalMaxBool = false;
        }
    }
}

double ChronogramWidget::mapValueToHeight(double value, double minHeight, double maxHeight)
{
    return minHeight + (maxHeight - minHeight) * (value / 255.0); // Example mapping for 8-bit values
}


