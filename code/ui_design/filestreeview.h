#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H


#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QPushButton>

class FilesTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit FilesTreeView(QPushButton *folderButton, QTreeView *treeView, QWidget *parent = nullptr);

public slots:
    void searchFolder();
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    QPushButton *folderButton;
    QTreeView *treeView;
    QFileSystemModel *fileSystemModel;
};

#endif // FILESTREEVIEW_H
