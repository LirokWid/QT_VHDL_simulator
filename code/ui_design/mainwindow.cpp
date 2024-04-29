#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    svgWidget = new QSvgWidget(this);
    svgWidget->load(QString(":/svg/mux_symp.svg"));

    ui->svgLayout->addWidget(svgWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
