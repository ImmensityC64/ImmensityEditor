#ifndef NEW_DIALOG_H
#define NEW_DIALOG_H

#include "mainwindow.h"
#include "map_general.h"
#include "ui_new_dialog.h"
#include <QMessageBox>

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QVector<VE *> *vector,
                       QString *name,
                       int *copy,
                       QWidget *parent = 0) :
        QDialog(parent), vector(vector), name(name), copy(copy),
        ui(new Ui::NewDialog)
    {
        ui->setupUi(this);
        setWindowTitle("Create New");

        ui->copy->addItem(QString(NewDialogNoCopyString), QVariant(-1));

        int size = vector->size();
        for(int i=0; i<size; i++)
            if(vector->at(i))
                ui->copy->addItem(vector->at(i)->name, QVariant(i));

        connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
        connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(verifyAndClose()));

        ui->name->setFocus();

        *name = "";
        *copy = -1;
    }
    ~NewDialog()
    {
        delete ui;
    }

private:
    Ui::NewDialog *ui;
    QVector<VE *> *vector;
    QString *name;
    int *copy;

private slots:
    void verifyAndClose()
    {
        QString tName = ui->name->text();

        if(tName.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("Name field is empty!");
            msgBox.exec();
            ui->name->setFocus();
            return;
        }

        int size = vector->size();
        for(int i=0; i<size; i++)
        {
            if(tName == vector->at(i)->name)
            {
                QMessageBox msgBox;
                msgBox.setText("That name is already in use.\nName must be unique.\nPlease, change it!");
                msgBox.exec();
                ui->name->setFocus();
                return;
            }
        }

        *name = tName;
        *copy = ui->copy->currentData().value<int>();

        close();
    }
};

#endif // NEW_DIALOG_H
