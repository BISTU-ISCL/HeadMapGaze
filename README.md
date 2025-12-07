# HeadMapGaze

该仓库提供一个用于 Qt 应用的**视线热图控件** `HeatmapOverlayWidget`，可将眼动跟踪设备传输的视线点实时叠加在任意图片上。控件尺寸默认与背景图片一致，内置透明蒙版与丰富的可调属性，便于在可视化界面中快速集成。

## 核心特性
- 透明热图叠加：热图保持透明，可直接覆盖在背景图片上。可选暗色蒙版以弱化背景。
- 可调参数：支持热度颜色、扩散半径、衰减系数、整体透明度等多种属性调整。
- 轻量易用：只需定期调用 `addGazePoint()` 传入视线坐标即可实时刷新热图。

## 主要文件
- `src/HeatmapOverlayWidget.h/.cpp`：Qt 控件实现，包含所有属性与绘制逻辑。

## 属性说明
| 属性 | 作用 | 默认值 |
| --- | --- | --- |
| `backgroundPixmap` | 背景图；决定控件默认尺寸与绘制对象 | 空 pixmap |
| `overlayOpacity` | 热图整体透明度（0-1） | 0.7 |
| `dimMaskEnabled` | 是否启用暗色蒙版 | true |
| `dimMaskOpacity` | 蒙版透明度（0-1） | 0.25 |
| `pointRadius` | 单个视线点的扩散半径（像素） | 32 |
| `decayFactor` | 高斯衰减系数，越大扩散越慢 | 18.0 |
| `coolColor` | 最低热度对应的颜色 | RGB(0,128,255) |
| `hotColor` | 最高热度对应的颜色 | RGB(255,64,0) |

## 快速使用示例
```cpp
#include "HeatmapOverlayWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QTimer>
#include <QRandomGenerator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto *heatmap = new HeatmapOverlayWidget;
    heatmap->setBackgroundPixmap(QPixmap(":/images/sample.jpg"));
    heatmap->setOverlayOpacity(0.65);
    heatmap->setDimMaskOpacity(0.3);

    QWidget window;
    QVBoxLayout layout(&window);
    layout.addWidget(heatmap);
    window.show();

    // 模拟眼动数据
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (heatmap->size().isEmpty()) {
            return;
        }
        QPointF p(QRandomGenerator::global()->bounded(heatmap->width()),
                  QRandomGenerator::global()->bounded(heatmap->height()));
        heatmap->addGazePoint(p, 1.0);
    });
    timer.start(30);

    return app.exec();
}
```

## 开发提示
- 调整 `pointRadius` 与 `decayFactor` 可以控制热图扩散与边缘柔滑程度。
- 通过 `clearHeatmap()` 可随时重置热图，便于切换场景或清理旧数据。
- 如需自定义颜色映射，可直接在运行时修改 `coolColor` 与 `hotColor` 属性。

## 示例应用
- `src/HeatmapDemoMain.cpp`：完整窗口示例，支持传入图片路径作为背景，定时生成随机视线点叠加到热图上，并提供“清空热图”按钮。
- 运行方式（Qt 5.15.2，qmake 示例）：
  ```bash
  qmake heatmapoverlaywidgetdemo.pro && make
  ./heatmapoverlaywidgetdemo /path/to/your/image.jpg
  ```
  如未提供有效图片路径，程序会自动生成一张灰色背景图以便快速体验热图效果。

## 作为 Qt 控件库 / Designer 插件使用
- 插件实现：`src/HeatmapOverlayWidgetPlugin.h/.cpp/.json`，可编译为 `designer` 插件放入 Qt 控件库（已按 Qt 5.15.2 兼容性仅在集合插件上导出 JSON 元数据，避免多重元数据导致 Designer 不加载）。
- Qt 版本匹配原则：**Qt Designer / Qt Creator 的 Qt 版本必须与插件编译所用的 Qt 版本一致**。例如：
  - Qt Creator 自带的 Designer 若是基于 Qt 6 构建，则无法加载 Qt 5.15.2 编译的插件，需要改用 Qt 6 Kit 重新编译插件，或安装/使用 Qt 5.15.2 的独立 Qt Designer（或基于 Qt 5.15.2 的 Qt Creator）。
  - 仅当插件与 Designer Qt 版本一致时，才会在“Visualization”分组中出现控件。
- qmake 插件工程（Qt 5.15.2）已经随仓库提供：`heatmapoverlaywidgetplugin.pro`。构建与安装示例：
  ```bash
  # 使用 Qt 5.15.2 的 qmake 与编译器
  qmake heatmapoverlaywidgetplugin.pro && make
  # 安装到当前 Qt 套件的 designer 插件目录（需要写权限）
  make install
  # 如无安装权限，可手动复制生成的 so/dll 与 HeatmapOverlayWidget.json 到 Qt 安装目录的 plugins/designer/
  ```
- 若仍未被 Qt Creator / Designer 识别，请确认：
  - 插件编译使用的 Qt 版本与 Designer 内置 Qt 版本一致；
  - 插件已复制到对应 Qt 安装的 `plugins/designer/` 目录；
  - 在 Qt Creator 的**帮助 -> 关于插件**中能看到 `heatmapoverlaywidget`，若有旧版本请先清理再复制新的动态库。
