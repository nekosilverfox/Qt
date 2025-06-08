#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcess>  // 运行脚本需要
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    runPipInstall();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runPipInstall()
{
    // 异步处理
    QString program = "/usr/bin/python3";  // 使用 Python 模块调用更可靠
    QStringList arguments = {"-m", "pip", "install", "blind-watermark", "--user"};
    QProcess* process = new QProcess(this);

    connect(process, &QProcess::started, this, [=](){
         qDebug() << "Start run:" << program << arguments.join(" ");
    });

    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = process->readAllStandardOutput();
        qDebug() << "Real-time output:" << output.trimmed();
    });

    // 新增：连接信号：标准错误输出
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        QByteArray error = process->readAllStandardError();
        qDebug() << "Error:" << error.trimmed();
    });

    // 新增：处理进程错误
    connect(process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        qDebug() << "Process Error:" << error << process->errorString();
    });

    process->start(program, arguments);
}
