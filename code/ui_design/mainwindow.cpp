#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "svgwidget.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("VHDL SIMULATOR"));

    //Setup the svg view
    svgWidget = new SvgWidget();
    svgWidget->loadSvg(TEMP_SVG_PATH);
    ui->svgLayout->addWidget(svgWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked() //temp debug svgwidget
{
    if(state)
    {
        state = !state;
        svgWidget->loadSvg(TEMP_SVG_PATH2);

    }
    else
    {
        state = !state;
        svgWidget->loadSvg(TEMP_SVG_PATH3);
    }
    qDebug()<<"new svg loaded";
}

