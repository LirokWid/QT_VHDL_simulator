#ifndef SVGHANDLER_H
#define SVGHANDLER_H

#include <QObject>
#include <QTemporaryDir>
#include <QFile>
#include <QMessageBox>

#include "svgwidget.h"
#include "simulationstate.h"
#include "elementsdisplay.h"

#include "SystemcLinker.h"


/**
 * @brief The SvgHandling class provides functionality to handle SVG files.
 */
class SvgHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a SvgHandling object.
     * @param simulationState The SimulationState object for managing the state of the application.
     * @param parent The parent object.
     */
    explicit SvgHandler(ElementsDisplay *display, SimulationState *simulationState, SvgWidget *svgWidget, QObject *parent = nullptr);

    /**
     * @brief loadSvg
     * @param filePath
     */
    bool loadSvg(const QString &filePath);
    bool clearSvg();

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
    QString tempFilePath;
    SimulationState *simulationState; /**< The SimulationState object for managing the state of the application. */
    SvgWidget *svgWidget;
    QFileInfo *fileInfo;
    QTemporaryDir tempDir; /**< The temporary directory for storing SVG files. */
    QString getTempFilePath() const;
    SystemcLinker *linker;
    ElementsDisplay *display;

    bool loadAndParse(QString svgPath);
};

#endif // SVGHANDLER_H
