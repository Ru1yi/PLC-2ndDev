#include "Model.h"
#pragma execution_character_set("utf-8")


Model* GlobalModel;


Model::Model()
{
	modelLog = new QStringListModel();

	QStringList list = QStringList();

	modelError = new QStandardItemModel(0, 3);
	modelError->setHorizontalHeaderLabels(QStringList() << "����ʱ��" << "�����ȼ�"<< "��������");

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
/// ��ӱ��������Զ������־
/// </summary>
/// <param name="text"></param>
/// <param name="level"></param>
void Model::AddError(QString text, ErrorLevel level)
{
	mutexError.lock();
	//����������˱�������Ӵ˱���
	if (errorList.contains(text) == false)
	{
		QString time = QTime::currentTime().toString("hh:mm:ss:zzz");
		QList<QStandardItem*> item;

		QString lvl = "Others";
		switch (level)
		{
		case ErrorLevel::inf:
			lvl = "��ʾ";
			break;
		case ErrorLevel::warning:
			lvl = "����";
			break;
		case ErrorLevel::error:
			lvl = "����";
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
	// ��ȡ��ǰʱ���ַ���  
	QDir adir(dir);
	if (adir.exists() == false)
	{
		adir.mkpath(dir);
	}

	QString filePath = dir + "Log.ini";

	// ����־�ļ�������������򴴽�  
	QFile logFile(filePath);
	if (!logFile.open(QIODevice::Append | QIODevice::Text))
		return;

	// �����ı�����д����־  
	QTextStream out(&logFile);
	out << message;
}
