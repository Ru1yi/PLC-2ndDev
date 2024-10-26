#pragma once
#include <QObject>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTime>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDir>

enum class ErrorLevel {
    inf = 0,
    warning,
    error
};


class Model :
    public QObject
{
    Q_OBJECT

public:
    Model();
    QStringListModel* modelLog;

    //»’÷æ
    void AddLog(QString text);
    void ClrLog();


    QStandardItemModel* modelError;
    QStringList errorList;
    void AddError(QString text, ErrorLevel level = ErrorLevel::inf);
    void ClrAllError();
    void ClrOneError(QString text);

private:
    QMutex mutexLog;
    QMutex mutexError;

    void writeLog(const QString& message);

};

extern Model* GlobalModel;

