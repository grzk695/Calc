#ifndef VARIABLESEDITOR_H
#define VARIABLESEDITOR_H

#include <QWidget>
#include <qtablewidget.h>
#include <qvector.h>

#include "Calc\VariableInfo.h"
#include "Calc\SignalEmitter.h"

#include "ui_variableseditor.h"
#include "VariableEditWidget.h"
#include "interpreterconnector.h"

class VariablesEditor : public QWidget
{
	Q_OBJECT
private:
	vector<QWidget*> to_remove;
public:
	VariablesEditor(QWidget *parent = 0);
	~VariablesEditor();

	void connectToInterpretSingals();
	void setInterpreter(InterpreterConnector *ptr){ interpreterConnector = ptr; }

private:
	Ui::VariablesEditor ui;
	InterpreterConnector *interpreterConnector;
	QWidget* findTab(const QString &name);

	void receiveRemoved(vector<string> removed);
	void receiveVarsUpdate(const KLab::VariableInfo *data, int num);

	public slots:
	void onVariableSelection(QTableWidgetItem *item);
	void receiveVariableInformation(KLab::VariableInfo info);
	void onCurrentTabChanged(int idx);
	void computationComplate();
	void tabClose(int idx);
signals:
	void variableInformationRequest(QString varName);
};

#endif // VARIABLESEDITOR_H
