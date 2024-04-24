#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

#include <QGraphicsSvgItem>


#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QTreeView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:


signals:

private slots:


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTreeView *treeView;
    QFileSystemModel *model;
};

#endif // MAINWINDOW_H
