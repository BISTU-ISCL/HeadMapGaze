# Qt 5.15.2 qmake 工程：构建 Designer 插件（必须使用与 Designer 相同的 Qt 版本）
TEMPLATE = lib
CONFIG += plugin designer c++14
QT += widgets designer gui

# 插件名称与输出位置（默认放到当前 Qt 套件的 designer 目录）
TARGET = heatmapoverlaywidget
DESTDIR = $$[QT_INSTALL_PLUGINS]/designer

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

# 安装规则，qmake install 时复制到插件目录
plugins.path = $$[QT_INSTALL_PLUGINS]/designer
plugins.files += $$DESTDIR/lib$${TARGET}.so
plugins.extra += $$QMAKE_COPY $$PWD/src/HeatmapOverlayWidget.json $$DESTDIR
INSTALLS += plugins
