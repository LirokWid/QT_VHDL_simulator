#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSvgWidget>
#include <system/simulationmanager.h>
#include "ui_mainwindow.h"

#include "svgwidget.h"
#include "filestreeview.h"
#include "simulationstate.h"
#include "svghandler.h"
#include "debugwindow.h"

#include "multitypeschrono.h"
#include "view.h"

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
    void loadSvgFileFromPath(QString path);
    void showDebugWindow();

    ~MainWindow();

private slots:
    void on_stop_clicked(); //temp debug svgwidget
    void on_minus_clicked(); //temp debug svgwidget
    void closeSvg();
    void loadNormal_clicked();
    void loadError_clicked();

private:
    Ui::MainWindow *ui;
    SvgWidget *svgWidget;
    FilesTreeView *filesTreeView;
    SimulationState *state;
    QLabel *stateLabel;
    SvgHandler *svgHandler;
    DebugWindow *debugWindow;
    MultiTypesChrono *chronoWidget;

    SimulationManager *simManager;

    void setSplitterToLeft(QSplitter *splitter, int leftSize);
    void updateGui(const QString &message);

    void startSimulation();
    void stopSimulation();
};
#endif // MAINWINDOW_H
