#include "HeatmapOverlayWidget.h"

#include <QPainter>
#include <QtMath>
#include <QResizeEvent>

HeatmapOverlayWidget::HeatmapOverlayWidget(QWidget *parent)
    : QWidget(parent)
{
    // 允许使用半透明背景
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAttribute(Qt::WA_NoSystemBackground, true);
}

void HeatmapOverlayWidget::setBackgroundPixmap(const QPixmap &pixmap)
{
    if (pixmap.cacheKey() == m_background.cacheKey()) {
        return;
    }
    m_background = pixmap;
    ensureHeatmapBuffer();
    updateGeometry();
    update();
    emit backgroundPixmapChanged();
}

void HeatmapOverlayWidget::setOverlayOpacity(qreal value)
{
    value = clamp01(value);
    if (qFuzzyCompare(value, m_overlayOpacity)) {
        return;
    }
    m_overlayOpacity = value;
    update();
    emit overlayOpacityChanged();
}

void HeatmapOverlayWidget::setDimMaskEnabled(bool enabled)
{
    if (enabled == m_dimMaskEnabled) {
        return;
    }
    m_dimMaskEnabled = enabled;
    update();
    emit dimMaskEnabledChanged();
}

void HeatmapOverlayWidget::setDimMaskOpacity(qreal value)
{
    value = clamp01(value);
    if (qFuzzyCompare(value, m_dimMaskOpacity)) {
        return;
    }
    m_dimMaskOpacity = value;
    update();
    emit dimMaskOpacityChanged();
}

void HeatmapOverlayWidget::setPointRadius(int radius)
{
    radius = qMax(1, radius);
    if (radius == m_pointRadius) {
        return;
    }
    m_pointRadius = radius;
    update();
    emit pointRadiusChanged();
}

void HeatmapOverlayWidget::setDecayFactor(qreal factor)
{
    factor = qMax<qreal>(1.0, factor);
    if (qFuzzyCompare(factor, m_decayFactor)) {
        return;
    }
    m_decayFactor = factor;
    update();
    emit decayFactorChanged();
}

void HeatmapOverlayWidget::setCoolColor(const QColor &color)
{
    if (color == m_coolColor) {
        return;
    }
    m_coolColor = color;
    update();
    emit coolColorChanged();
}

void HeatmapOverlayWidget::setHotColor(const QColor &color)
{
    if (color == m_hotColor) {
        return;
    }
    m_hotColor = color;
    update();
    emit hotColorChanged();
}

void HeatmapOverlayWidget::addGazePoint(const QPointF &pos, qreal intensity)
{
    ensureHeatmapBuffer();
    if (m_heatmap.isNull()) {
        return;
    }

    // 防止强度溢出
    intensity = clamp01(intensity);
    drawGaussian(pos, intensity);
    update();
}

void HeatmapOverlayWidget::clearHeatmap()
{
    if (!m_heatmap.isNull()) {
        m_heatmap.fill(Qt::transparent);
        update();
    }
}

void HeatmapOverlayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 绘制背景图（缩放适配控件大小）
    if (!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background);
    } else {
        painter.fillRect(rect(), Qt::transparent);
    }

    // 可选蒙版，弱化背景
    if (m_dimMaskEnabled && m_dimMaskOpacity > 0) {
        QColor maskColor = Qt::black;
        maskColor.setAlphaF(clamp01(m_dimMaskOpacity));
        painter.fillRect(rect(), maskColor);
    }

    if (m_heatmap.isNull()) {
        return;
    }

    QImage colored = colorizeHeatmap();
    if (colored.isNull()) {
        return;
    }

    painter.setOpacity(clamp01(m_overlayOpacity));
    painter.drawImage(rect(), colored);
}

