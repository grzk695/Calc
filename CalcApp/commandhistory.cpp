#include "commandhistory.h"

CommandHistory::CommandHistory(QWidget *parent)
	: QTreeWidget(parent)
{
	prevItem = nullptr;
	prevCommandItem = nullptr;
	AppData::loadCommandHistory(this);
	dateInserted = false;
	insertCurrentDate();
	expandAll();
	scrollToBottom();
}

CommandHistory::~CommandHistory()
{
}

void CommandHistory::insertCurrentDate()
{
	insertTopLevel("%-- " + QDateTime::currentDateTime().toString() + " --%");
}

void CommandHistory::insertTopLevel(const QString &str)
{
	prevItem = new QTreeWidgetItem(QStringList(QString(str)));
	addTopLevelItem(prevItem);
	//	AppData::appendToCommandHistory(str);
}

void CommandHistory::insertCommand(QString str)
{
	if (prevItem == nullptr)
		return;

	if (prevCommandItem != nullptr && prevCommandItem->text(0) == str)
		return;

	prevCommandItem = new QTreeWidgetItem(QStringList(QString(str)));
	prevItem->addChild(prevCommandItem);
	scrollToBottom();

	if (!dateInserted)
	{
		AppData::appendToCommandHistory(prevItem->text(0));
		dateInserted = true;
	}
	AppData::appendToCommandHistory(str);
}

void CommandHistory::onItemDoubleClicked(QTreeWidgetItem *item, int idx)
{
	emit executeCommand(item->text(0));
}