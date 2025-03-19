#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QSvgRenderer>  // .pro 中 QT += svg

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // ui->label->setPixmap(recolorSVG(":/unlock.svg", Qt::red));   // OK
    // ui->label->setPixmap(recolorSVG(":/unlock.svg", QColor("#ECBC1D")));  // OK
    ui->label->setPixmap(recolorSVG(":/unlock.svg", QColor(245, 136, 56)));  // OK
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::recolorSVG 重新绘制 svg 图像的颜色
 * @param path
 * @param color
 * @return
 */
QPixmap Widget::recolorSVG(const QString& path, const QColor& color)
{
    QSvgRenderer renderer(path, this);
    if (!renderer.isValid())
    {
        qDebug() << "Failed to load SVG!";
        return QPixmap();
    }

    // 创建透明图像
    QImage image(renderer.defaultSize(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);

    // 绘制SVG并应用颜色
    QPainter painter(&image);
    renderer.render(&painter); // 渲染SVG的Alpha通道
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(image.rect(), color); // 填充颜色
    painter.end();

    return QPixmap::fromImage(image);
}
