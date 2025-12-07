#ifndef HEATMAPOVERLAYWIDGET_H
#define HEATMAPOVERLAYWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QPointF>
#include <QtUiPlugin/QDesignerExportWidget>

// \brief 用于显示眼动跟踪热图的可重用控件
// 该控件支持将透明蒙版覆盖在任意图片之上，实时绘制视线点热力效果。
// QDESIGNER_WIDGET_EXPORT 便于作为 Qt Designer 插件导出
class QDESIGNER_WIDGET_EXPORT HeatmapOverlayWidget : public QWidget
{
    Q_OBJECT
    // 背景图片，决定控件的默认尺寸与叠加对象
    Q_PROPERTY(QPixmap backgroundPixmap READ backgroundPixmap WRITE setBackgroundPixmap NOTIFY backgroundPixmapChanged)
    // 热图叠加整体透明度，便于透视原图
    Q_PROPERTY(qreal overlayOpacity READ overlayOpacity WRITE setOverlayOpacity NOTIFY overlayOpacityChanged)
    // 是否启用暗色蒙版，以弱化原图、突出热图
    Q_PROPERTY(bool dimMaskEnabled READ dimMaskEnabled WRITE setDimMaskEnabled NOTIFY dimMaskEnabledChanged)
    // 蒙版透明度，0 为无蒙版，1 为完全遮罩
    Q_PROPERTY(qreal dimMaskOpacity READ dimMaskOpacity WRITE setDimMaskOpacity NOTIFY dimMaskOpacityChanged)
    // 视线点扩散半径（像素），影响热图模糊范围
    Q_PROPERTY(int pointRadius READ pointRadius WRITE setPointRadius NOTIFY pointRadiusChanged)
    // 高斯衰减系数，值越大扩散越慢
    Q_PROPERTY(qreal decayFactor READ decayFactor WRITE setDecayFactor NOTIFY decayFactorChanged)
    // 最低强度对应的颜色
    Q_PROPERTY(QColor coolColor READ coolColor WRITE setCoolColor NOTIFY coolColorChanged)
    // 最高强度对应的颜色
    Q_PROPERTY(QColor hotColor READ hotColor WRITE setHotColor NOTIFY hotColorChanged)

public:
    explicit HeatmapOverlayWidget(QWidget *parent = nullptr);

    // 属性访问器
    QPixmap backgroundPixmap() const { return m_background; }
    qreal overlayOpacity() const { return m_overlayOpacity; }
    bool dimMaskEnabled() const { return m_dimMaskEnabled; }
    qreal dimMaskOpacity() const { return m_dimMaskOpacity; }
    int pointRadius() const { return m_pointRadius; }
    qreal decayFactor() const { return m_decayFactor; }
    QColor coolColor() const { return m_coolColor; }
    QColor hotColor() const { return m_hotColor; }

public slots:
    // 设置背景图，控件尺寸默认随背景调整
    void setBackgroundPixmap(const QPixmap &pixmap);
    void setOverlayOpacity(qreal value);
    void setDimMaskEnabled(bool enabled);
    void setDimMaskOpacity(qreal value);
    void setPointRadius(int radius);
    void setDecayFactor(qreal factor);
    void setCoolColor(const QColor &color);
    void setHotColor(const QColor &color);

    // 新增一个视线点，intensity 表示权重（0~1，超过将被截断）
    void addGazePoint(const QPointF &pos, qreal intensity = 1.0);
    // 清空已有热图
    void clearHeatmap();

signals:
    void backgroundPixmapChanged();
    void overlayOpacityChanged();
    void dimMaskEnabledChanged();
    void dimMaskOpacityChanged();
    void pointRadiusChanged();
    void decayFactorChanged();
    void coolColorChanged();
    void hotColorChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void ensureHeatmapBuffer();
    void drawGaussian(const QPointF &center, qreal strength);
    QImage colorizeHeatmap() const;
    qreal clamp01(qreal value) const;

private:
    QPixmap m_background;
    QImage m_heatmap; // alpha 通道累积热度值

    qreal m_overlayOpacity = 0.7;      // 热图整体透明度
    bool m_dimMaskEnabled = true;      // 是否启用暗色蒙版
    qreal m_dimMaskOpacity = 0.25;     // 蒙版透明度
    int m_pointRadius = 32;            // 单点影响半径
    qreal m_decayFactor = 18.0;        // 高斯衰减因子
    QColor m_coolColor = QColor(0, 128, 255); // 低热度颜色（蓝色）
    QColor m_hotColor = QColor(255, 64, 0);   // 高热度颜色（红色）
};

#endif // HEATMAPOVERLAYWIDGET_H
