# Qt 6.10.0 qmake 工程：演示应用（MSVC 2022 64bit Release）
TEMPLATE = app
CONFIG += c++17 release
QT += widgets gui

SOURCES += \
    src/HeatmapOverlayWidget.cpp \
    src/HeatmapDemoMain.cpp

HEADERS += \
    src/HeatmapOverlayWidget.h

INCLUDEPATH += src