QSize HeatmapOverlayWidget::sizeHint() const
{
    // 默认尺寸跟随背景，没有背景时给出可视化友好的默认值，方便在 Designer 中展示
    if (!m_background.isNull()) {
        return m_background.size();
    }
    const QSize fallback(320, 240);
    return QWidget::sizeHint().expandedTo(fallback);
}

void HeatmapOverlayWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // 重新创建缓冲区，保持与控件一致
    ensureHeatmapBuffer();
}

void HeatmapOverlayWidget::ensureHeatmapBuffer()
{
    QSize targetSize = m_background.isNull() ? size() : m_background.size();
    if (targetSize.isEmpty()) {
        return;
    }
    if (m_heatmap.size() == targetSize) {
        return;
    }

    QImage newBuffer(targetSize, QImage::Format_ARGB32_Premultiplied);
    newBuffer.fill(Qt::transparent);

    // 如果已有热图，需要拉伸到新的尺寸，避免数据立即丢失
    if (!m_heatmap.isNull()) {
        QPainter p(&newBuffer);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        p.drawImage(QRect(QPoint(0, 0), targetSize), m_heatmap);
    }

    m_heatmap = newBuffer;
}

void HeatmapOverlayWidget::drawGaussian(const QPointF &center, qreal strength)
{
    if (m_heatmap.isNull()) {
        return;
    }

    const int radius = m_pointRadius;
    const qreal sigma = m_decayFactor;
    const int left = qMax(0, static_cast<int>(center.x()) - radius);
    const int right = qMin(m_heatmap.width() - 1, static_cast<int>(center.x()) + radius);
    const int top = qMax(0, static_cast<int>(center.y()) - radius);
    const int bottom = qMin(m_heatmap.height() - 1, static_cast<int>(center.y()) + radius);

    // 使用 alpha 通道累计热度
    for (int y = top; y <= bottom; ++y) {
        QRgb *scan = reinterpret_cast<QRgb *>(m_heatmap.scanLine(y));
        for (int x = left; x <= right; ++x) {
            const qreal dx = x - center.x();
            const qreal dy = y - center.y();
            const qreal dist2 = dx * dx + dy * dy;
            // 高斯函数
            qreal value = strength * qExp(-dist2 / (2.0 * sigma * sigma));
            int alpha = qAlpha(scan[x]);
            int newAlpha = qBound(0, alpha + static_cast<int>(value * 255), 255);
            scan[x] = qRgba(qRed(scan[x]), qGreen(scan[x]), qBlue(scan[x]), newAlpha);
        }
    }
}

QImage HeatmapOverlayWidget::colorizeHeatmap() const
{
    if (m_heatmap.isNull()) {
        return QImage();
    }

    QImage colored(m_heatmap.size(), QImage::Format_ARGB32_Premultiplied);
    colored.fill(Qt::transparent);

    const int width = m_heatmap.width();
    const int height = m_heatmap.height();

    for (int y = 0; y < height; ++y) {
        const QRgb *src = reinterpret_cast<const QRgb *>(m_heatmap.constScanLine(y));
        QRgb *dst = reinterpret_cast<QRgb *>(colored.scanLine(y));
        for (int x = 0; x < width; ++x) {
            int alpha = qAlpha(src[x]);
            if (alpha == 0) {
                dst[x] = qRgba(0, 0, 0, 0);
                continue;
            }
            qreal t = static_cast<qreal>(alpha) / 255.0;
            // 在冷色-热色间插值
            int r = static_cast<int>(m_coolColor.red() + (m_hotColor.red() - m_coolColor.red()) * t);
            int g = static_cast<int>(m_coolColor.green() + (m_hotColor.green() - m_coolColor.green()) * t);
            int b = static_cast<int>(m_coolColor.blue() + (m_hotColor.blue() - m_coolColor.blue()) * t);
            dst[x] = qRgba(r, g, b, alpha);
        }
    }

    return colored;
}

qreal HeatmapOverlayWidget::clamp01(qreal value) const
{
    return qBound<qreal>(0.0, value, 1.0);
}
