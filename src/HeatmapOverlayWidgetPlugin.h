#ifndef HEATMAPOVERLAYWIDGETPLUGIN_H
#define HEATMAPOVERLAYWIDGETPLUGIN_H

#include <QObject>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

class QDesignerFormEditorInterface;

class HeatmapOverlayWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit HeatmapOverlayWidgetPlugin(QObject *parent = nullptr);

    // QDesignerCustomWidgetInterface 接口实现
    void initialize(QDesignerFormEditorInterface *core) override;
    bool isInitialized() const override;
    QString name() const override;
    QString group() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QString includeFile() const override;
    QIcon icon() const override;
    bool isContainer() const override;
    QWidget *createWidget(QWidget *parent) override;
    QString domXml() const override;

private:
    bool m_initialized = false;
};

// Qt 6 Designer 集合插件包装，确保控件按 JSON 元数据被识别
class HeatmapOverlayWidgetCollectionPlugin : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
    // Qt 6 Designer 通过 JSON 元数据发现集合插件，这里显式引用文件
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface" FILE "HeatmapOverlayWidget.json")

public:
    explicit HeatmapOverlayWidgetCollectionPlugin(QObject *parent = nullptr);
    QList<QDesignerCustomWidgetInterface *> customWidgets() const override;

private:
    QList<QDesignerCustomWidgetInterface *> m_plugins;
};

#endif // HEATMAPOVERLAYWIDGETPLUGIN_H
