
#include "editor.h"

CnFSketchEditor::CnFSketchEditor(shared_ptr<GfxData> init, int index, QWidget *parent) :
    Editor(init, index, parent),
    wui(new Ui::CnFSketchEditorWidget)
{
    spec = new QWidget(this);
    wui->setupUi(spec);
    addSpecWidget();

    setWindowTitle("CnF Tile Sketch Editor");

    /* Colors */

    connect(wui->dispColor, SIGNAL(sendColor(quint8)), this, SLOT(receiveColor(quint8)));
    connect(wui->dispECM0, SIGNAL(sendColor(quint8)), this, SLOT(receiveECM0(quint8)));
    connect(wui->dispECM1, SIGNAL(sendColor(quint8)), this, SLOT(receiveECM1(quint8)));
    connect(wui->dispECM2, SIGNAL(sendColor(quint8)), this, SLOT(receiveECM2(quint8)));
    connect(wui->dispECM3, SIGNAL(sendColor(quint8)), this, SLOT(receiveECM3(quint8)));
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
CnFSketchEditor::~CnFSketchEditor()
{
    delete wui;
    delete spec;
}

/****    C O L O R S
 ******************************************************************************/

/* Receiving selection from color dialog windows */

void CnFSketchEditor::receiveColor(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::Color, i);
    his->save(data);
    img->refresh();
}
void CnFSketchEditor::receiveECM0(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::ECM0, i);
    his->save(data);
    img->refresh();
}
void CnFSketchEditor::receiveECM1(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::ECM1, i);
    his->save(data);
    img->refresh();
}
void CnFSketchEditor::receiveECM2(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::ECM2, i);
    his->save(data);
    img->refresh();
}
void CnFSketchEditor::receiveECM3(quint8 i)
{
    data->setColor((int)GfxData::ColorIndex::ECM3, i);
    his->save(data);
    img->refresh();
}
void CnFSketchEditor::refreshColorButtons(void)
{
    wui->dispColor->buttonColor(data->color((int)GfxData::ColorIndex::Color));
    wui->dispECM0->buttonColor(data->color((int)GfxData::ColorIndex::ECM0));
    wui->dispECM1->buttonColor(data->color((int)GfxData::ColorIndex::ECM1));
    wui->dispECM2->buttonColor(data->color((int)GfxData::ColorIndex::ECM2));
    wui->dispECM3->buttonColor(data->color((int)GfxData::ColorIndex::ECM3));
}
