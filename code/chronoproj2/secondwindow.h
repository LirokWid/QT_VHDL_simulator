#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "mutitypeschrono.h"

class SecondWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SecondWindow(MutiTypesChrono *chronoWidget, QWidget *parent = nullptr);

private:
    MutiTypesChrono *chronoWidget;

    QPushButton *trueButton;
    QPushButton *falseButton;

    QVBoxLayout *layout;

private slots:
    void handleTrueButton();
    void handleFalseButton();
};

#endif // SECONDWINDOW_H
