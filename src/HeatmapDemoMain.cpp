#include "HeatmapOverlayWidget.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QVBoxLayout>

// \brief 简单示例应用：加载一张图片，并在其上叠加视线热图
// 展示控件的常用属性设置以及随机视线点模拟。
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建热图控件并加载背景图（使用外部传入的图片路径，默认 fallback）
    auto *heatmap = new HeatmapOverlayWidget;
    const QString imagePath = (argc > 1) ? QString::fromLocal8Bit(argv[1]) : QString();
    QPixmap background(imagePath);
    if (background.isNull()) {
        // 若未提供有效图片，则使用纯灰色背景代替
        background = QPixmap(800, 500);
        background.fill(QColor(230, 230, 230));
    }
    heatmap->setBackgroundPixmap(background);

    // 配置一些可视属性，方便观察效果
    heatmap->setOverlayOpacity(0.65);      // 热图透明度
    heatmap->setDimMaskOpacity(0.25);      // 蒙版轻微压暗背景
    heatmap->setPointRadius(28);           // 调整扩散半径
    heatmap->setDecayFactor(16.0);         // 调整衰减因子

    // UI 容器
    QWidget window;
    window.setWindowTitle(QStringLiteral("HeatmapOverlayWidget 示例"));

    auto *info = new QLabel(QStringLiteral(
        "使用 HeatmapOverlayWidget 叠加随机视线点\n"
        "• 可传入图片路径作为背景\n"
        "• 单击“清空热图”可重置\n"
        "• 随机点频率：20ms"));

    auto *clearButton = new QPushButton(QStringLiteral("清空热图"));

    auto *layout = new QVBoxLayout(&window);
    layout->addWidget(info);
    layout->addWidget(heatmap, 1);
    layout->addWidget(clearButton, 0, Qt::AlignRight);

    // 模拟眼动数据：定时生成随机坐标
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, heatmap, [heatmap]() {
        if (heatmap->size().isEmpty()) {
            return;
        }
        const qreal x = QRandomGenerator::global()->bounded(heatmap->width());
        const qreal y = QRandomGenerator::global()->bounded(heatmap->height());
        heatmap->addGazePoint(QPointF(x, y), 1.0);
    });
    timer.start(20);

    // 清空按钮：重置热图缓冲
    QObject::connect(clearButton, &QPushButton::clicked, heatmap, &HeatmapOverlayWidget::clearHeatmap);

    window.resize(background.size());
    window.show();

    return app.exec();
}
