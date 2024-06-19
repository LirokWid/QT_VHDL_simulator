#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSvgWidget>
#include "ui_mainwindow.h"

#include "svgwidget.h"
#include "filestreeview.h"
#include "simulationstate.h"
#include "svghandler.h"
#include "debugwindow.h"

#include "chronogramwidget.h"
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
    void updateStateLabel(SimulationState::State state);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    SvgWidget *svgWidget;
    FilesTreeView *filesTreeView;
    SimulationState *simulationState;
    QLabel *stateLabel;
    SvgHandler *svgHandler;
    DebugWindow *debugWindow;
    ChronogramWidget *chronoWidget;

    void setSplitterToLeft(QSplitter *splitter, int leftSize);
};
#endif // MAINWINDOW_H
