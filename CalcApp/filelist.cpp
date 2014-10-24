#include "filelist.h"

FileList::FileList(QWidget *parent)
	: QListWidget(parent)
{

}

FileList::~FileList()
{

}
void FileList::set(const QStringList &list)
{
	clear();
	insertItems(0, list);
}