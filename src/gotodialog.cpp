#include <QtGui/QCompleter>
#include "gotodialog.h"

GoToDialog::GoToDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.nameLine->setEnabled(false);
    ui.positionLine->setEnabled(false);

    ui.cancelButton->setIcon(QIcon::fromTheme("dialog-cancel"));
    ui.goButton->setIcon(QIcon::fromTheme("dialog-ok"));

    connect(ui.goButton, SIGNAL(clicked()), this, SLOT(goSlot()));
    connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));
    connect(ui.positionLine, SIGNAL(valueChanged(int)), this, SLOT(spinSlot(int)));
    connect(ui.nameLine, SIGNAL(textChanged(QString)), this, SLOT(nameLineSlot(QString)));
}

void GoToDialog::setList(QStringList list)
{
    completer = new QCompleter(list);
    this->list = list;
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui.nameLine->setEnabled(true);
    ui.nameLine->setCompleter(completer);
}

void GoToDialog::setRange(int min, int max, int pos)
{
    oldPos = pos;
    ui.positionLine->setRange(min, max);
    ui.positionLine->setValue(pos + 1);
    ui.positionLine->setEnabled(true);
    ui.nameLine->setText(list.at(pos));
}

void GoToDialog::cancelSlot()
{
    emit goTo("", oldPos);
    this->close();
}

void GoToDialog::goSlot()
{
    emit goTo(ui.nameLine->text(), ui.positionLine->value() - 1);
    this->close();
}

void GoToDialog::nameLineSlot(QString d)
{
    Q_UNUSED(d);
    ui.positionLine->setValue(list.lastIndexOf(ui.nameLine->text()) + 1);
}

void GoToDialog::spinSlot(int d)
{
    ui.nameLine->setText(list.at(d-1));
    emit goTo(list.at(d-1), d-1);
}
