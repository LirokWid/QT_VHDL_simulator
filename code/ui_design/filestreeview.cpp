#include "filestreeview.h"
#include <QPushButton>
#include <QFileDialog>

FilesTreeView::FilesTreeView(QPushButton *folderButton, QTreeView *treeView, SvgWidget *svgWidget, SimulationState *simulationState, QWidget *parent) :
    QWidget(parent),
    folderButton(folderButton),
    treeView(treeView),
    svgWidget(svgWidget),
    simulationState(simulationState)
{
    //Connect button to change root path of the tree view
    connect(folderButton, &QPushButton::clicked, this, &FilesTreeView::searchFolder);

    //Connect double click event to load svg into svgwidget
    connect(treeView,&QTreeView::doubleClicked,this,&FilesTreeView::handleFileDoubleClicked);


    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("");
    treeView->setModel(fileSystemModel);

    treeView->setColumnWidth(0,140);
    treeView->setColumnHidden(1,true); //Hide size column
    treeView->setColumnHidden(2,true); //Hide type column
}

FilesTreeView::~FilesTreeView()
{
    // Ensure that any remaining temporary files are deleted when the FilesTreeView is destroyed
    deleteTempSvg("");
}


void FilesTreeView::searchFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (!folderPath.isEmpty())
    {
        fileSystemModel->setRootPath(folderPath);
        treeView->setRootIndex(fileSystemModel->index(folderPath));
    }
    else
    {
        qDebug() << "No folder selected";
    }
}


void FilesTreeView::handleFileDoubleClicked(const QModelIndex &index) {
    if (!index.isValid())
    {
        return;
    }

    QString filePath = fileSystemModel->filePath(index);
    QString tempFilePath;

    if (copySvgToTemp(filePath, tempFilePath))
    {
        if (simulationState->getState() == SimulationState::IDLE_SVG_LOADED)
        {
            // Clear the temp SVG and replace it with the new one
            deleteTempSvg("");
        }
        svgWidget->loadSvg(tempFilePath);
        simulationState->setState(SimulationState::IDLE_SVG_LOADED);
    }
}
