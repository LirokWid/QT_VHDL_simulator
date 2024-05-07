#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "svgwidget.h"
#include "filestreeview.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("SIMULATOR"));

    //Setup the svg view
    svgWidget = new SvgWidget();
    svgWidget->loadSvg(TEMP_SVG_PATH);
    ui->svgLayout->addWidget(svgWidget);

    //Setup the tree view
    filesTreeView = new FilesTreeView(ui->folder_btn, ui->treeView, this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stop_clicked() //temp debug svgwidget
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

