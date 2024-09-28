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

    ///////////////////////////////////
    QVector<double> sineWave;
    int totalPoints = 300;  // 3 periods * 15 points per period
    double step = (6 * M_PI) / (totalPoints - 1);  // Step size

    for (int i = 0; i < totalPoints; ++i)
    {
        double x = i * step;  // Calculate x value
        double y = std::sin(x)*10;  // Calculate y value (sine of x)
        sineWave.append(y);  // Append y to QVector
    }

    ///////////////////////////////////
    QVector<int> ramp;
    for (int var = 0; var < totalPoints; ++var)
    {
        ramp.append(var);
    }

    QVector<bool> square;
    for (int i = 0; i < totalPoints; ++i)
    {
        double y = rand() % 2;
        square.append(y);  // Append y to QVector
    }

    int steps = 300;
    double frequency1 = 1.0; // Frequency of the first sine wave
    double frequency2 = 2.0; // Frequency of the second sine wave
    double offset = 10;
    double samplingRate = 100.0; // Sampling rate

    QVector<double> sineSumValues;

    // Generate the summed sine values
    for (int i = 0; i < steps; ++i) {
        double time = i / samplingRate; // Time value
        double sine1 = std::sin(2 * M_PI * frequency1 * time); // First sine wave
        double sine2 = std::sin(2 * M_PI * frequency2 * time); // Second sine wave

        // Add the two sine waves together
        double sineSum = sine1 + sine2 + offset;

        // Append the summed value to the QVector
        sineSumValues.append(sineSum);
    }

    //Debug, should be dynamically added whith simulation results
    chronoWidgetSine = new MultiTypesChrono(sineWave);
    ui->tabWidget->addTab(chronoWidgetSine, "Sine");
    chronoWidgetRamp = new MultiTypesChrono(ramp);
    ui->tabWidget->addTab(chronoWidgetRamp, "ramp");
    chronoWidgetBool = new MultiTypesChrono(square);
    ui->tabWidget->addTab(chronoWidgetBool, "square");
    chronoWidgetSum = new MultiTypesChrono(sineSumValues);
    ui->tabWidget->addTab(chronoWidgetSum, "2Sine");
    ///////////////////////////////////

    //Setup threading which keep ui running during simulation
    simManager = new SimulationManager;

    //Setup the svg file close button
    connect(ui->closeFile, &QPushButton::clicked, this, &MainWindow::closeSvg);

    //Force resize the splitter
    setSplitterToLeft(ui->mainSplitter, 201);

    //Debug, load svg buttons
    connect(ui->loadNormalBtn, &QPushButton::clicked, this, &MainWindow::loadNormal_clicked);
    connect(ui->loadErrorBtn, &QPushButton::clicked, this, &MainWindow::loadError_clicked);

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
    //connect(ui->minus_sim, &QPushButton::clicked, this, &MainWindow::);
    //onnect(ui->plus_sim, &QPushButton::clicked, this, &MainWindow::);

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


void MainWindow::on_pushButton_clicked()
{
    double val = 2;
    chronoWidgetSine->addPoint(val);
}

