#include "mainwindow.h"
#include "map_settings.h"
#include "ui_map_settings.h"

void MainWindow::settingsClicked()
{
    MapSettings dialog(props, map_index, this);
    connect(&dialog, SIGNAL(mapSettingsChanged()), this, SLOT(mapSettingsChanged()));
    dialog.exec();
}

MapSettings::MapSettings(Props &props,
                         int map_index,
                         QWidget *parent) :
    QDialog(parent),
    props(props),
    map_index(map_index),
    ui(new Ui::MapSettings)
{
    ui->setupUi(this);
    init();
}

MapSettings::~MapSettings()
{
    delete ui;
}

void MapSettings::init()
{
    initDropBox();

    setWindowTitle("Map Settings");

    ui->name->setText(props.maps.at(map_index)->name);

    human_comm0_index = props.maps.at(map_index)->human_comm0_index;
    human_comm1_index = props.maps.at(map_index)->human_comm1_index;
    cyber_comm0_index = props.maps.at(map_index)->cyber_comm0_index;
    cyber_comm1_index = props.maps.at(map_index)->cyber_comm1_index;

    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(btnOkClicked()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->humanComm0, SIGNAL(sendColor(quint8)), this, SLOT(receiveHumanComm0(quint8)));
    connect(ui->humanComm1, SIGNAL(sendColor(quint8)), this, SLOT(receiveHumanComm1(quint8)));
    connect(ui->cyberComm0, SIGNAL(sendColor(quint8)), this, SLOT(receiveCyberComm0(quint8)));
    connect(ui->cyberComm1, SIGNAL(sendColor(quint8)), this, SLOT(receiveCyberComm1(quint8)));

    refreshColorButtons();
}

void MapSettings::initDropBox()
{
    for(int i=0; i<props.sceneries.size(); i++)
        ui->scenery->addItem(props.sceneries.at(i)->name);
    ui->scenery->setCurrentIndex(props.maps.at(map_index)->scenery_index);

    for(int i=0; i<props.themes.size(); i++)
        ui->theme->addItem(props.themes.at(i)->name);
    ui->theme->setCurrentIndex(props.maps.at(map_index)->theme_index);

    for(int i=0; i<props.humans.size(); i++)
        ui->human->addItem(props.humans.at(i)->name);
    ui->human->setCurrentIndex(props.maps.at(map_index)->human_index);

    for(int i=0; i<props.cybers.size(); i++)
        ui->cyber->addItem(props.cybers.at(i)->name);
    ui->cyber->setCurrentIndex(props.maps.at(map_index)->cyber_index);

    for(int i=0; i<props.musics.size(); i++)
        ui->music->addItem(props.musics.at(i)->name);
    ui->music->setCurrentIndex(props.maps.at(map_index)->music_index);
}

void MapSettings::btnOkClicked()
{
    QString name = ui->name->text();

    if(name.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Name field is empty!");
        msgBox.exec();
        ui->name->setFocus();
        return;
    }

    int size = props.maps.size();

    /* verify that no other map has the same name */
    for(int i=0; i<size; i++)
    {
        if(i == map_index) continue; // skip myself
        if(nullptr == props.maps.at(i)) continue; // skip deleted

        if(name == props.maps.at(i)->name)
        {
            QMessageBox msgBox;
            msgBox.setText("A map has already been created with this name.\nName must be unique.\nPlease, change it!");
            msgBox.exec();
            ui->name->setFocus();
            return;
        }
    }

    Map *tMap = props.maps.data()[map_index];

    int scenery_index = ui->scenery->currentIndex();
    int   theme_index = ui->theme->currentIndex();
    int   human_index = ui->human->currentIndex();
    int   cyber_index = ui->cyber->currentIndex();
    int   music_index = ui->music->currentIndex();

    tMap->name = name;
    tMap->scenery_index = scenery_index;
    tMap->theme_index   = theme_index;
    tMap->human_index   = human_index;
    tMap->cyber_index   = cyber_index;
    tMap->music_index   = music_index;

    tMap->human_comm0_index = human_comm0_index;
    tMap->human_comm1_index = human_comm1_index;
    tMap->cyber_comm0_index = cyber_comm0_index;
    tMap->cyber_comm1_index = cyber_comm1_index;

    emit mapSettingsChanged();
    close();
}

void MapSettings::receiveHumanComm0(quint8 i)
{
    human_comm0_index = i;
    ui->humanComm0->buttonColor(human_comm0_index);
}

void MapSettings::receiveHumanComm1(quint8 i)
{
    human_comm1_index = i;
    ui->humanComm1->buttonColor(human_comm1_index);
}

void MapSettings::receiveCyberComm0(quint8 i)
{
    cyber_comm0_index = i;
    ui->cyberComm0->buttonColor(cyber_comm0_index);
}

void MapSettings::receiveCyberComm1(quint8 i)
{
    cyber_comm1_index = i;
    ui->cyberComm1->buttonColor(cyber_comm1_index);
}

void MapSettings::refreshColorButtons(void)
{
    ui->humanComm0->buttonColor(human_comm0_index);
    ui->humanComm1->buttonColor(human_comm1_index);
    ui->cyberComm0->buttonColor(cyber_comm0_index);
    ui->cyberComm1->buttonColor(cyber_comm1_index);
}
