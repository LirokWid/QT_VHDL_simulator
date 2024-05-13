#include "filestreeview.h"
#include <QPushButton>
#include <QFileDialog>

FilesTreeView::FilesTreeView(
    QPushButton *folderButton,
    QTreeView *treeView,
    SvgHandler *svgHandler,
    SimulationState *simulationState,
    QWidget *parent
    ) :
    QWidget(parent),
    folderButton(folderButton),
    treeView(treeView),
    simulationState(simulationState)
{
    //Set up the svg widget
    this->svgHandler = svgHandler;

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
    //Verify that the file is a valid svg file
    if (!filePath.endsWith(".svg"))
    {
        qDebug() << "Invalid file type";
        return;
    }
    else
    {
        //Load the svg file into the svg widget
        svgHandler->changeSvg(filePath);
    }
}
