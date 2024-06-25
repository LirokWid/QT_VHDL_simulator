#include "secondwindow.h"

SecondWindow::SecondWindow(MutiTypesChrono *chronoWidget, QWidget *parent)
    : QWidget(parent), chronoWidget(chronoWidget)
{
    trueButton = new QPushButton("Add True", this);
    falseButton = new QPushButton("Add False", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(trueButton);
    layout->addWidget(falseButton);

    connect(trueButton, &QPushButton::clicked, this, &SecondWindow::handleTrueButton);
    connect(falseButton, &QPushButton::clicked, this, &SecondWindow::handleFalseButton);
}

void SecondWindow::handleTrueButton()
{
    chronoWidget->addPoint(true);
}

void SecondWindow::handleFalseButton()
{
    chronoWidget->addPoint(false);
}
