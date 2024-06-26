QT = core gui
QT += svg
QT += xml
QT += svgwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 cmdline
CONFIG += gnu++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


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
    $$files("systemc_modules/*.h") \
    gui/debugwindow.h \
    gui/elementsdisplay.h \
    gui/filestreeview.h \
    gui/mainwindow.h \
    gui/params.h \
    gui/simulationstate.h \
    gui/svghandler.h \
    gui/svgwidget.h \
    gui/view.h \
    parser_linker/ComponentsStruct.h \
    system/eventfilter.h

SOURCES += \
    $$files("parser_linker/*.cpp") \
    $$files("systemc_modules/*.cpp") \
    gui/debugwindow.cpp \
    gui/elementsdisplay.cpp \
    gui/filestreeview.cpp \
    gui/mainwindow.cpp \
    gui/simulationstate.cpp \
    gui/svghandler.cpp \
    gui/svgwidget.cpp \
    gui/ui_svg_item.cpp \
    gui/view.cpp \
    main.cpp \
    system/eventfilter.cpp


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

#win32:CONFIG(release, debug|release):    LIBS += -L$$PWD/../../../../../../../Qt/SystemC/lib/ -lsystemc
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../Qt/SystemC/lib/ -lsystemcd

#INCLUDEPATH += $$PWD/../../../../../../../Qt/SystemC/include
#DEPENDPATH  += $$PWD/../../../../../../../Qt/SystemC/include
