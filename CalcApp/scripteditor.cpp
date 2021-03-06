#include "scripteditor.h"

int ScriptEditor::i = 0;

ScriptEditor::ScriptEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tabWidget->setTabsClosable(true);
	ui.actionNew->setShortcut(QKeySequence::New);
	ui.actionOtw_Rz->setShortcut(QKeySequence::Open);
	ui.actionSave->setShortcut(QKeySequence::Save);
	ui.actionSave_as->setShortcut(QKeySequence::SaveAs);
	ui.actionRun->setShortcut(QKeySequence::Refresh);

	setWindowIcon(QIcon(":/CalcApp/script_edit.png"));
	setWindowTitle("KLab - Script editor");

	connect(ui.actionSave_as, SIGNAL(triggered()), this, SLOT(onSaveAsAction()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSaveAction()));
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(onNewFileAction()));
	connect(ui.actionOtw_Rz, SIGNAL(triggered()), this, SLOT(onOpenAction()));
	connect(ui.actionRun, SIGNAL(triggered()), this, SLOT(onRunAction()));
	connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	setupToolbar();
	statusBar()->showMessage("Ln: 1, Col: 1");
}

ScriptEditor::~ScriptEditor()
{
}

void ScriptEditor::closeTab(int idx)
{
	ScriptEditWidget *widget = static_cast<ScriptEditWidget*>(ui.tabWidget->widget(idx));
	if (!widget->isUpdated())
	{
		removeTab(idx);
		return;
	}
	prepereSaveDialog(widget);
}

void ScriptEditor::removeTab(int idx)
{
	ui.tabWidget->removeTab(idx);
	if (ui.tabWidget->count() == 0)
		hide();
}

void ScriptEditor::prepereSaveDialog(ScriptEditWidget *widget)
{
	QMessageBox msg;
	msg.setText("The script has been modified.");
	msg.setInformativeText("Do you want to save changes in \"" + ui.tabWidget->tabText(ui.tabWidget->indexOf(widget)) + "\"?");
	msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	int ret = msg.exec();
	switch (ret)
	{
	case QMessageBox::Save:
		widget->saveToFile();
		break;
	case QMessageBox::Discard:
		removeTab(ui.tabWidget->indexOf(widget));
		break;
	}
}

bool ScriptEditor::close()
{
	bool _save_all = false;
	for (int i = 0; i < ui.tabWidget->count(); i++)
	{
		auto *widget = static_cast<ScriptEditWidget*>(ui.tabWidget->widget(i));
		if (!widget->isUpdated())
			continue;
		if (!_save_all)
		{
			QMessageBox msg;
			msg.setText("The script has been modified.");
			msg.setInformativeText("Do you want to save changes in \"" + ui.tabWidget->tabText(ui.tabWidget->indexOf(widget)) + "\"?");
			msg.setStandardButtons(QMessageBox::Yes | QMessageBox::YesAll | QMessageBox::No | QMessageBox::NoToAll | QMessageBox::Cancel);
			int ret = msg.exec();
			switch (ret)
			{
			case QMessageBox::Yes:
				widget->saveToFile();
				break;
			case QMessageBox::YesAll:
				_save_all = true;
				widget->saveToFile();
				break;
			case QMessageBox::NoAll:
				return QMainWindow::close();
			case QMessageBox::Cancel:
				return false;
			}
		}
		else
			widget->saveToFile();
	}
	return QMainWindow::close();
}

