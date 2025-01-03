#include "widget.h"
#include "ui_widget.h"

/* Json 对应头文件 */
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    rewriteJsonValueFromFile();
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::writeJsonFile 测试将 JSON 构建并写入文件
 */
void Widget::writeJsonFile()
{
    /* 1. 假如我们需要构建以下创建 JSON 对象
     * {
     *      "server": {
     *          "ip": "127.0.0.1",
     *          "port": "8080"
     *      }
     * }
     *
     * 由于 JSON 是由键值对（Key-Value）的形式存储，所以我们需要从内到外构建Json对象
     */
    QJsonObject jsonSub;
    jsonSub.insert("ip", QJsonValue("127.0.0.1"));
    jsonSub.insert("port", QJsonValue("8080"));

    QJsonObject jsonObj;
    jsonObj.insert("server", jsonSub);

    /* 2. 将 JSON 写入到文件
     *      1. 数据转换为 JsonDocument
     *      2. JsonDocument.toJson(); 转换为 QByteArray 二进制
     *      3. 写入文件
     */
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson();

    QFile file("/Users/fox/雪狸的文件/Programma/Qt/042_Json_read_write/output.json");
    file.open(QIODeviceBase::WriteOnly);
    file.write(jsonData);
    file.close();
}


/**
 * @brief Widget::readJsonFile 从文件中读取 JSON
 */
void Widget::readJsonFile()
{
    /* 1. 假如我们读取以下 JSON 对象
     * {
     *      "server": {
     *          "ip": "127.0.0.1",
     *          "port": "8080"
     *      }
     * }
     *
     * 由于 JSON 是由键值对（Key-Value）的形式存储，所以我们需要从外到内读取Json对象
     */
    QFile file("/Users/fox/雪狸的文件/Programma/Qt/042_Json_read_write/input.json");
    file.open(QIODevice::ReadOnly);
    QByteArray jsonData = file.readAll();
    file.close();

    /* 2. 使用 QJsonDocument 加载 JSON 对象 并 检查是否为有效的 JSON 文档 */
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        qDebug() << "Invalid JSON format!";
        return;
    }

    /* 3. 获取根对象 */
    QJsonObject rootObj = jsonDoc.object();

    /* 4. 提取 "server" 对象
     * 其中，rootObj.contains("server") - 如果指定对象包含 "server" 则返回 true
     */
    if (rootObj.contains("server") && rootObj["server"].isObject())
    {
        QJsonObject objServer = rootObj["server"].toObject();

        /* 提取 "ip" 和 "port" 字段 */
        QString ip = objServer.value("ip").toString();
        QString port = objServer.value("port").toString();

        qDebug() << "Server IP:" << ip;
        qDebug() << "Server Port:" << port;
    }

    /* 4. 提取 "user" 对象 */
    if (rootObj.contains("user") && rootObj["user"].isObject())
    {
        QJsonObject objServer = rootObj["user"].toObject();

        /* 提取 "name" 和 "age" 字段 */
        QString name = objServer.value("name").toString();
        QString age = objServer.value("age").toString();

        qDebug() << "User name:" << name;
        qDebug() << "User age:" << age;
    }
}

/**
 * @brief Widget::rewriteJsonValueFromFile 修改 JSON 文件中某个 key 的 value
            {
                "server":
                {
                    "ip": "127.0.0.1",
                    "port": "8080"
                },

                "user":
                {
                    "name": "fox",
                    "age": "17"
                }
            }
 */
void Widget::rewriteJsonValueFromFile()
{
    QFile file("/Users/fox/雪狸的文件/Programma/Qt/042_Json_read_write/input.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Can not open file";
        return;
    }

    // 读取文件内容
    QByteArray fileData = file.readAll();
    file.close();

    // 解析 JSON
    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "Invalid JSON file";
        return;
    }

    // 获取根对象
    QJsonObject rootObj = doc.object();

    // 这里我们修改 user 的 name
    if (!rootObj.contains("user") || !rootObj["user"].isObject())
    {
        qWarning() << "JSON does not contain 'login' object";
        return;
    }

    // 更新 user 的值
    QJsonObject userObj = rootObj["user"].toObject();
    userObj["name"] = "silverfox";
    userObj["age"] = QString::number(userObj["age"].toInt() + 1);

    // 更新根对象
    rootObj["user"] = userObj;

    // 写回 JSON 文件
    doc.setObject(rootObj);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() <<"Can not open file";
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

}
