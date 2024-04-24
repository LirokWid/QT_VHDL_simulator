#include "mainwindow.h"

//#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTreeView>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QSplitter>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    model = new QFileSystemModel(this);
    model->setRootPath("C:\\Users\\BJRODIER\\Documents");
    ui->setupUi(this);

    //connect(ui->bStartSim, SIGNAL(clicked()), this, SLOT(receiver_bStartSim_clicked()));
    //ui->bStartSim->setEnabled(true);
    //emit ui->bStartSim->clicked();
}

MainWindow::~MainWindow()
{
    //delete ui;
}

/*
void MainWindow::receiver_bStartSim_clicked()
{
    QMessageBox msgBox;

    msgBox.setText("Simulation started");
    msgBox.exec();
}

*/

void MainWindow::on_pushButton_clicked()
{
    QString directory = "C:\\Users\\BJRODIER\\Documents";
    qDebug() << directory;
    if (!directory.isEmpty())
    {
        ui->treeView->setModel(model);
        ui->treeView->setRootIndex(model->index(directory));
    }
}

