#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "multitypeschrono.h"

class SecondWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SecondWindow(MultiTypesChrono *chronoWidget, QWidget *parent = nullptr);

private:
    MultiTypesChrono *chronoWidget;

    QPushButton *trueButton;
    QPushButton *falseButton;

    QVBoxLayout *layout;

private slots:
    void handleTrueButton();
    void handleFalseButton();
};

#endif // SECONDWINDOW_H
