#include "variableseditor.h"

VariablesEditor::VariablesEditor(InterpreterConnector *interpreter,QWidget *parent)
	: QWidget(parent)
{
	interpreterConnector = interpreter;
	ui.setupUi(this);
	setWindowFlags(Qt::Window);

	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onCurrentTabChanged(int)));
}

VariablesEditor::~VariablesEditor()
{

}

void VariablesEditor::connectToInterpretSingals()
{
	PR::SignalEmitter::get()->connect_updated_variables_slot(boost::bind(&VariablesEditor::receiveVarsUpdate, this, _1, _2));
}

void VariablesEditor::onVariableSelection(QTableWidgetItem *item)
{
	if (item == nullptr || item->column() != 0)
		return;
	emit variableInformationRequest(item->text());
}

void VariablesEditor::receiveVariableInformation(PR::VariableInfo info)
{
	auto tabs = ui.tabWidget;
	
	QWidget *widget = findTab(info.getName().c_str());
	int idx;
	if (widget == nullptr)
	{
		widget = new VariableEditWidget(ui.tabWidget);
		ui.tabWidget->addTab(widget, info.getName().c_str());
		connect(widget, SIGNAL(notifyVariableUpdate(QString)), interpreterConnector, SLOT(commandToInterpreter(QString)));
	}



	idx = ui.tabWidget->indexOf(widget);
	ui.tabWidget->blockSignals(true);
	ui.tabWidget->setCurrentIndex(idx);
	ui.tabWidget->blockSignals(false);
	show();
	
	dynamic_cast<VariableEditWidget *>(widget)->loadWidget(info);
}

void VariablesEditor::receiveVarsUpdate(const PR::VariableInfo *data, int num)
{
	using PR::VariableInfo;
	for (int i = 0; i < num; i++)
	{
		const VariableInfo *variable = data + i;
		VariableEditWidget *edit = dynamic_cast<VariableEditWidget *>(findTab(variable->getName().c_str()));

		if (edit == nullptr)
			continue;

		edit->setUpdate(true);
		if (edit->isVisibleTo(this))
			edit->loadWidget(*variable,true);
	}
}

QWidget* VariablesEditor::findTab(const QString &name)
{
	int tabs_number = ui.tabWidget->count();
	for (int i=0; i < tabs_number; i++)
	{
		if (ui.tabWidget->tabText(i) == name)
			return ui.tabWidget->widget(i);
	}

	return nullptr;
}

void VariablesEditor::onCurrentTabChanged(int idx)
{
	VariableEditWidget *widget = dynamic_cast<VariableEditWidget*>(ui.tabWidget->widget(idx));
	if (widget->needUpdate())
		emit variableInformationRequest(ui.tabWidget->tabText(idx));
}