#include "Model.h"
#pragma execution_character_set("utf-8")


Model* GlobalModel;


Model::Model()
{
	modelLog = new QStringListModel();

	QStringList list = QStringList();

	modelError = new QStandardItemModel(0, 3);
	modelError->setHorizontalHeaderLabels(QStringList() << "报警时间" << "报警等级"<< "报警内容");

	errorList.clear();
	AddLog("Model is Running");

	
}

void Model::AddLog(QString text)
{
	mutexLog.lock();

	QString time = QTime::currentTime().toString("[hh:mm:ss:zzz]->");
	int index = 0;//modelLog->rowCount();
	modelLog->insertRow(index);
	QModelIndex modindex = modelLog->index(index, 0);
	QString code = time + text;
	modelLog->setData(modindex, code);

	writeLog(code);

	mutexLog.unlock();
}

void Model::ClrLog()
{
	mutexLog.lock();
	modelLog->removeRows(0, modelLog->rowCount());
	mutexLog.unlock();
}


/// <summary>
/// 添加报警，并自动添加日志
/// </summary>
/// <param name="text"></param>
/// <param name="level"></param>
void Model::AddError(QString text, ErrorLevel level)
{
	mutexError.lock();
	//如果不包含此报警就添加此报警
	if (errorList.contains(text) == false)
	{
		QString time = QTime::currentTime().toString("hh:mm:ss:zzz");
		QList<QStandardItem*> item;

		QString lvl = "Others";
		switch (level)
		{
		case ErrorLevel::inf:
			lvl = "提示";
			break;
		case ErrorLevel::warning:
			lvl = "警告";
			break;
		case ErrorLevel::error:
			lvl = "错误";
			break;
		}

		item.append(new QStandardItem(time));
		item.append(new QStandardItem(lvl));
		item.append(new QStandardItem(text));

		modelError->insertRow(0,item);
		errorList.insert(0,text);

		AddLog(QString("level:%0,%1").arg(lvl).arg(text));

	}

	mutexError.unlock();
}

void Model::ClrAllError()
{
	mutexError.lock();
	modelError->removeRows(0, modelError->rowCount());
	errorList.clear();
	mutexError.unlock();
}

void Model::ClrOneError(QString text)
{
	mutexError.lock();
	int index = errorList.indexOf(text);
	if (index >= 0)
	{
		modelError->removeRow(index);
		errorList.removeAt(index);
	}
	mutexError.unlock();
}

void Model::writeLog(const QString& message)
{

	QDateTime time = QDateTime::currentDateTime();

	QString Path = QApplication::applicationDirPath() + "/Report/";
	QString dir = QString("%0%1/%2/%3/").arg(Path).arg(time.date().year()).arg(time.date().month()).arg(time.date().day());
	// 获取当前时间字符串  
	QDir adir(dir);
	if (adir.exists() == false)
	{
		adir.mkpath(dir);
	}

	QString filePath = dir + "Log.ini";

	// 打开日志文件，如果不存在则创建  
	QFile logFile(filePath);
	if (!logFile.open(QIODevice::Append | QIODevice::Text))
		return;

	// 创建文本流并写入日志  
	QTextStream out(&logFile);
	out << message;
}
