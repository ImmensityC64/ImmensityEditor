#ifndef SCENERY_EDITOR_H
#define SCENERY_EDITOR_H

#include "map_general.h"
#include "mainwindow.h"
#include "new_dialog.h"
#include "ui_scenery_editor.h"

class Props;

namespace Ui {
class SceneryEditor;
}

class SceneryEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit SceneryEditor(QWidget *parent = 0);
    ~SceneryEditor();

private:
    Ui::SceneryEditor *ui;
    int scenery_index;
    Props &props;
    Scenery tmp_scenery;
    void init();
    void thereAreChanges(bool b);
private slots:
    void createNew();
    void apply();
    void reload();
    void selectScenery(int /* unused */);
    void deleteScenery();
    void nameEdited(const QString &/* unused */);
    void enableDeletBtn(bool b);
signals:
    void sceneryChanged(int);
};

#endif // SCENERY_EDITOR_H
