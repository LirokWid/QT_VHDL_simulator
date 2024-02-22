#include <QCoreApplication>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QResource>
#include <QtSvg>

#include "svg_link_parser.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    // Load the SVG file from the resource system
    QResource::registerResource(":/resources.qrc");
    SvgLinkParser parser("C:\\Users\\BJRODIER\\Documents\\QT_REPO\\Qt_test_proj\\mux.svg");
    SvgLinkParser parser2("C:\\Users\\BJRODIER\\Documents\\QT_REPO\\Qt_test_proj\\Schematic_Paco.svg");

    return 0;
}
