#include "widget.h"
#include "ui_widget.h"

#include <QProcess>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    /* 将 secret.txt 嵌入 input.png，并将输出生成 test.png（注意，不支持 jpg，因为 jpg 会压缩数据），使用密码 -p 加密
     * java -jar /Users/fox/Downloads/openstego-0.8.6/lib/openstego.jar embed -a RandomLSB -mf secret.txt -cf IMG_9056.JPG -sf output.png -p happfox123
     *
     *java -jar /Users/fox/Downloads/openstego-0.8.6/lib/openstego.jar extract -a RandomLSB -sf output.png -xd . -p happfox123
     *
     */
    QProcess process;
    QString program = "java"; // 使用Java运行OpenStego的JAR文件
    QStringList arguments;
    arguments << "-jar" << "/Users/fox/雪狸的文件/Programma/Qt/061_Use_Java_jar_in_Qt/openstego.jar"
              << "embed" << "-a" << "RandomLSB"
              << "-mf" << "/Users/fox/雪狸的文件/Programma/Qt/061_Use_Java_jar_in_Qt/secret.txt"
              << "-cf" << "/Users/fox/Downloads/IMG_8971.JPG"
              << "-sf" << "/Users/fox/Downloads/output.png"
              << "-p" << "happfox123";

    process.start(program, arguments);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        qDebug() << "数据隐藏成功！" << process.readAllStandardOutput();
    } else {
        qDebug() << "错误：" << process.readAllStandardError();
    }

}

Widget::~Widget()
{
    delete ui;
}
