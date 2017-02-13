
#include "editor.h"

SketchEditor::SketchEditor(shared_ptr<GfxData> init, int index, QWidget *parent) :
    Editor(init, index, parent),
    wui(new Ui::SketchEditorWidget)
{
    spec = new QWidget(this);
    wui->setupUi(spec);
    addSpecWidget();

    setWindowTitle("Tile Sketch Editor");

    /* Colors */

    connect(wui->dispAlpha, SIGNAL(sendColor(quint8)), this, SLOT(receiveAlpha(quint8)));
    connect(wui->dispColor, SIGNAL(sendColor(quint8)), this, SLOT(receiveColor(quint8)));
    connect(wui->dispComm0, SIGNAL(sendColor(quint8)), this, SLOT(receiveComm0(quint8)));
    connect(wui->dispComm1, SIGNAL(sendColor(quint8)), this, SLOT(receiveComm1(quint8)));
    connect(img, SIGNAL(refreshHappened()), this, SLOT(refreshColorButtons()));
    refreshColorButtons();

    /* Tools */

    ui->toolbox->addTool(new GfxEditorToolBitDraw(img, this));
    ui->toolbox->addTool(new GfxEditorToolBitRect(img, this));
    ui->toolbox->addTool(new GfxEditorToolClrDraw(img, this));
    ui->toolbox->addTool(new GfxEditorToolClrRect(img, this));
    ui->toolbox->addTool(new GfxEditorToolSelect(img, this));
    ui->toolbox->changeTool(0);

    ui->checkFineGrid->setChecked(true);
}
SketchEditor::~SketchEditor()
{
    delete wui;
    delete spec;
}

/****    C O L O R S
 ******************************************************************************/

/* Receiving selection from color dialog windows */

void SketchEditor::receiveColor(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Color, i);
    his->save(data);
    img->refresh();
}
void SketchEditor::receiveAlpha(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Backg, i);
    his->save(data);
    img->refresh();
}
void SketchEditor::receiveComm0(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Comm0, i);
    his->save(data);
    img->refresh();
}
void SketchEditor::receiveComm1(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Comm1, i);
    his->save(data);
    img->refresh();
}
void SketchEditor::refreshColorButtons(void)
{
    wui->dispColor->buttonColor(data->color((int)GfxData::ColorIndex::Color));
    wui->dispAlpha->buttonColor(data->color((int)GfxData::ColorIndex::Backg));
    wui->dispComm0->buttonColor(data->color((int)GfxData::ColorIndex::Comm0));
    wui->dispComm1->buttonColor(data->color((int)GfxData::ColorIndex::Comm1));
}
