#ifndef CHRONOGRAMWIDGET_H
#define CHRONOGRAMWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>

class ChronogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChronogramWidget(QWidget *parent = nullptr);
    explicit ChronogramWidget(const QVector<bool>& boolArray, const QString& varName, QWidget *parent = nullptr);

    void addValue(bool value);
    void setVarName(const QString& varName);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<bool> m_boolArray;
    QString m_varName;
};

#endif // CHRONOGRAMWIDGET_H
