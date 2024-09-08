#include "widget.h"
#include "ui_widget.h"

#include <QTimer>
#include <QtMath>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    /* Qt 图表 https://doc.qt.io/qt-6/qtcharts-overview.html
     * 容器，用于管理和显示数据系列（如折线图、柱状图、散点图等）。它负责图表的整体布局、标题、坐标轴、图例和图形的呈现。（相当于一个画布）
     */
    chart = new QChart();

    // QChart添加到ui中
    ui->chartView->setChart(chart);

    // 定义两个坐标轴，一个用作X轴，一个用作Y轴，把X轴范围设置为0-10并放置在坐标系的底部，Y轴范围设置为0-10并放置在坐标系的左边
    axisX = new QValueAxis();
    chart->addAxis(axisX, Qt::AlignBottom);  // 【！！！注意！！！：创建之后需要立刻放入chart！否则会报错！！！】
    axisX->setTickCount(10);
    axisX->setRange(0, 100);

    axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);  // 【！！！注意！！！：创建之后需要立刻放入chart！否则会报错！！！】
    axisY->setRange(-1.1, 1.1);

    // 用于创建和显示平滑的曲线。与 QLineSeries 不同，QSplineSeries 会根据数据点生成平滑的曲线，使得图表的线条看起来更自然，而不仅仅是直线连接各个数据点。
    splineSeries = new QSplineSeries(this);
    chart->addSeries(splineSeries);  // 【！！！注意！！！：创建之后需要立刻放入chart！否则会报错！！！】
    splineSeries->attachAxis(axisX);
    splineSeries->attachAxis(axisY);

    // 创建 QScatterSeries 显示原始数据点
    scatterSeries = new QScatterSeries(this);
    chart->addSeries(scatterSeries);  // 【！！！注意！！！：创建之后需要立刻放入chart！否则会报错！！！】
    scatterSeries->setMarkerSize(5); // 设置点的大小
    scatterSeries->attachAxis(axisX);
    scatterSeries->attachAxis(axisY);
    scatterSeries->setColor(Qt::darkYellow);

    // 启用抗锯齿以平滑显示
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    timer = new QTimer(this);
    connect(ui->btnStart, &QPushButton::clicked, this, &Widget::timerStart);
    connect(ui->btnStop, &QPushButton::clicked, this, &Widget::timerStop);
    connect(timer, &QTimer::timeout, this, &Widget::drawPoint);
}

void Widget::timerStart()
{
    timer->start(50);
}


void Widget::timerStop()
{
    timer->stop();
}

void Widget::drawPoint()
{
    static double x = 0;
    double y = sin(x); // 使用 sin(x) 作为 Y 轴数据

    // 动态调整 X 轴范围，使曲线保持在视图中
    if (x > 100) {
        axisX->setRange(x - 100, x); // 滚动视图，保持显示最新数据
    }

    splineSeries->append(x, y); // 添加数据点到曲线
    scatterSeries->append(x, y);
    qDebug() << "Point (" << x << ", " << y << ")";
    x += 0.1; // 增加 x 值
}

Widget::~Widget()
{
    delete ui;
}