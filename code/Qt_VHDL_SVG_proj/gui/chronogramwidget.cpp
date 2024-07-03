#include "chronogramwidget.h"
#include <QPainter>

ChronogramWidget::ChronogramWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800, 200);
}

ChronogramWidget::ChronogramWidget(const QVector<bool>& boolArray, const QString& varName, QWidget *parent)
    : QWidget(parent), m_boolArray(boolArray), m_varName(varName)
{
    setMinimumSize(800, 200);
}

void ChronogramWidget::addValue(bool value)
{
    m_boolArray.append(value);
    update();  // Redraw the widget
}

void ChronogramWidget::setVarName(const QString& varName)
{
    m_varName = varName;
    update();  // Redraw the widget
}

void ChronogramWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int width = this->width();
    int height = this->height();
    int step = width / (m_boolArray.size() ? m_boolArray.size() : 1);

    // Draw variable name
    painter.setPen(Qt::black);
    painter.drawText(10, 20, m_varName);

    // Draw middle line
    painter.drawLine(0, height / 2, width, height / 2);

    for (int i = 0; i < m_boolArray.size(); ++i)
    {
        int x = i * step;
        if (m_boolArray[i])
        {
            painter.setBrush(Qt::green);  // Green for high
            painter.drawRect(x, 0, step, height / 2);
        }
        else
        {
            painter.setBrush(Qt::red);  // Red for low
            painter.drawRect(x, height / 2, step, height / 2);
        }

        // Draw vertical lines to separate each state
        painter.setPen(Qt::black);
        painter.drawLine(x, 0, x, height);
    }
}
