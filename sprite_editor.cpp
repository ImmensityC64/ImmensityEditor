
#include "editor.h"

SpriteEditor::SpriteEditor(shared_ptr<GfxData> init, int index, QWidget *parent) :
    Editor(init, index, parent),
    wui(new Ui::SpriteEditorWidget)
{
    spec = new QWidget(this);
    wui->setupUi(spec);
    addSpecWidget();

    setWindowTitle("Sprite Editor");

    /* Selection rectangles are not displayed properly in
     * Ceiling and Floor display modes!
     * TODO: Think about how to solve this, or at least,
     * display a warning message about it somewhere!
     * Uncomment this to disable selection target rectangle: */
    //showSelTgt = false;

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

    if(GfxImage::Mode::Wall == img->mode())
    {
        wui->labelDisplay->setEnabled(false);
        wui->comboDisplay->setEnabled(false);
    }
    else
    {
        connect(wui->comboDisplay, SIGNAL(currentIndexChanged(int)), this, SLOT(changeImgDisplayMode(int)));
    }
}
SpriteEditor::~SpriteEditor()
{
    delete wui;
    delete spec;
}

/****    C O L O R S
 ******************************************************************************/

/* Receiving selection from color dialog windows */

void SpriteEditor::receiveColor(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Color, i);
    his->save(data);
    img->refresh();
}
void SpriteEditor::receiveAlpha(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Backg, i);
    his->save(data);
    img->refresh();
}
void SpriteEditor::refreshColorButtons(void)
{
    wui->dispColor->buttonColor(data->color((int)GfxData::ColorIndex::Color));
    wui->dispAlpha->buttonColor(data->color((int)GfxData::ColorIndex::Backg));
}

/****    D I S P L A Y   M O D E
 ******************************************************************************/

void SpriteEditor::changeDisplayMode(int i)
{
    wui->comboDisplay->setCurrentIndex(i);
}

void SpriteEditor::changeImgDisplayMode(int i)
{
    if      (1==i) img->setMode(GfxImage::Mode::Ceiling );
    else if (2==i) img->setMode(GfxImage::Mode::Floor   );
    else           img->setMode(GfxImage::Mode::Expanded);
    grid->clear();
    createGrids();
}

/****    S C E N E R Y   E D I T O R S
 ******************************************************************************/

ScenerySpriteEditor::ScenerySpriteEditor(shared_ptr<GfxData> init, int index, QWidget *parent) :
    SpriteEditor(init, index, parent)
{
    setWindowTitle("Scenery Sprite Editor");
}

ScenerySpriteEditor::~ScenerySpriteEditor() {}

void ScenerySpriteEditor::apply()
{
    if (props.editor_scenery.sprite_vector.at(I).usage == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Unused sprites are not allowed to be modified!\nSorry!");
        msgBox.exec();
        return;
    }

    props.img2sprite(I,data);

    shared_ptr<GfxData> d = src.lock();
    if(d)
    {
        d->load(data);
        d->dataChanged();
    }
}

void ScenerySpriteEditor::revert()
{

}
