#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QTreeView>
#include <QFileSystemModel>
#include "svghandler.h"
#include "svgwidget.h"
#include "simulationstate.h"

/**
 * @brief The FilesTreeView class provides functionality to manage files in a tree view.
 */
class FilesTreeView : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a FilesTreeView object.
     * @param folderButton The QPushButton used to open a folder.
     * @param treeView The QTreeView for displaying files.
     * @param svgWidget The SvgWidget for displaying SVG files.
     * @param simulationState The SimulationState object for managing the state of the application.
     * @param parent The parent widget.
     */
    explicit FilesTreeView(
        QPushButton *folderButton,
        QTreeView *treeView,
        SvgHandler *svgHandler,
        SimulationState *simulationState,
        QWidget *parent = nullptr);

    ~FilesTreeView();

public slots:
    /**
     * @brief Slot to handle clicking on the folder button to search for a folder.
     */
    void searchFolder();

    /**
     * @brief Slot to handle double-clicking on a file in the tree view.
     * @param index The index of the double-clicked file.
     */
    void handleFileDoubleClicked(const QModelIndex &index);

private:
    QPushButton *folderButton; 		/**< The QPushButton used to open a folder. */
    QTreeView *treeView; 		/**< The QTreeView for displaying files. */
    SvgWidget *svgWidget; 		/**< The SvgWidget for displaying SVG files. */
    QFileSystemModel *fileSystemModel;	/**< The file model used by the TreeView */
    SimulationState *simulationState; 	/**< The SimulationState object for managing the state of the application */
    SvgHandler *svgHandler;		/**< The SvgHandling object for handling SVG files */

};

#endif // FILESTREEVIEW_H
