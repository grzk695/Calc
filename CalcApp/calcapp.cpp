#include "calcapp.h"




CalcApp::CalcApp(QWidget *parent)
	: QMainWindow(parent),
	interpreterConnector(new InterpreterConnector()),
	fileWatcher(QDir::currentPath(),this)
{
	ui.setupUi(this);
	interpreterConnector = new InterpreterConnector();
	interpreterConnector->connectStopComputing();
	
	QThread *t = new QThread();
	t->start();
	interpreterConnector->moveToThread(t);
	
	QString temp = QDir::currentPath();
	interpreterConnector->workingDirectoryChanged(temp);
	fileWatcher.addPath(QDir::currentPath());
	ui.dirComboBox->workingDirectoryChanged(temp);


	PR::SignalEmitter::get()->connect_output(boost::bind(&InterpreterConnector::signal_receiver, interpreterConnector, _1, _2));
	PR::SignalEmitter::get()->connect_errors(boost::bind(&InterpreterConnector::errors_receiver, interpreterConnector, _1, _2));

	ui.variables->connectSlots();

	qRegisterMetaType<QString>("QString");
	qRegisterMetaType<std::string>("std::string");
	
//	ui.commandL->setFocus();
	connect(ui.commandLine, SIGNAL(commandEntered(const QString &)), interpreterConnector, SLOT(commandToInterpreter(const QString &)));
	connect(ui.commandLine, SIGNAL(commandEntered(const QString &)), ui.console, SLOT(appendWithoutRealase(const QString &)));
	connect(ui.commandLine, SIGNAL(commandEntered(const QString &)), ui.commandHistory, SLOT(insertCommand(const QString &)));
	connect(ui.commandHistory, SIGNAL(executeCommand(const QString &)), ui.console,(SLOT(appendWithoutRealase(const QString &))));
	connect(ui.commandHistory, SIGNAL(executeCommand(const QString &)), interpreterConnector, SLOT(commandToInterpreter(const QString &)));
	connect(ui.commandHistory, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), ui.commandHistory, SLOT(onItemDoubleClicked(QTreeWidgetItem *, int)));
	connect(interpreterConnector, SIGNAL(interpreterResponded(const QString&)), ui.console, SLOT(append(const QString&)));
	connect(interpreterConnector, SIGNAL(interpreterRespondedHtml(const QString&)), ui.console, SLOT(insertHtml(const QString&)));
	connect(interpreterConnector, SIGNAL(interpreterError(const QString &)), ui.console, SLOT(insertHtml(const QString&)));
	connect(&fileWatcher, SIGNAL(directoryChanged(const QString&)), &fileWatcher, SLOT(changed(const QString &)));
	connect(&fileWatcher, SIGNAL(sendFileList(const QStringList &)), ui.filesList, SLOT(set(const QStringList &)));
	connect(&fileWatcher, SIGNAL(fileUpdated(const QString&)), interpreterConnector, SLOT(updateFile(const QString &)));
	connect(ui.workingDirChangeButton, SIGNAL(clicked()), &fileWatcher, SLOT(fileDialogButtonClicked()));
	connect(&fileWatcher, SIGNAL(workingDirectoryChanged(const QString &)), interpreterConnector, SLOT(workingDirectoryChanged(const QString &)));
	connect(&fileWatcher, SIGNAL(workingDirectoryChanged(const QString &)), ui.dirComboBox, SLOT(workingDirectoryChanged(const QString &)));
	connect(ui.dirComboBox, SIGNAL(currentIndexChanged(const QString &)), &fileWatcher, SLOT(setNewDirectory(const QString &)));
	fileWatcher.changed(QDir::currentPath());
}

CalcApp::~CalcApp()
{

}

