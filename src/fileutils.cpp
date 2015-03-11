#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include "fileutils.h"

FileUtils::FileUtils(QObject *parent) :
    QObject(parent)
{
    exist = false;

    //the directory hasn't been explored since no file has been setted
    dirLoaded = false;

    emptyDir = true;
    sortBy = FileUtils::Name;

}

bool FileUtils::openFile(const QString fileName)
{
    //no path loaded yet
    dirLoaded = false;

    //qDebug()<<"open file:"<<fileName;

    //check if the new file name is valid
    if(fileName.isEmpty() || fileName.isNull()){
        qDebug()<<tr("Empty file name, i can't do anything.");
        return false;
    }

    //check if the file exists
    if(!QFile(fileName).exists()){
        return false;
    }

    //check if the file path if the same than the older
    if(QFileInfo(fileName) != currentFilePath)
    {
        emit filePathChanged(QFileInfo(fileName).absolutePath());
    }

    //clear everything
    this->clear();

    //set the name, complete and full file path
    this->setFileInfo(fileName);

    //explore the folder
    bool d = this->sortAndGetFiles(currentFilePath);
    if(d){//if succes exploring folder
        dirLoaded = true;

        //get the position of the file opened in the files list
        this->setCurrentPosition();
    }

    return true;
}

bool FileUtils::openFolder(const QString folder)
{
    //no dir loaded yet
    dirLoaded = false;
    //qDebug()<<"open folder:"<<folder;

    //check if the folder string is empty or null
    if(folder.isEmpty() || folder.isNull()) {
        qDebug()<<tr("Empty path, i can't do anything.");
        return false;
    }

    //check if it exists in the system
    else if(!QDir(folder).exists()){
        return false;
    }

    //check that it isn't the same than the older
    else if(QDir(folder).absolutePath() == currentFilePath){
        qDebug()<<tr("The folder is the same than the current.");
        return false;
    }

    //if got here, then clear everything, fill the list and, if no file was opened but
    //a folder, then open the first file in the list
    else{
        this->clear();//clear
        bool d = sortAndGetFiles(QDir(folder).absolutePath());//fill
        if(d){//open first file
            currentPosition = 0;
            currentFileName = files.at(0);
            currentFilePath = QDir(folder).absolutePath();
            completeFileName = currentFilePath + QDir::separator() + currentFileName;
            dirLoaded = true;
            emit filePathChanged(currentFilePath);
        }
    }

    return true;
}

void FileUtils::reload()
{
    //fill the list
    this->sortAndGetFiles(currentFilePath);

    //get the new position of the current file
    this->setCurrentPosition();
}

bool FileUtils::sortAndGetFiles(QString path)
{
    //clear the list of files
    files.clear();

    //explore the path and fill the file list
    QDir dir(path);
    files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::SortFlags(sortFalg()));
    subFolders = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::NoSort);

    return !files.isEmpty();
}

void FileUtils::setSorting(Order sort)
{
    sortBy = sort;

    //if the folder has been explored, re explore it, else, do nothing
    if(dirLoaded){
        this->reload();
    }
}

void FileUtils::setCurrentPosition()
{
    //check if the list ain't empty
    if(files.isEmpty()){
        return;
    }

    //get the position
    currentPosition = files.indexOf(currentFileName);
}

