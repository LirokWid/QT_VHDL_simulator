#include "filestreeview.h"
#include <QPushButton>
#include <QFileDialog>

FilesTreeView::FilesTreeView(QPushButton *folderButton, QTreeView *treeView, QWidget *parent)
    : QWidget(parent),
    folderButton(folderButton),
    treeView(treeView)
{
    //Change root path of the tree view
    connect(folderButton, &QPushButton::clicked, this, &FilesTreeView::searchFolder);

    //Connect double click event to load svg into svgwidget
    connect(treeView,&QTreeView::doubleClicked,this,&FilesTreeView::on_treeView_doubleClicked);

    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("");
    treeView->setModel(fileSystemModel);

    treeView->setColumnWidth(0,150);
    treeView->setColumnHidden(1,true); //Hide size column
    treeView->setColumnHidden(2,true); //Hide type column
}


void FilesTreeView::searchFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

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


void FilesTreeView::on_treeView_doubleClicked(const QModelIndex &index)
{
    //Load svg into svgwidget from the selected file


    qDebug() << "Double clicked on: " << fileSystemModel->filePath(index);
}

