#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H


#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QPushButton>
#include "svghandling.h"
#include "svgwidget.h"


class FilesTreeView :
    public QWidget,
    public SvgHandling
{
    Q_OBJECT
public:
    explicit FilesTreeView(
        QPushButton *folderButton,
        QTreeView *treeView,
        SvgWidget *svgWidget,
        QWidget *parent = nullptr
    );
    ~FilesTreeView();

public slots:
    void searchFolder();
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    QPushButton *folderButton;
    QTreeView *treeView;
    QFileSystemModel *fileSystemModel;
    SvgWidget *svgWidget;

};

#endif // FILESTREEVIEW_H
