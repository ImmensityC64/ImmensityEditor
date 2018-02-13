
#include "editor.h"

WallEditor::WallEditor(shared_ptr<GfxData> init, int index, QWidget *parent) :
    Editor(init, index, parent),
    wui(new Ui::WallEditorWidget)
{
    spec = new QWidget(this);
    wui->setupUi(spec);
    addSpecWidget();

    setWindowTitle("Wall Editor");

    /* Colors */

    connect(wui->dispAlpha, SIGNAL(sendColor(quint8)), this, SLOT(receiveAlpha(quint8)));
    connect(wui->dispColor, SIGNAL(sendColor(quint8)), this, SLOT(receiveColor(quint8)));
    connect(img, SIGNAL(refreshHappened()), this, SLOT(refreshColorButtons()));
    refreshColorButtons();

    /* Tools */

    ui->toolbox->addTool(new GfxEditorToolBitDraw(img, this));
    ui->toolbox->addTool(new GfxEditorToolBitRect(img, this));
    ui->toolbox->addTool(new GfxEditorToolInvRect(img, this));
    ui->toolbox->addTool(new GfxEditorToolSelect(img, this));
    ui->toolbox->changeTool(0);

    /* Buttons */

    connect(wui->btnCopy2_4,  SIGNAL(clicked()), this, SLOT(copy2_4()));
    connect(wui->btnCopy2_34, SIGNAL(clicked()), this, SLOT(copy2_34()));
    connect(wui->btnCopy3,    SIGNAL(clicked()), this, SLOT(copy3()));
    connect(wui->btnCopy4_2,  SIGNAL(clicked()), this, SLOT(copy4_2()));
    connect(wui->btnCopy4_23, SIGNAL(clicked()), this, SLOT(copy4_23()));
}
WallEditor::~WallEditor()
{
    delete wui;
    delete spec;
}

void WallEditor::copy2_4()
{
    int y=21;
    shared_ptr<GfxData> src_data(data->copy(0,y,23,y+20));
    data->paste(src_data,0,63);
    his->save(data);
    img->refresh();
}

void WallEditor::copy2_34()
{
    int y=21;
    shared_ptr<GfxData> src_data(data->copy(0,y,23,y+20));
    data->paste(src_data,0,42);
    data->paste(src_data,0,63);
    his->save(data);
    img->refresh();
}

void WallEditor::copy3()
{
    int y=42;
    shared_ptr<GfxData> src_data(data->copy(0,y,23,y+20));
    data->paste(src_data,0,21);
    data->paste(src_data,0,63);
    his->save(data);
    img->refresh();
}

void WallEditor::copy4_2()
{
    int y=63;
    shared_ptr<GfxData> src_data(data->copy(0,y,23,y+20));
    data->paste(src_data,0,21);
    his->save(data);
    img->refresh();
}

void WallEditor::copy4_23()
{
    int y=63;
    shared_ptr<GfxData> src_data(data->copy(0,y,23,y+20));
    data->paste(src_data,0,21);
    data->paste(src_data,0,42);
    his->save(data);
    img->refresh();
}

/****    C O L O R S
 ******************************************************************************/

/* Receiving selection from color dialog windows */

void WallEditor::receiveColor(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Color, i);
    his->save(data);
    img->refresh();
}
void WallEditor::receiveAlpha(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Backg, i);
    his->save(data);
    img->refresh();
}
void WallEditor::refreshColorButtons(void)
{
    wui->dispColor->buttonColor(data->color((int)GfxData::ColorIndex::Color));
    wui->dispAlpha->buttonColor(data->color((int)GfxData::ColorIndex::Backg));
}

/****    S C E N E R Y   E D I T O R S
 ******************************************************************************/

SceneryWallEditor::SceneryWallEditor(shared_ptr<GfxData> init, int index, QWidget *parent) :
    WallEditor(init, index, parent)
{
    setWindowTitle("Scenery Wall Editor");
}

SceneryWallEditor::~SceneryWallEditor() {}

void SceneryWallEditor::apply()
{
    if (props.editor_scenery.wall_vector.at(I).usage == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Unused walls are not allowed to be modified!\nSorry!");
        msgBox.exec();
        return;
    }

    if(props.img2wall(I,data))
    {
        shared_ptr<GfxData> d = src.lock();
        if(d)
        {
            d->load(data);
            d->dataChanged();
        }
        /* No 'else'. It is a scenery tile. Its data must exist! */
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("There are not enough resources to modify wall!");
        msgBox.exec();
        return;
    }
}

void SceneryWallEditor::revert()
{

}
