#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mutitypeschrono.h"
#include "secondwindow.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MutiTypesChrono *multiTypesChrono;
    SecondWindow *secondWindow;

};
#endif // MAINWINDOW_H
