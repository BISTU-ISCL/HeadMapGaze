# Qt 5.15.2 qmake 工程：演示应用
TEMPLATE = app
CONFIG += c++14
QT += widgets gui

SOURCES += \
    src/HeatmapOverlayWidget.cpp \
    src/HeatmapDemoMain.cpp

HEADERS += \
    src/HeatmapOverlayWidget.h

INCLUDEPATH += src
