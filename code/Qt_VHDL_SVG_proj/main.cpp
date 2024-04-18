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


//TODO selector inputs
