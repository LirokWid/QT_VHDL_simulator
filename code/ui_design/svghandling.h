#ifndef SVGHANDLING_H
#define SVGHANDLING_H

#include <QObject>
#include <QTemporaryDir>
#include <QFile>
#include <QMessageBox>
#include "svgwidget.h"
#include "simulationstate.h"

/**
 * @brief The SvgHandling class provides functionality to handle SVG files.
 */
class SvgHandling : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a SvgHandling object.
     * @param simulationState The SimulationState object for managing the state of the application.
     * @param parent The parent object.
     */
    explicit SvgHandling(SimulationState *simulationState, QObject *parent = nullptr);

protected:
    /**
     * @brief Copies the SVG file to a temporary directory.
     * @param sourceFilePath The path of the source SVG file.
     * @param tempFilePath The path of the temporary SVG file.
     * @return True if the copy operation succeeds; otherwise, false.
     */
    bool copySvgToTemp(const QString &sourceFilePath, QString &tempFilePath);

    /**
     * @brief Deletes the temporary SVG file.
     * @param tempFilePath The path of the temporary SVG file.
     */
    void deleteTempSvg(const QString &tempFilePath);

private:
    SimulationState *simulationState; /**< The SimulationState object for managing the state of the application. */
    QTemporaryDir tempDir; /**< The temporary directory for storing SVG files. */
};

#endif // SVGHANDLING_H
