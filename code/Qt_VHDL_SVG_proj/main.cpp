#include <QCoreApplication>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QResource>
#include <QtSvg>

#include "svg_link_parser.h"


int main(int argc, char *argv[])
{

    qDebug() << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "Starting";
    QCoreApplication a(argc, argv);
    // Load the SVG file from the resource system
    QResource::registerResource(":/resources.qrc");

    SvgLinkParser parser1("C:\\Users\\BJRODIER\\Documents\\QT_REPO\\QT_VHDL_simulator\\code\\Qt_VHDL_SVG_proj\\mux_symp.svg");

    //SvgLinkParser parser2("C:\\Users\\BJRODIER\\Documents\\QT_REPO\\QT_VHDL_simulator\\code\\Qt_test_proj\\Schematic_Paco.svg");
    return 0;
}


//TODO selector inputs
