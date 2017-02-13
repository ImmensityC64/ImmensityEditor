#ifndef MAP_SETTINGS_H
#define MAP_SETTINGS_H

#include <QDialog>
#include "mainwindow.h"
#include "new_dialog.h"

class Props;

namespace Ui {
class MapSettings;
}

class MapSettings : public QDialog
{
    Q_OBJECT

public:
    explicit MapSettings(Props &props,
                         int map_index,
                         QWidget *parent = 0);
    ~MapSettings();

private:
    Ui::MapSettings *ui;
    Props &props;

    quint8 human_comm0_index;
    quint8 human_comm1_index;
    quint8 cyber_comm0_index;
    quint8 cyber_comm1_index;

    int map_index;
    void init();
    void initDropBox();
    void refreshColorButtons();

public slots:
    void receiveHumanComm0(quint8 i);
    void receiveHumanComm1(quint8 i);
    void receiveCyberComm0(quint8 i);
    void receiveCyberComm1(quint8 i);
    void btnOkClicked();
signals:
    void mapSettingsChanged();
};

#endif // MAP_SETTINGS_H
