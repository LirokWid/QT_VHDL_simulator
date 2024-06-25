#include "chronogramwidget.h"
#include "mainwindow.h"
#include "mutitypeschrono.h"
#include "secondwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Old code
    // Créer une instance de ChronogramWidget

    // Exemple de données à ajouter
    QVector<int> intDataArray = {0, 1, 0, 1, 1, 0, 1, 0, 1, 0};
    QVector<float> floatDataArray = {0.5f, 0.7f, 0.4f, 0.6f, 0.8f, 0.3f, 0.2f, 0.9f, 0.1f, 0.0f};
    QVector<bool> boolDataArray = {true, false, true, true, false, true, false, true, true, false};
    QString varName = "Example Variable";

    // Ajouter les données à ChronogramWidget

    ChronogramWidget *chronogramWidget1 = new ChronogramWidget();
    chronogramWidget1->addSeries(intDataArray,floatDataArray,boolDataArray,varName);

    ChronogramWidget *chronogramWidget2 = new ChronogramWidget();
    chronogramWidget2->addSeries(intDataArray,floatDataArray,boolDataArray,varName);
    */

    QVBoxLayout *layout = new QVBoxLayout();
    multiTypesChrono = new MutiTypesChrono();

    layout->addWidget(multiTypesChrono);
    ui->centralwidget->setLayout(layout);


    secondWindow = new SecondWindow(multiTypesChrono);
    secondWindow->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete secondWindow;
    delete multiTypesChrono;
}
