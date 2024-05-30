#include "svghandler.h"
#include "mainwindow.h"

#include "SystemcLinker.h"
#include "elementsdisplay.h"

SvgHandler::SvgHandler(ElementsDisplay *display, QHBoxLayout *displayTitle, SimulationState *simulationState, SvgWidget *svgWidget, QObject *parent) :
    QObject(parent),
    simulationState(simulationState),
    svgWidget(svgWidget),
    display(display),
    displayTitle(displayTitle)
{
    if (!tempDir.isValid())
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("Failed to create temporary directory."));
    }

    DebugWindow::getInstance()->addDebug("SvgHandler created.");
    DebugWindow::getInstance()->addDebug("Temp directory created: " + tempDir.path() + "/");
}

SvgHandler::~SvgHandler()
{
}

bool SvgHandler::loadSvg(const QString &filePath)
{
    if (SimulationState::RUNNING == simulationState->getState())
    {
        QMessageBox::information(nullptr, tr("Information"), tr("Simulation is running. Please stop the simulation before loading a new SVG file."));
        return false;
    }

    if (SimulationState::IDLE_SVG_LOADED == simulationState->getState())
    { // Clear the previous SVG file it was loaded
        deleteTempSvg(tempFilePath);
    }

    if (copySvgToTemp(filePath))
    {
        //static_cast<MainWindow *>(parent())->showDebugWindow(); //temp
        if (loadAndParse(tempFilePath))
        {
            //Change the file name on the main window title
            QFileInfo fileInfo(filePath);
            static_cast<MainWindow *>(parent())->setWindowTitle(tr("SystemC Parser - ") + fileInfo.absoluteFilePath());
            return true;
        }
        else
        {
            DebugWindow::getInstance()->addError(tr("An error was find in the SVG file. Please check the file attributes."));
            QMessageBox::critical(nullptr, tr("Error"), tr("An error was find in the SVG file. Please check the file attributes."));
        }
    }
    else
    {
        DebugWindow::getInstance()->addError(tr("Failed to copy the SVG file to the temporary directory."));
        QMessageBox::critical(nullptr, tr("Error"), tr("Failed to copy the SVG file to the temporary directory."));
    }
    return false;
}

bool SvgHandler::loadAndParse(QString svgPath)
{
    svgWidget->loadSvg(svgPath);                        //Load the svg temp image to the svg widget
    if (linker)
    {//Delete linker if it already exists
        delete linker;
    }
    linker = new SystemcLinker(svgPath);                //Parse the svg file
    bool is_error = linker->getGlobalParsingError();        //Check if there is any parsing error
    if (is_error)
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("There was an error parsing the file, check the attributes."));
        parent()->
    }
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
        svgWidget->clearSvg();
        display->clearTree();
        deleteTempSvg(tempFilePath);

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

bool SvgHandler::copySvgToTemp(const QString &sourceFilePath)
{
    if (!tempDir.isValid())
    {
        DebugWindow::getInstance()->addError("Temp directory is not valid.");
        return false;
    }

    QFileInfo fileInfo(sourceFilePath);
    if (fileInfo.isFile() && fileInfo.suffix().toLower() == "svg")
    {
        this->tempFilePath = tempDir.path() + "/temp.svg";

        if (QFile::copy(sourceFilePath, tempFilePath))
        {
            // Emit signal to indicate SVG file is loaded
            simulationState->setState(SimulationState::IDLE_SVG_LOADED);
            return true;
        }
    }
    return false;
}

void SvgHandler::deleteTempSvg(const QString &tempFilePath)
{
    if (!tempFilePath.isEmpty())
    {
        QFile fileInfo(tempFilePath);
        fileInfo.setPermissions(QFile::ReadOther | QFile::WriteOther); //File has to be writable to be removed

        if(fileInfo.remove())
        {
            this->tempFilePath = "";
            DebugWindow::getInstance()->addDebug("Temp SVG file removed.");
        }
        else
        {
            DebugWindow::getInstance()->addError("Failed to remove temp SVG file.");
        }
    }
}
