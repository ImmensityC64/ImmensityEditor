#include "scenery_editor.h"

SceneryEditor::SceneryEditor(Props &props, QWidget *parent) :
    QMainWindow(parent),
    tmp_scenery(),
    props(props),
    scenery_index(0),
    ui(new Ui::SceneryEditor)
{
    ui->setupUi(this);
    init();
}

SceneryEditor::~SceneryEditor()
{
    delete ui;
}

void SceneryEditor::init()
{
    for(int i=0; i<props.sceneries.size(); i++)
        ui->scenery->addItem(props.sceneries.at(i)->name);

    reload();

    setWindowTitle("Scenery Editor");

    connect(ui->newScenery, SIGNAL(clicked()), this, SLOT(createNew()));
    connect(ui->checkDelete, SIGNAL(clicked(bool)), this, SLOT(enableDeletBtn(bool)));
    connect(ui->deleteScenery, SIGNAL(clicked()), this, SLOT(deleteScenery()));
    connect(ui->scenery, SIGNAL(currentIndexChanged(int)), this, SLOT(selectScenery(int)));
    connect(ui->name, SIGNAL(textEdited(QString)), this, SLOT(nameEdited(QString)));
    connect(ui->btnApply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->btnReload, SIGNAL(clicked()), this, SLOT(reload()));
}

void SceneryEditor::enableDeletBtn(bool b) { ui->deleteScenery->setEnabled(b); }

void SceneryEditor::apply()
{
    QString tName = ui->name->text();

    if(tName != props.sceneries.at(scenery_index)->name)
    {
        if(tName.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("Name field is empty!");
            msgBox.exec();
            ui->name->setFocus();
            return;
        }

        /* verify that no other scenery has the same name */
        for(int i=0; i<props.sceneries.size(); i++)
        {
            if(tName == props.sceneries.at(i)->name)
            {
                QMessageBox msgBox;
                msgBox.setText("A scenery has already been created with this name.\nName must be unique.\nPlease, change it!");
                msgBox.exec();
                ui->name->setFocus();
                return;
            }
        }

        tmp_scenery.name = tName;
        ui->scenery->setItemText(scenery_index, tName);
    }

    *props.sceneries.at(scenery_index) = tmp_scenery;
    thereAreChanges(false);
    emit sceneryChanged(scenery_index);
}

void SceneryEditor::reload()
{
    tmp_scenery = *props.sceneries.at(scenery_index);
    ui->name->setText(tmp_scenery.name);
    if(ui->scenery->currentIndex() != scenery_index)
        ui->scenery->setCurrentIndex(scenery_index);
    thereAreChanges(false);
    ui->checkDelete->setChecked(false);
    enableDeletBtn(false);
}

void SceneryEditor::deleteScenery()
{
    if(props.deleteScenery(scenery_index))
        ui->scenery->removeItem(scenery_index); // this will trigger the currentIndexChanged signal
}

void SceneryEditor::thereAreChanges(bool b)
{
    ui->btnApply->setEnabled(b);
    ui->btnReload->setEnabled(b);
    ui->scenery->setEnabled(!b);
    ui->newScenery->setEnabled(!b);
}

void SceneryEditor::selectScenery(int index)
{
    /* Clearing combobox also triggers the signal which is connected to this slot,
     * thus we have to be sure that the combobox is not empty!
     */
    if(0 <= index)
    {
        scenery_index = index;
        reload();
    }
}

void SceneryEditor::nameEdited(const QString &/* unused */) { thereAreChanges(true); }

void SceneryEditor::createNew()
{
    QString name = "";
    int copy = -1;
    NewDialog dialog((QVector<VE *> *) &props.sceneries, &name, &copy, this);
    dialog.exec();

    if(!name.isEmpty())
    {
        Scenery *tScenery;
        if(-1 == copy)
        {
            tScenery = new Scenery();
        }
        else
        {
            tScenery = props.sceneries.at(copy)->copy();
            tScenery->clearUsage();
        }
        tScenery->name = name;
        scenery_index = props.sceneries.size(); // will be the index of last element after append
        props.sceneries.append(tScenery);
        ui->scenery->addItem(name);
        reload();
    }
}
