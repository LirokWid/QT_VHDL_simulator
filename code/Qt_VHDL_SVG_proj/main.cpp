/**
 * @file main.cpp
 * @brief Main entry point for the application.
 *
 * This file is the main entry point for the application.
 * It is responsible for launching the graphical interface which will then start the simulation.
 */

#define GRAPHICAL

#ifdef GRAPHICAL

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}

#else

#include <QCoreApplication>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QResource>
#include <QtSvg>

#include "SystemcLinker.h"

int main(int argc, char *argv[])
{
    qDebug() << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "Starting";
    QCoreApplication a(argc, argv);
    // Load the SVG file from the resource system
    QResource::registerResource(":/resources.qrc");


    QString svg_file(":/svg/mux_symp.svg");
    
    SystemcLinker linker(svg_file);
    return 0;
}
#endif

//TODO selector inputs
