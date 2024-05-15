#include "mainwindow.h"

#include "params.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup ui
    ui->setupUi(this);
    setWindowTitle(tr("SIMULATOR"));

    //Setup the debug interface
    debugWindow = new DebugWindow(ui->actionOpenDebugWindow, this);

#ifdef DEBUG
    for(int i=0;i<150;i++)
    {
        debugWindow->addMessage("test " + QString::number(i));
    }
    debugWindow->addMessage("Error text",Severity::Error);
    debugWindow->addMessage("Warning text",Severity::Warning);
    debugWindow->addMessage("Success text",Severity::Success);
    debugWindow->addMessage("Debug text",Severity::Debug);
    debugWindow->addMessage("Info text",Severity::Info);

#endif


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
    delete debugWindow;
    delete ui;
}


void MainWindow::on_stop_clicked()
{
#ifdef DEBUG
    static unsigned int i;
    if (i%2)
        debugWindow->addMessage("PshBtn sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss " + QString::number(i++),Severity::Error);
    else
        debugWindow->addMessage("PshBtn " + QString::number(i++),Severity::Warning);
#endif
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

void MainWindow::showDebugWindow()
{
    debugWindow->openWindow();
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