void ScriptEditor::addTab(QString pathArg)
{
	QString fileName = QDir(pathArg).dirName();
	ScriptEditWidget *widget = new ScriptEditWidget(ui.tabWidget);
	widget->setFilePath(pathArg);
	widget->readFromFile();
	connect(widget, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
	connect(widget, SIGNAL(fileSaved()), this, SLOT(onChangesSaved()));
	connect(widget, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
	if (fileName == ".")
		fileName = "Untitled" + (i++ ? QString::number(i) : "");

	ui.tabWidget->addTab(widget, fileName);
	ui.tabWidget->setCurrentWidget(widget);
}

void ScriptEditor::cursorChanged()
{
	auto *widget = static_cast<ScriptEditWidget*>(QObject::sender());
	QTextCursor q = widget->textCursor();
	statusBar()->showMessage("Ln: " + QString::number(q.blockNumber() + 1) + ", Col: " + QString::number(q.columnNumber() + 1));
}

void ScriptEditor::onTextChanged()
{
	QObject *sender = QObject::sender();
	ScriptEditWidget *widget = dynamic_cast<ScriptEditWidget *>(sender);
	if (widget == nullptr)
		return;
	widget->setUpdated(true);
	int idx = ui.tabWidget->indexOf(widget);
	ui.tabWidget->setTabIcon(idx, QIcon(":/CalcApp/dot.png"));
}

void ScriptEditor::workingDirectoryChanged(QString newWorkingDirectory)
{
	ScriptEditWidget::setWorkingDirectory(newWorkingDirectory);
}

void ScriptEditor::onChangesSaved()
{
	ScriptEditWidget *widget = dynamic_cast<ScriptEditWidget *>(QObject::sender());
	if (widget == nullptr)
		return;
	ui.tabWidget->setTabIcon(ui.tabWidget->indexOf(widget), QIcon());
	ui.tabWidget->setTabText(ui.tabWidget->indexOf(widget), QDir(widget->getFilePath()).dirName());
}

void ScriptEditor::onScriptDblClicked(QListWidgetItem *item)
{
	QString scriptName = ScriptEditWidget::getWorkingDirectory() + "/" + item->text();
	int N = ui.tabWidget->count();
	for (int i = 0; i < N; i++)
	{
		ScriptEditWidget *widget = dynamic_cast<ScriptEditWidget*>(ui.tabWidget->widget(i));
		if (widget->getFilePath() == scriptName)
		{
			ui.tabWidget->setCurrentIndex(i);
			hide();
			show();
			return;
		}
	}
	addTab(scriptName);
	hide();
	show();
}

void ScriptEditor::onSaveAsAction()
{
	ScriptEditWidget *widget = dynamic_cast<ScriptEditWidget *>(ui.tabWidget->currentWidget());
	if (widget == nullptr)
		return;

	QString fileName = widget->askForPathToSave();
	if (fileName == "")
		return;
	widget->setUpdated(true);
	widget->setFilePath(fileName);
	widget->saveToFile();
}

void ScriptEditor::onSaveAction()
{
	ScriptEditWidget *widget = dynamic_cast<ScriptEditWidget *>(ui.tabWidget->currentWidget());
	if (widget == nullptr)
		return;
	widget->saveToFile();
}

void ScriptEditor::onNewFileAction()
{
	QString name = "Untitled" + (i++ == 0 ? "" : QString::number(i)) + ".m";
	addTab("");
	hide();
	show();
}

void ScriptEditor::onOpenAction()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select scripts"),
		ScriptEditWidget::getWorkingDirectory(), tr("Script files (*.m)"));

	for (const auto &path : files)
	{
		int count = ui.tabWidget->count();
		bool found = false;
		i = 0;
		for (; i < count; i++)
		{
			ScriptEditWidget *widget = dynamic_cast<ScriptEditWidget*>(ui.tabWidget->widget(i));
			if (widget->getFilePath() == path)
			{
				found = true;
				break;
			}
		}
		if (found)
		{
			ui.tabWidget->setCurrentIndex(i);
			continue;
		}
		addTab(path);
	}
}

void ScriptEditor::onRunAction()
{
	ScriptEditWidget *widget = dynamic_cast<ScriptEditWidget*>(ui.tabWidget->currentWidget());
	widget->saveToFile();
	QString scriptName = QDir(widget->getFilePath()).dirName();
	scriptName = scriptName.left(scriptName.lastIndexOf("."));
	if (scriptName.size())
		emit runCommand(scriptName);
}

void ScriptEditor::setupToolbar()
{
	QPushButton* button = new QPushButton(this);
	button->setIcon(QIcon(":/CalcApp/new.png"));
	button->setToolTip("New script");
	ui.mainToolBar->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(onNewFileAction()));

	button = new QPushButton(this);
	button->setIcon(QIcon(":/CalcApp/save.png"));
	button->setToolTip("Save");
	ui.mainToolBar->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(onSaveAction()));

	button = new QPushButton(this);
	button->setIcon(QIcon(":/CalcApp/run.png"));
	button->setToolTip("Run script");
	ui.mainToolBar->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(onRunAction()));
}