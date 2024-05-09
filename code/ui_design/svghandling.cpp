#include "svghandling.h"

SvgHandling::SvgHandling(SimulationState *simulationState, QObject *parent) :
    QObject(parent),
    simulationState(simulationState)
{
    if (!tempDir.isValid())
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("Failed to create temporary directory."));
    }
}

bool SvgHandling::copySvgToTemp(const QString &sourceFilePath, QString &tempFilePath)
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

void SvgHandling::deleteTempSvg(const QString &tempFilePath)
{
    if (!tempFilePath.isEmpty())
    {
        QFile::remove(tempFilePath);
        // Emit signal to indicate SVG file is cleared
        simulationState->setState(SimulationState::IDLE);
    }
}
