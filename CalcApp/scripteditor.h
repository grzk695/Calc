#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QMainWindow>
#include <qdir.h>
#include <qlistwidget.h>
#include "ui_scripteditor.h"
#include "scripteditwidget.h"

class ScriptEditor : public QMainWindow
{
	Q_OBJECT

public:
	ScriptEditor(QWidget *parent = 0);
	~ScriptEditor();

	void addTab(QString filePath);

	public slots:
	void onTextChanged();
	void onChangesSaved();
	void workingDirectoryChanged(QString directoryPath);
	void onScriptDblClicked(QListWidgetItem *item);
	
	void onSaveAction();
	void onSaveAsAction();
	void onNewFileAction();
	void onOpenAction();

private:
	Ui::ScriptEditor ui;

	static int i;
};

#endif // SCRIPTEDITOR_H
