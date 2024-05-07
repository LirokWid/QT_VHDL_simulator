#include "filestreeview.h"
#include <QPushButton>
#include <QFileDialog>

FilesTreeView::FilesTreeView(QPushButton *folderButton, QTreeView *treeView, QWidget *parent)
    : QWidget(parent),
    folderButton(folderButton),
    treeView(treeView)
{
    connect(folderButton, &QPushButton::clicked, this, &FilesTreeView::searchFolder);

    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("");
    treeView->setModel(fileSystemModel);
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
