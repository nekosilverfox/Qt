#include "widget.h"
#include "ui_widget.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 使用 Manger 对象，做 GET/POST 请求
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    // 初始化一个 GET/POST 请求：设置请求头、URL
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) "
                      "AppleWebKit/537.36 (KHTML, like Gecko) "
                      "Chrome/131.0.0.0 Safari/537.36");  // 设置头，伪装成浏览器
    request.setUrl(QUrl("http://baidu.com"));

    // manager 发送数据并接收返回的数据
    // QNetworkReply* replay =  manager->post(request, "");
    QNetworkReply* replay =  manager->get(request);

    // 读取服务器回写的数据
    connect(replay, &QNetworkReply::readyRead, this, [=](){
        // 根据需求通过 QNetworkRequest::XXXXX 读取不同的 header
        QString header = replay->header(QNetworkRequest::LocationHeader).toString();
        qDebug() << "读取到的头：" << header;


        /* 注意，这里只是包含回复的数据段，不包含头！ */
        QByteArray data = replay->readAll();
        qDebug() << "读取的返回数据：" << data;
    });
}

Widget::~Widget()
{
    delete ui;
}
