#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
class QFileInfo;

class FileUtils : public QObject
{
    Q_OBJECT

public:
    /**
      *constructor
      */
    FileUtils(QObject *parent = 0);

    /**
     * \enum    Operation
     *
     * \brief   Options to use for navigation between files
     */
    enum Operation{Next,        ///<Goes to the next file in folder
                   Previous,    ///<Goes to the previous file infolder
                   Last,        ///<Goes to the last file in folder
                   First,       ///<Goes to the first file in folder
                   UserDefined  ///<Goes to the file hte user pass
                  };

    /**
     * \enum    Order
     *
     * \brief   Available order styles for the files list
     */
    enum Order{Name, ///<By name
               Type, ///<By type (extension)
               Size, ///<By size
               Time  ///<By time (modification time)
              };

    /**
      * Sets the file that the FileUtils provides tools about to \a file.
      * \returns true if succes, otherwise \returns false
      */
    bool openFile(const QString fileName);

    /**
      * deletes the current file
      * \returns true if succes, otherwise returns false
      */
    bool deleteFile();

    /**
      * moves the current file to the new \a path, including
      * a new filename
      */
    bool moveFile(const QString path);

    /**
      * resetes everythong
      */
    void clear();

    /**
      * @returns the complete file path
      * \see getFilePath()
      * \see getFileName()
      */
    inline QString absoluteFilePath() const{return completeFileName;}

    /**
      * @return the path of the current file
      * \see absoluteFilePath()
      * \see getFileName()
      */
    inline QString getFilePath() const{return currentFilePath;}

    /**
      * @return the file name
      * \see absoluteFilePath()
      * \see getFilePath()
      */
    inline QString getFileName() const{return currentFileName;}

    /**
      * @returns the position of the current file in the list, being 0 the first one
      * and \fn getFilesAmount() - 1 the last
      */
    inline int getCurrentPosition() const{return currentPosition;}

    /**
      * @returns the amount of images in the folder
      */
    inline int getFilesAmount() const{return files.count();}

    /**
      *sets \a filters as the name filters to explore the folder
      */
    void setNameFilters(const QStringList filters);

    /**
      * @returns the fileName of the asked image in the folder
      *if \a is UseDefined, then retunr the image in the \a position
      *in the folder
      */
    bool goToFile(Operation operation, int position=-1);

    /**
      * @return true if the file exists, otherwise returns false
      */
    inline bool exists() const{return exist;}

    /**
      *explores the given \a folder
      * \returns true if the folder contains valid files, otherwise return false
      */
    bool openFolder(const QString folder);

    /**
      *sets \a sort as the sorting way to use
      * \see getSorting()
      */
    void setSorting(Order sort);

    /**
      * \returns the files sorting, as an int value, you need to convert it
      * to QDir::SortFlag
      * \see setSorting(QDir::SortFlag sort)
      */
    inline int getSorting() const{return sortBy;}

    /**
      \returns the list with the files of the folder, ordered.
      */
    inline QStringList getFilesList() const{return files;}

    /*!
     * \brief getSubFolders
     * \return all the sub folders in the current folder, without the "." and ".."
     */
    inline QStringList getSubFolders() const{return subFolders;}

    /*!
      Goes to the file given by \a name, or if it's empty,
      then go to the file in the position \a position
      */
    bool goToFile(const QString name="", const int position=-1);

signals:
    /*!
      signal emitted when the current work path changes
      */
    void filePathChanged(QString newPath);

private:
    /*!
      this is the name of the current file
      i.e. image.png
      */
    QString currentFileName;

    /*!
      this is the path to the current file
      i.e. /home/USER/Desktop(
      */
    QString currentFilePath;

    /*!
      file name + file path
      i.e. /home/USER/Desktop/image.png
      */
    QString completeFileName;

    /*!
      list with all the files (file name) in the current path
      */
    QStringList files;

    /*!
     * \brief subFolders
     */
    QStringList subFolders;

    /*!
      name filters used to explore the folder
      */
    QStringList nameFilters;

    /*!
      order i'll use to sort the files in the folder
      */
    Order sortBy;

    /*!
      position of the current file in the list
      */
    int currentPosition;

    /*!
      i use it to know if the file it's being openned exist
      */
    bool exist;

    /*!
      boolean that stores if the folder has alredy been explored
      or not, and in consequence, a file
      */
    bool dirLoaded;

    /*!
      boolean to know if the folder opened is diferent to the
      current one or not
      */
    bool newFolder;

    bool emptyDir;

    /*!
      sets the current file name, path and the complete file name
      */
    void setFileInfo(const QString name);

    int sortFalg();

    bool sortAndGetFiles(QString path);

    void setCurrentPosition();

    void reload();

    /**
      *goes to image passed by \a operation
      *if \a operation is \a UserDefined, then goes to image in
      *\a position
      */
    bool goTo(Operation operation, int position=-1);

};

#endif // FILEUTILS_H
