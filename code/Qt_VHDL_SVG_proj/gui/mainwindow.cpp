#include "mainwindow.h"

#include "multitypeschrono.h"
#include "params.h"
#include "system/eventfilter.h" //temp debug
#include "system/simulationmanager.h"

#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup ui
    ui->setupUi(this);
    setWindowTitle(tr("SIMULATOR"));

    //Debug, show current process when pressing space
    EventFilter* myFilter = new EventFilter();
    this->installEventFilter(myFilter);

    //Setup the debug interface
    debugWindow = DebugWindow::getInstance(ui->actionOpenDebugWindow);

    //Setup the application state manager and ui label
    state = SimulationState::instance();
    stateLabel = ui->label;
    connect(state, &SimulationState::stateChanged, this, [this]()
    {
        stateLabel->setText(state->getStateStr());
    });


#ifdef DEBUG
    //Show differents display formats on the debug window
    debugWindow->addMessage("Error text",   DebugWindow::Error);
    debugWindow->addMessage("Warning text", DebugWindow::Warning);
    debugWindow->addMessage("Success text", DebugWindow::Success);
    debugWindow->addMessage("Debug text",   DebugWindow::Debug);
    debugWindow->addMessage("Info text",    DebugWindow::Info);
#endif

    //Setup the folders tree view
    filesTreeView = new FilesTreeView(ui->folder_btn, ui->fileTreeView, svgHandler);

    //Setup the svg view and handler for svg files management
    svgWidget = new SvgWidget();
    ui->svgLayout->addWidget(svgWidget);

    svgHandler = new SvgHandler(ui->componentsInfoContainer, state, svgWidget, this);

    //Debug, should be dynamically added whith simulation result
    chronoWidget = new MultiTypesChrono(100);
    ui->tabWidget->addTab(chronoWidget, "Chronogram");

    //Setup the svg file close button
    connect(ui->closeFile, &QPushButton::clicked, this, &MainWindow::closeSvg);

    //Force resize the splitter
    setSplitterToLeft(ui->mainSplitter, 201);

    //Debug, load svg buttons
    connect(ui->loadNormalBtn, &QPushButton::clicked, this, &MainWindow::loadNormal_clicked);
    connect(ui->loadErrorBtn, &QPushButton::clicked, this, &MainWindow::loadError_clicked);

    //Setup threading which keep ui running during simulation
    simManager = new SimulationManager;

    // Connect signals and slots
    //connect(startButton, &QPushButton::clicked, this, &MainWindow::on_minus_clicked);
    connect(simManager, &SimulationManager::resultReady, this, [this](const QString &result)
        {
            updateGui(result);
        });
    connect(simManager, &SimulationManager::workStarted, this, [this]()
        {
            updateGui("Work started...");
        });
    connect(simManager, &SimulationManager::workFinished, this, [this]()
        {
            updateGui("Work finished.");
        });
    connect(simManager, &SimulationManager::threadBusy, this, [this]()
        {
            updateGui("Thread is busy, please wait...");
        });

    connect(ui->start_sim, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(ui->stop_sim, &QPushButton::clicked, this, &MainWindow::stopSimulation);

    debugWindow->addDebug("UI initialized");
}

MainWindow::~MainWindow()
{
    delete debugWindow;
    delete ui;
}

void MainWindow::updateGui(const QString &message)
{
    ui->minus_sim->setText(message);
}

void MainWindow::startSimulation()
{
    simManager->lauchSimulation();
}

void MainWindow::stopSimulation()
{
    simManager->stopSimulation();
}

void MainWindow::on_stop_clicked()
{
#ifdef DEBUG
    static unsigned int i;
    if (i%2)
        debugWindow->addMessage("PshBtn error " + QString::number(i++),DebugWindow::Error);
    else
        debugWindow->addMessage("PshBtn " + QString::number(i++),DebugWindow::Warning);
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

void MainWindow::setSplitterToLeft(QSplitter *splitter, int leftSize)
{
    // Set a minimum size for the left widget
    splitter->widget(0)->setMinimumSize(leftSize, 0);

    // Calculate the size for the left and right widgets
    QList<int> sizes;
    sizes << leftSize << splitter->size().width() - leftSize;

    // Set the sizes
    splitter->setSizes(sizes);
}

void MainWindow::loadNormal_clicked()
{// debug
    svgHandler->loadSvg(TEMP_SVG_PATH);//debug
}

void MainWindow::loadError_clicked()
{// debug
    svgHandler->loadSvg(TEMP_SVG_ER_PATH);//debug
}

