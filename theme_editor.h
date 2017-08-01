#ifndef THEME_EDITOR_H
#define THEME_EDITOR_H

#include "map_general.h"
#include "mainwindow.h"
#include "new_dialog.h"
#include "ui_theme_editor.h"

class Props;

namespace Ui {
class ThemeEditor;
}

class ClickableRectItem : public QObject, public  QGraphicsRectItem
{
    Q_OBJECT

public:
    int id;
    explicit ClickableRectItem(int id, int x, int y, int w, int h) :
        QObject(),
        QGraphicsRectItem(x, y, w, h),
        id(id) {}

signals:
    void clicked(int id);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent */* unused */)
    {
        emit clicked(id);
    }
};

class ThemeEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ThemeEditor(QWidget *parent = 0);
    ~ThemeEditor();

    const QVector<QString> bgAreas = {
        "Ceiling Fg Common",    // CeilingFg,
        "Ceiling Fg ECM 0",     // CeilingFgECM0,
        "Ceiling Fg ECM 1",     // CeilingFgECM1,
        "Ceiling Fg ECM 2",     // CeilingFgECM2,
        "Ceiling Fg ECM 3",     // CeilingFgECM3,
        "Ceiling 0 (Nearest)",  // Ceiling0,
        "Ceiling 1",            // Ceiling1,
        "Ceiling 2",            // Ceiling2,
        "Ceiling 3",            // Ceiling3,
        "Ceiling 4",            // Ceiling4,
        "Ceiling 5",            // Ceiling5,
        "Ceiling 6 (Farthest)", // Ceiling6,
        "Background",           // Background,
        "Background Comm 0",    // BackgroundComm0,
        "Background Comm 1",    // BackgroundComm1,
        "Floor 6 (Farthest)",   // Floor6,
        "Floor 5",              // Floor5,
        "Floor 4",              // Floor4,
        "Floor 3",              // Floor3,
        "Floor 2",              // Floor2,
        "Floor 1",              // Floor1,
        "Floor 0 (Nearest)",    // Floor0,
        "Floor Fg Common",      // FloorFg,
        "Floor Fg ECM 0",       // FloorFgECM0,
        "Floor Fg ECM 1",       // FloorFgECM1,
        "Floor Fg ECM 2",       // FloorFgECM2,
        "Floor Fg ECM 3"        // FloorFgECM3
    };

private:
    Ui::ThemeEditor *ui;
    int theme_index;
    Props &props;
    Theme tmp_theme;
    QVector<ClickableRectItem *> scrBgs;
    quint8 received_color;
    void init();
    void initThemeDropBox();
    void initColorDropBox();
    void initScrBgs();
    void refreshScrBgColors();
    void thereAreChanges(bool b);
private slots:
    void createNew();
    void apply();
    void reload();
    void selectTheme(int index);
    void deleteTheme();
    void setAreaColor(int i);
    void nameEdited(const QString &/* unused */);
    void enableDeletBtn(bool b);
public slots:
    void receiveColor(quint8 i);
    void scrBgClicked(int i);
signals:
    void themeChanged(int);
};

#endif // THEME_EDITOR_H
