#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QFileSystemWatcher>
#include <qstringlist.h>
#include <qdir.h>
#include <qregexp.h>
#include <qfiledialog.h>

class FileWatcher : public QFileSystemWatcher
{
	Q_OBJECT
private:
	QDir dir;
public:
	FileWatcher(const QString &dir,QObject *parent=0);
	~FileWatcher();

	QFileSystemWatcher mFilesWatcher;
signals:
	void sendFileList(const QStringList &list);
	void fileUpdated(const QString &);
	void workingDirectoryChanged(const QString &);

public slots:
	void changed(const QString & path);
	void fileDialogButtonClicked();
	void changedFile(const QString &path);
	void setNewDirectory(const QString &path);
	
};

#endif // FILEWATCHER_H
