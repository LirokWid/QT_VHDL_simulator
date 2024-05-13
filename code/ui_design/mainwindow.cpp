#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "svgwidget.h"
#include "filestreeview.h"

#define DEBGUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("SIMULATOR"));

    //Setup the simulation state and label
    simulationState = new SimulationState();
    stateLabel = ui->label;
    connect(simulationState, &SimulationState::stateChanged, this, &MainWindow::updateStateLabel);

    //Setup the svg view and handler for svg files management
    svgWidget = new SvgWidget();
    ui->svgLayout->addWidget(svgWidget);

    svgHandler = new SvgHandler(simulationState, svgWidget, this);

#ifdef DEBUG
    svgHandler->changeSvg(TEMP_SVG_PATH);//debug
#endif

    //Setup the tree view
    filesTreeView = new FilesTreeView(ui->folder_btn, ui->treeView, svgHandler, simulationState, this);

    //Setup the svg file close button
    connect(ui->closeFile, &QPushButton::clicked, this, &MainWindow::closeSvg);

    //Setup the debug window display
    //TODO

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_stop_clicked()
{
    /*                                      //temp debug svgwidget
    if (state)
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
*/
}

void MainWindow::closeSvg()
{
    //Clear the svg widget
    svgHandler->clearSvg();
    qDebug()<<"svg cleared";
}

void MainWindow::loadSvgFileFromPath(QString path)
{
    //Load the svg file into the svg widget
    svgWidget->loadSvg(path);
    qDebug()<<"svg loaded from path: "<<path;
}

void MainWindow::updateStateLabel(SimulationState::State state)
{
    switch (state) {
    case SimulationState::IDLE:
        stateLabel->setText("Idle");
        break;
    case SimulationState::IDLE_SVG_LOADED:
        stateLabel->setText("Idle (SVG Loaded)");
        break;
    case SimulationState::RUNNING:
        stateLabel->setText("Running");
        break;
    default:
        stateLabel->setText("Unknown State");
        break;
    }
}


