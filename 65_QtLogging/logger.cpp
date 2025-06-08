#include "logger.h"
#include <QHash>
#include <QDateTime>


QString Logger::_date = "yyyy-MM-dd hh:mm:ss";  // 默认日期格式
QString Logger::_sep = " | ";                   // 默认分隔符
QFile* Logger::_logFile = nullptr;
bool   Logger::_isInit  = false;
QHash<QtMsgType, QString> Logger::_contextName = {
    {QtMsgType::QtDebugMsg,     " Debug  "},
    {QtMsgType::QtInfoMsg,      "  Info  "},
    {QtMsgType::QtWarningMsg,   "Warning "},
    {QtMsgType::QtCriticalMsg,  "Critical"},
    {QtMsgType::QtFatalMsg,     " Fatal  "}
};

/**
 * @brief Logger::init 初始化 logger
 * @param path log文件路径
 * @param date 日期格式
 * @param sep 分隔符
 */
void Logger::init(const QString &path, const QString &date, const QString &sep)
{
    if (_isInit) return;

    _date = date;
    _sep = sep;

    _logFile = new QFile;
    _logFile->setFileName(path);
    if (!_logFile->open(QIODevice::Append | QIODevice::Text))
        qDebug() << "[ERROR] Can not open file " << _logFile->errorString();

    _logFile->resize(0);  // 将文件的大小调整为0字节。如果 _logFile 已经关联到一个实际的文件，这会导致该文件的内容被清空，即删除所有已写入的数据

    // 自定义日志消息的处理方式，比如将日志发送到网络或格式化输出，这里告诉 Qt 所有log将使用我们的自定义函数处理
    qInstallMessageHandler(Logger::msgOutput);

    _isInit = true;
}

/**
 * @brief Logger::msgOutput 我们自定义的日志处理函数
 * @param type
 * @param context
 * @param msg
 */
void Logger::msgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream stream(_logFile);
    stream <<         QDateTime::currentDateTime().toString(_date)
           << _sep << _contextName.value(type)
           << _sep << QString(context.file)  // 文件名
           << _sep << QString(context.function).section('(', -2, -2).section(' ', -1).section(':', -1)  // 函数名
           << _sep << QString::number(context.line)  // 行号
           << _sep << msg
           << "\n";

    _logFile->flush();  // 待写数据从缓冲区写入文件
}


void Logger::clean()
{
    if (_logFile != nullptr)
    {
        _logFile->close();
        delete _logFile;
    }
}
