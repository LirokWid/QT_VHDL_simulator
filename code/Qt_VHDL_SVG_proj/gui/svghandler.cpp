#include "svghandler.h"
#include "mainwindow.h"

#include "SystemcLinker.h"

SvgHandler::SvgHandler(SimulationState *simulationState, SvgWidget *svgWidget, QObject *parent) :
    QObject(parent),
    simulationState(simulationState),
    svgWidget(svgWidget)
{
    if (!tempDir.isValid())
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("Failed to create temporary directory."));
    }
}



bool SvgHandler::loadSvg(const QString &filePath)
{
    if (!tempDir.isValid())
    {
        return false;
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

        return loadAndParse(tempFilePath);

    }
    return false;
}

bool SvgHandler::loadAndParse(QString svgPath)
{
    //Load and parse the new svg
    svgWidget->loadSvg(svgPath);
    linker = new SystemcLinker(svgPath);
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
        // Emit signal to indicate SVG file is cleared
        simulationState->setState(SimulationState::IDLE);
    }
}
