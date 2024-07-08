#include "mainwindow.h"

#include "multitypeschrono.h"
#include "params.h"
#include "system/eventfilter.h" //temp debug
#include "system/threadmanager.h"

#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup ui
    ui->setupUi(this);
    setWindowTitle(tr("SIMULATOR"));

    EventFilter* myFilter = new EventFilter();
    this->installEventFilter(myFilter);

    //Setup the debug interface
    debugWindow = DebugWindow::getInstance(ui->actionOpenDebugWindow);


#ifdef DEBUG
    debugWindow->addMessage("Error text",   DebugWindow::Error);
    debugWindow->addMessage("Warning text", DebugWindow::Warning);
    debugWindow->addMessage("Success text", DebugWindow::Success);
    debugWindow->addMessage("Debug text",   DebugWindow::Debug);
    debugWindow->addMessage("Info text",    DebugWindow::Info);
#endif

    //Setup the simulation state and label
    simulationState = new SimulationState();
    stateLabel = ui->label;
    connect(simulationState, &SimulationState::stateChanged, this, &MainWindow::updateStateLabel);

    //Setup the folders tree view
    filesTreeView = new FilesTreeView(ui->folder_btn, ui->fileTreeView, svgHandler, simulationState);

    //Setup the svg view and handler for svg files management
    svgWidget = new SvgWidget();
    ui->svgLayout->addWidget(svgWidget);

    svgHandler = new SvgHandler(ui->componentsInfoContainer, simulationState, svgWidget, this);

    //Debug, should be dynamically added whith simulation result
    chronoWidget = new MultiTypesChrono(100);
    ui->tabWidget->addTab(chronoWidget, "Chronogram");

#ifdef DEBUG
    //svgHandler->loadSvg(TEMP_SVG_PATH);//debug
#endif

    //Setup the svg file close button
    connect(ui->closeFile, &QPushButton::clicked, this, &MainWindow::closeSvg);

    //Force resize the splitter
    setSplitterToLeft(ui->mainSplitter, 201);

    //Setup threading which keep ui running during simulation
    threadManager = new ThreadManager;
    connect(threadManager, &ThreadManager::resultReady, this, &MainWindow::updateGui);

    // Connect signals and slots
    //connect(startButton, &QPushButton::clicked, this, &MainWindow::on_minus_clicked);
    connect(threadManager, &ThreadManager::resultReady, this, [this](const QString &result)
        {
            updateGui(result);
        });
    connect(threadManager, &ThreadManager::workStarted, this, [this]()
        {
            updateGui("Work started...");
        });
    connect(threadManager, &ThreadManager::workFinished, this, [this]()
        {
            updateGui("Work finished.");
        });
    connect(threadManager, &ThreadManager::threadBusy, this, [this]()
        {
            updateGui("Thread is busy, please wait...");
        });
}

MainWindow::~MainWindow()
{
    delete debugWindow;
    delete ui;
}

void MainWindow::updateGui(const QString &message)
{
    ui->minus->setText(message);
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

void MainWindow::updateStateLabel(SimulationState::State state)
{
    switch (state)
    {
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

void MainWindow::on_minus_clicked()
{//Debug, ask for simulation to start, should use method to lauch simulation instead
    threadManager->startWork();
}


void MainWindow::on_pushButton_clicked()
{
    svgHandler->loadSvg(TEMP_SVG_PATH);//debug
}


void MainWindow::on_pushButton_2_clicked()
{
    svgHandler->loadSvg(TEMP_SVG_ER_PATH);//debug
}