bool FileUtils::goTo(Operation operation, int position)
{
    //get the amount of files
    int amountOfFiles = files.size();

    //if there's only one file, get out
    if(amountOfFiles == 1){
      qDebug()<<"only one file in folder";
      return false;
    }

    //temporal string to store the new file name
    QString temp;
    switch(operation){
    case Next:
        if((currentPosition + 1) == amountOfFiles){
          currentPosition = 0;
          temp=files.at(0);
        }
        else{
          temp=files.at(currentPosition + 1);
          currentPosition += 1;
        }
        break;
    case Previous:
        if((currentPosition) == 0){
          temp=files.at(amountOfFiles - 1);
          currentPosition = amountOfFiles - 1;
        }
        else{
          temp=files.at(currentPosition - 1);
          currentPosition -= 1;
        }
        break;
    case Last:
        if((currentPosition + 1) == amountOfFiles){
	  qDebug()<<"this is already the last one";
          return false;
        }
        else{
          temp=files.at(amountOfFiles - 1);
          currentPosition = amountOfFiles - 1;
        }
        break;
    case First:
        if(currentPosition == 0){
	  qDebug()<<"this is already the first one";
          return false;
        }
        else{
          temp=files.at(0);
          currentPosition = 0;
        }
        break;
    case UserDefined:
        if(position == currentPosition){
            return false;
        }
        if(position < 0 || position == amountOfFiles){
            return false;
        }
        else{
            temp = files.at(position);
            currentPosition = position;
        }
        break;
    }

    //set the new file as current
    currentFileName = temp;
    temp.prepend(currentFilePath + "/");
    completeFileName = temp;

    return true;
}

void FileUtils::setNameFilters(const QStringList filters)
{
    nameFilters = filters;
}

void FileUtils::clear()
{
    currentFileName.clear();
    currentFilePath.clear();
    completeFileName.clear();
    files.clear();
    currentPosition = 0;
    exist = false;
    dirLoaded = false;
    emptyDir = true;
}

bool FileUtils::goToFile(Operation operation, int position)
{
    return goTo(operation,position);
}

void FileUtils::setFileInfo(const QString name)
{
    QFileInfo f(name);
    currentFileName = f.fileName();
    currentFilePath = f.absolutePath();
    completeFileName = f.absoluteFilePath();
    f.~QFileInfo();
}

bool FileUtils::moveFile(const QString path)
{
    //check if the new folder is the same than the current one
    if(currentFilePath == QDir(path).path()){
        qDebug()<<tr("The image wasn't moved.\nThe new folder is iqual to the older.");
        newFolder = false;
        return false;
    }

    //the new file name
    QString newFileName = path + QDir::separator() + getFileName();

    //check if there is an existing file with the same name
    if(QFile(newFileName).exists()){
        //ask if wish to overwrite
        int result = QMessageBox::question(0, tr("Overwrite?"),
                    tr("File %1 already exists. Do you want to overwrite it?").arg(getFileName()),
                    QMessageBox::Yes|QMessageBox::No);

        //if doesn't want return;
        if (result == QMessageBox::No){
            return false;
        }

        //else remove the previous file
        QFile::remove(newFileName);
    }

    //try to move the file
    bool d = QFile(absoluteFilePath()).rename(newFileName);

    //if succes open it
    if(d){
        this->openFile(newFileName);
    }

    //return the result
    return d;
}

bool FileUtils::deleteFile()
{
    bool d = false;

    //try to delete the file
    d = QFile::remove(completeFileName);

    //if fail, get out
    if(!d){
        return d;
    }

    //remove it from the list
    files.removeAt(currentPosition);

    //if the list got empty, then clear the names
    if(files.isEmpty()){
        currentFileName.clear();
        currentFilePath.clear();
        completeFileName.clear();
        emptyDir = true;
    }
    else{
        //go to the file in the position of the deleted one
        currentPosition--;
        goTo(FileUtils::Next);
    }

    return d;
}

bool FileUtils::goToFile(const QString name, const int position)
{
    /*first search by name, and if name is empty os isn't in the list, then use the
      given position*/
    int gtPos;

    if(files.contains(name)){
        gtPos = files.lastIndexOf(name);
    }
    else{
        gtPos = position;
    }

    return goTo(FileUtils::UserDefined, gtPos);
}

int FileUtils::sortFalg()
{
    QDir::SortFlags f;
    switch(sortBy){
    case Name:
        f = QDir::Name|QDir::IgnoreCase;
        break;
    case Type:
        f = QDir::Type;
        break;
    case Time:
        f = QDir::Time;
        break;
    case Size:
        f = QDir::Size;
        break;
    default:
        f = QDir::Name|QDir::IgnoreCase;
        break;
    }

    return int(f);
}
