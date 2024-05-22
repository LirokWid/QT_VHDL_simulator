#include "svghandler.h"
#include "mainwindow.h"

#include "SystemcLinker.h"
#include "elementsdisplay.h"

SvgHandler::SvgHandler(ElementsDisplay *display, SimulationState *simulationState, SvgWidget *svgWidget, QObject *parent) :
    QObject(parent),
    simulationState(simulationState),
    svgWidget(svgWidget),
    display(display)
{
    if (!tempDir.isValid())
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("Failed to create temporary directory."));
    }
}

SvgHandler::~SvgHandler()
{
}

bool SvgHandler::loadSvg(const QString &filePath)
{
    if (!tempDir.isValid())
    {
        DebugWindow::getInstance()->addMessage("Failed to create temporary directory.", DebugWindow::Error);
    }

    if (SimulationState::RUNNING == simulationState->getState())
    {
        QMessageBox::information(nullptr, tr("Information"), tr("Simulation is running. Please stop the simulation before loading a new SVG file."));
        return false;
    }

    if (SimulationState::IDLE_SVG_LOADED == simulationState->getState())
    { // Clear the previous SVG file it was loaded
        deleteTempSvg(tempDir.path() + "/" + QFileInfo(filePath).fileName());
    }

    if (copySvgToTemp(filePath, tempFilePath))
    {
        static_cast<MainWindow *>(parent())->showDebugWindow(); //temp


        int success = loadAndParse(tempFilePath);
        if (!success)
        {
            QMessageBox::critical(nullptr, tr("Error"), tr("An error was find in the SVG file. Please check the file attributes."));
        }

    }
    return false;
}

bool SvgHandler::loadAndParse(QString svgPath)
{

    svgWidget->loadSvg(svgPath);                        //Load the svg temp image to the svg widget
    linker = new SystemcLinker(svgPath);                //Parse the svg file
    linker->getGlobalParsingError();                    //Check if there is any parsing error
    QMessageBox::critical(nullptr, tr("Error"), tr("There was an error parsing the file, check the attributes."));
    display->loadTree(linker->get_components_list());   //Display the parsed data in the elements tree view

    return true;
}

bool SvgHandler::clearSvg()
{
    if (SimulationState::RUNNING == simulationState->getState())
    {
        QMessageBox::information(nullptr, tr("Information"), tr("Simulation is running. Please stop the simulation before clearing the SVG file."));
        return false;
    }
    if (SimulationState::IDLE_SVG_LOADED == simulationState->getState())
    {
        deleteTempSvg(tempFilePath);
        svgWidget->clearSvg();
        display->clearTree();

        // Emit signal to indicate SVG file is cleared
        simulationState->setState(SimulationState::IDLE);

        return true;
    }
    return false;
}

QString SvgHandler::getTempFilePath() const
{
    return tempFilePath;
}

bool SvgHandler::copySvgToTemp(const QString &sourceFilePath, QString &tempFilePath)
{
    if (!tempDir.isValid())
    {
        return false;
    }

    QFileInfo fileInfo(sourceFilePath);
    if (fileInfo.isFile() && fileInfo.suffix().toLower() == "svg")
    {
        tempFilePath = tempDir.path() + "/" + fileInfo.fileName();
        bool success = QFile::copy(sourceFilePath, tempFilePath);
        if (success)
        {
            // Emit signal to indicate SVG file is loaded
            simulationState->setState(SimulationState::IDLE_SVG_LOADED);
        }
        return success;
    }

    return false;
}

void SvgHandler::deleteTempSvg(const QString &tempFilePath)
{
    if (!tempFilePath.isEmpty())
    {
        QFile::remove(tempFilePath);
        tempDir.remove();
    }
}
