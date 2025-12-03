#ifndef HEATMAPOVERLAYWIDGETPLUGIN_H
#define HEATMAPOVERLAYWIDGETPLUGIN_H

#include <QObject>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QDesignerFormEditorInterface;

class HeatmapOverlayWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "HeatmapOverlayWidget.json")

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

#endif // HEATMAPOVERLAYWIDGETPLUGIN_H
