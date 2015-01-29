#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>
#include "ui_gotodialog.h"

class GoToDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * constructor
     */
    GoToDialog(QWidget *parent = 0);

    /*!
     * sets \a list as the list to use for the autocompletion
     */
    void setList(QStringList list);

    /*!
     * sets the range(\a min, \a max) to use in the spin box, and sets it to \a pos
     * */
    void setRange(int min, int max, int pos);

signals:
    /*!
     * signal emited when clicking on go button, it send the \a name selected
     * and the \a postion
     */
    void goTo(QString name, int pos);

private:
    Ui_goToDialog ui;

    //a completer for the QLineEdit object
    QCompleter *completer;

    //a list qith the files so the completer knows what to recomend
    QStringList list;

    //stores old position
    int oldPos;

private slots:
    void goSlot();
    void cancelSlot();
    void spinSlot(int);
    void nameLineSlot(QString);

};

#endif // GOTODIALOG_H
