#include "HeatmapOverlayWidgetPlugin.h"
#include "HeatmapOverlayWidget.h"

#include <QtPlugin>
#include <QIcon>
#include <QtUiPlugin/QDesignerFormEditorInterface>

HeatmapOverlayWidgetPlugin::HeatmapOverlayWidgetPlugin(QObject *parent)
    : QObject(parent)
{
}

QString HeatmapOverlayWidgetPlugin::name() const
{
    return QStringLiteral("HeatmapOverlayWidget");
}

void HeatmapOverlayWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);
    if (m_initialized)
        return;
    m_initialized = true;
}

bool HeatmapOverlayWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QString HeatmapOverlayWidgetPlugin::group() const
{
    return QStringLiteral("Visualization");
}

QString HeatmapOverlayWidgetPlugin::toolTip() const
{
    return QStringLiteral("眼动热图叠加控件");
}

QString HeatmapOverlayWidgetPlugin::whatsThis() const
{
    return QStringLiteral("支持将眼动轨迹转换为热图叠加在任意图片之上的可视化控件。");
}

QString HeatmapOverlayWidgetPlugin::includeFile() const
{
    return QStringLiteral("HeatmapOverlayWidget.h");
}

QIcon HeatmapOverlayWidgetPlugin::icon() const
{
    // 此处未提供图标，占位返回空图标；可根据需要替换
    return QIcon();
}

bool HeatmapOverlayWidgetPlugin::isContainer() const
{
    return false;
}

QWidget *HeatmapOverlayWidgetPlugin::createWidget(QWidget *parent)
{
    // 保证在 Qt Designer 中创建控件实例
    return new HeatmapOverlayWidget(parent);
}

QString HeatmapOverlayWidgetPlugin::domXml() const
{
    // 提供 Designer 使用的 XML 描述，包含默认尺寸，方便拖拽使用
    return QStringLiteral(
        "<widget class=\"HeatmapOverlayWidget\" name=\"heatmapOverlayWidget\">"
        " <property name=\"geometry\">"
        "  <rect><x>0</x><y>0</y><width>320</width><height>240</height></rect>"
        " </property>"
        " <property name=\"toolTip\" >"
        "  <string>眼动热图叠加控件</string>"
        " </property>"
        " <property name=\"whatsThis\" >"
        "  <string>用于在任意背景上绘制眼动热图的可重用控件。</string>"
        " </property>"
        "</widget>"
    );
}

// 必须导出插件元数据
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(HeatmapOverlayWidgetPlugin, HeatmapOverlayWidgetPlugin)
#endif
