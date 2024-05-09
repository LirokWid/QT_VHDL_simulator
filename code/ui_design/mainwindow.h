#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSvgWidget>
#include "svgwidget.h"
#include "filestreeview.h"
#include "simulationstate.h"

#include "view.h"

#define TEMP_SVG_PATH  ":/svg/mux_symp.svg"
#define TEMP_SVG_PATH2 ":/svg/mux.svg"
#define TEMP_SVG_PATH3 ":/svg/Schematic_Paco.svg"

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

    ~MainWindow();

private slots:
    void on_stop_clicked(); //temp debug svgwidget
    void closeSvg();
    void updateStateLabel(SimulationState::State state);



private:
    Ui::MainWindow *ui;
    SvgWidget *svgWidget;
    FilesTreeView *filesTreeView;
    SimulationState *simulationState;
    QLabel *stateLabel;

};
#endif // MAINWINDOW_H
