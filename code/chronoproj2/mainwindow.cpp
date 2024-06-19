#include "chronogramwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Créer une instance de ChronogramWidget
    ChronogramWidget *chronogramWidget = new ChronogramWidget(this);

    // Exemple de données à ajouter
    QVector<int> intDataArray = {0, 1, 0, 1, 1, 0, 1, 0, 1, 0};
    QVector<float> floatDataArray = {0.5f, 0.7f, 0.4f, 0.6f, 0.8f, 0.3f, 0.2f, 0.9f, 0.1f, 0.0f};
    QVector<bool> boolDataArray = {true, false, true, true, false, true, false, true, true, false};
    QString varName = "Example Variable";

    // Ajouter les données à ChronogramWidget
    chronogramWidget->addSeries(intDataArray, floatDataArray, boolDataArray, varName);



    setCentralWidget(chronogramWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
