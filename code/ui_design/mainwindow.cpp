#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "svgwidget.h"

#define TEMP_SVG_PATH  ":/svg/mux_symp.svg"
#define TEMP_SVG_PATH2 ":/svg/mux.svg"
#define TEMP_SVG_PATH3 ":/svg/Schematic_Paco.svg"



void MainWindow::init_svg_view()
{
    //TODO file error checking
    //1. create a view and set it to the svgLayout

    /*
    View *view = new View("Svg display view");
    view->view()->setScene(new QGraphicsScene(view));
    ui->svgLayout->addWidget(view);

    svgWidget = new SvgWidget();
    svgWidget->loadSvg(TEMP_SVG_PATH);
    ui->svgLayout->addWidget(svgWidget);
*/

    //svgWidget = new QSvgWidget(ui->centralwidget);
    //svgWidget->load(QString(TEMP_SVG_PATH));
    //ui->svgLayout->addWidget(svgWidget);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("VHDL SIMULATOR"));

    svgWidget = new SvgWidget();
    svgWidget->loadSvg(TEMP_SVG_PATH);
    ui->svgLayout->addWidget(svgWidget);
    //init_svg_view();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_3_clicked() //temp
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

