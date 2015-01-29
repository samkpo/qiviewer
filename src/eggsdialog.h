#ifndef EGGSDIALOG_H
#define EGGSDIALOG_H

#include <QDialog>
#include "ui_eggsdialog.h"

class EggsDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
      constructor
      */
    EggsDialog(QWidget *parent = 0);

private:
    Ui_eggsDialog ui;

private slots:
    void saveSettings();
    void loadSettings();

    /**
      * @return \a hour and \a min from the given string
      * @param time string that contains the time
      * @param hour returns the hour
      * @param min returns the min
      */
    void getHour(QString time, int& hour, int& min);

    /**
      * @return a string containing the hour and min
      * @param hour pass the hour
      * @param min pass the minutes
      */
    QString setHour(int hour, int min);

};

#endif // EGGSDIALOG_H
