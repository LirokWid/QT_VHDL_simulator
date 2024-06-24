#ifndef CHRONOGRAMWIDGET_H
#define CHRONOGRAMWIDGET_H

#include <QWidget>
#include <QVector>
#include <QSlider>
#include <QResizeEvent>

class ChronogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChronogramWidget(QWidget *parent = nullptr);

    ChronogramWidget(const QVector<QVector<int>>& intDataArrays,
                     const QVector<QVector<float>>& floatDataArrays,
                     const QVector<QVector<bool>>& boolDataArrays,
                     const QVector<QString>& varNames,
                     QWidget *parent = nullptr);

    void addSeries(const QVector<int>& intDataArray,
                   const QVector<float>& floatDataArray,
                   const QVector<bool>& boolDataArray,
                   const QString& varName);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QVector<int>> m_intDataArrays;
    QVector<QVector<float>> m_floatDataArrays;
    QVector<QVector<bool>> m_boolDataArrays;
    QVector<QString> m_varNames;
    QSlider *m_slider;
    int m_maxSteps;
    int m_currentOffset;
    int m_numSeries;

    void updateMaxSteps();
    void calculateGlobalMinMax();
    double mapValueToHeight(double value, double minHeight, double maxHeight);
};

#endif // CHRONOGRAMWIDGET_H
