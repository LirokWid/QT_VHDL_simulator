#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSvgWidget>
#include "svgwidget.h"

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
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    SvgWidget *svgWidget;

    bool state = false; //temp

};
#endif // MAINWINDOW_H
