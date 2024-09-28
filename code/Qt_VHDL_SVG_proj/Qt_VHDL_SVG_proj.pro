QT = core gui
QT += svg
QT += xml
QT += svgwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 cmdline
CONFIG += gnu++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_INCDIR += \
    parser_linker \
    systemc_modules \
    gui


HEADERS += \
    $$files("parser_linker/*.h") \
    #$$files("systemC/systemc_modules/*.h") \
    gui/debugwindow.h \
    gui/elementsdisplay.h \
    gui/filestreeview.h \
    gui/mainwindow.h \
    gui/multitypeschrono.h \
    gui/params.h \
    gui/simulationstate.h \
    gui/svghandler.h \
    gui/svgwidget.h \
    parser_linker/ComponentsStruct.h \
    simulation/simulationworker.h \
    system/eventfilter.h \
    system/simulationmanager.h \
    systemC/systemcbuilder.h

SOURCES += \
    $$files("parser_linker/*.cpp") \
    #$$files("systemC/systemc_modules/*.cpp") \
    gui/debugwindow.cpp \
    gui/elementsdisplay.cpp \
    gui/filestreeview.cpp \
    gui/mainwindow.cpp \
    gui/multitypeschrono.cpp \
    gui/simulationstate.cpp \
    gui/svghandler.cpp \
    gui/svgwidget.cpp \
    main.cpp \
    simulation/simulationworker.cpp \
    system/eventfilter.cpp \
    system/simulationmanager.cpp \
    systemC/systemcbuilder.cpp


RESOURCES += \
    $$files("svg/*.svg") \
    ressources.qrc

FORMS += \
    $$files("forms/*.ui")


# SystemC
#SYSTEMC_INSTALL_PREFIX=C:/Qt/SystemC
SYSTEMC_INSTALL_PREFIX=S:/Qt/SystemC

LIBS        += -L$$SYSTEMC_INSTALL_PREFIX/lib/ -lsystemc
INCLUDEPATH +=   $$SYSTEMC_INSTALL_PREFIX/include
