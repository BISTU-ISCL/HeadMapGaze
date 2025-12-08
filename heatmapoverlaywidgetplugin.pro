# Qt 6.10.0 qmake 工程：构建 Qt Designer 自定义控件插件（MSVC 2022 64bit Release）
TEMPLATE = lib
CONFIG += plugin designer c++17 release
QT += widgets designer gui

# 插件名称与输出位置（先输出到构建目录，install 时再复制到 Qt 安装的 designer 目录）
TARGET = heatmapoverlaywidget
DESTDIR = $$OUT_PWD/bin

# 源文件
SOURCES += \
    src/HeatmapOverlayWidget.cpp \
    src/HeatmapOverlayWidgetPlugin.cpp

HEADERS += \
    src/HeatmapOverlayWidget.h \
    src/HeatmapOverlayWidgetPlugin.h

# Designer 元数据 JSON，需要和插件放在同一目录
DISTFILES += src/HeatmapOverlayWidget.json

# 允许相对包含
INCLUDEPATH += src

# 安装规则，qmake install 时复制到插件目录（Windows 下为 *.dll，与 JSON 一起复制）
plugin_file = $$DESTDIR/$$qtLibraryTarget($$TARGET)
plugins.path = $$[QT_INSTALL_PLUGINS]/designer
plugins.files += $$plugin_file
plugins.files += $$PWD/src/HeatmapOverlayWidget.json
INSTALLS += plugins
