QT += core gui
QT += svg
QT += xml
QT += svgwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filestreeview.cpp \
    main.cpp \
    mainwindow.cpp \
    svghandling.cpp \
    svgwidget.cpp \
    ui_svg_item.cpp \
    view.cpp

HEADERS += \
    filestreeview.h \
    mainwindow.h \
    svghandling.h \
    svgwidget.h \
    ui_svg_item.h \
    view.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    $$files("svg/*.svg")\
    $$files("ui_img/*.png")

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
