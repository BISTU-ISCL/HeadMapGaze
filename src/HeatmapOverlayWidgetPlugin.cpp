#include "HeatmapOverlayWidgetPlugin.h"
#include "HeatmapOverlayWidget.h"

#include <QtPlugin>
#include <QIcon>
#include <QtUiPlugin/QDesignerFormEditorInterface>

// Qt 6.10.0 MSVC 2022 64bit：插件元数据仅在集合插件上导出，避免重复元数据
// 干扰 Qt Designer 的加载流程，并与 Qt 6 的 JSON 元数据机制保持一致。
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

HeatmapOverlayWidgetCollectionPlugin::HeatmapOverlayWidgetCollectionPlugin(QObject *parent)
    : QObject(parent)
{
    // Qt 6 Designer 也支持集合插件形式，这里直接包装单个控件确保被发现
    m_plugins.append(new HeatmapOverlayWidgetPlugin(this));
}

QList<QDesignerCustomWidgetInterface *> HeatmapOverlayWidgetCollectionPlugin::customWidgets() const
{
    return m_plugins;
}

// Qt 6 不再需要 Q_EXPORT_PLUGIN2，元数据由 Q_PLUGIN_METADATA 与 JSON 提供
