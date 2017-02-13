
#include "color_dialog.h"

 /*================================================================================*\
( *     Color Dialog Rectangle
 \*================================================================================*/

/* ColorDialogRect is a painted rectangle
 * that represents a selectable color
 * in the ColorDialog window.
 */

void ColorDialogRect::mousePressEvent(QGraphicsSceneMouseEvent */* unused */)
{
    emit clicked(id);
}

ColorDialogRect::ColorDialogRect(int id=0, int x=0, int y=0)
{
    this->id = (quint8)id;
    this->setRect( x*ConstW,  /* x */
                   y*ConstH,  /* y */
                     ConstW,  /* width  */
                     ConstH); /* height */
}


 /*================================================================================*\
( *     Color Dialog Window
 \*================================================================================*/

/* ColorDialog window is used to select
 * a color from the C64 color palette.
 */

ColorDialog::ColorDialog(int quantity, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    ui->setupUi(this);
    rect.resize(quantity);
    scene = new QGraphicsScene(this);
    for(int i=0; i<quantity; i++)
    {
        rect.data()[i] = new ColorDialogRect(i, (int)(i%8), (int)(i/8));
        scene->addItem(rect.at(i));
        connect(rect.at(i), SIGNAL(clicked(quint8)), this, SLOT(receiveColor(quint8)));
    }
    ui->view->setScene(scene);
    int lines = (quantity+7)/8;
    ui->view->setMinimumHeight(lines*ColorDialogRect::ConstH+2);
    ui->view->setMaximumHeight(lines*ColorDialogRect::ConstH+2);
    ui->view->setSceneRect(0, 0, 8*ColorDialogRect::ConstW,
                             lines*ColorDialogRect::ConstH );
    ui->view->show();
}
ColorDialog::~ColorDialog()
{
    delete ui;
}

void ColorDialog::receiveColor(quint8 i)
{
    emit sendColor(i);
    this->close();
}


ColorDialogHires::ColorDialogHires(QWidget *parent) :
    ColorDialog(0x10, parent)
{
    for(int i=0; i<rect.size(); i++)
        rect.at(i)->setBrush(C64::ins().brush_hires(i)); /* Brush Hires */
}

ColorDialogMulti::ColorDialogMulti(QWidget *parent) :
    ColorDialog(0x10, parent)
{
    for(int i=0; i<rect.size(); i++)
        rect.at(i)->setBrush(C64::ins().brush_multi(i)); /* Brush MC */
}

ColorDialogAlpha::ColorDialogAlpha(QWidget *parent) :
    ColorDialog(0x20, parent)
{
    for(int i=0; i<rect.size(); i++)
        rect.at(i)->setBrush(C64::ins().brush_alpha(i)); /* Brush Alpha */
}

 /*================================================================================*\
( *     Color Dialog Button
 \*================================================================================*/

/* ColorDialogButton is a button
* which pops-up a ColorDialog window on click
* and displays the selected color.
*/

/* Pop-up default ColorDialog window with 16 HiRes colors */
void ColorDialogButton::mousePressEvent(QMouseEvent */* unused */)
{
    ColorDialogHires colorDialogWindow;
    colorDialogWindow.setModal(true);
    QObject::connect(&colorDialogWindow, SIGNAL(sendColor(quint8)),
                     this, SLOT(receiveColor(quint8)));
    colorDialogWindow.exec();
}

ColorDialogButton::ColorDialogButton(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    show();
}

ColorDialogButton::~ColorDialogButton()
{}

void ColorDialogButton::setEnabled(bool val)
{
    if(!val) scene->setBackgroundBrush(QColor(QRgb(0xFFF0F0F0)));
    QWidget::setEnabled(val);
}

void ColorDialogButton::receiveColor(quint8 color)
{
    emit sendColor(color);
}

void ColorDialogButton::buttonColor(quint8 color)
{
    if( isEnabled() )
        scene->setBackgroundBrush( C64::ins().brush_hires(color) );
}


/* Pop-up an extended ColorDialog window with
 * 16 HiRes colors and 16 additional colors for
 * the background of an editor area.
 */
void ColorDialogButtonAlpha::mousePressEvent(QMouseEvent */* unused */)
{
    ColorDialogAlpha colorDialogWindow;
    colorDialogWindow.setModal(true);
    QObject::connect(&colorDialogWindow, SIGNAL(sendColor(quint8)),
                     this, SLOT(receiveColor(quint8)));
    colorDialogWindow.exec();
}

ColorDialogButtonAlpha::ColorDialogButtonAlpha(QWidget *parent) :
    ColorDialogButton(parent)
{}

ColorDialogButtonAlpha::~ColorDialogButtonAlpha()
{}

void ColorDialogButtonAlpha::buttonColor(quint8 color)
{
    if( isEnabled() )
        scene->setBackgroundBrush( C64::ins().brush_alpha(color) );
}


/* Pop-up ColorDialog window with 16 MC colors */
void ColorDialogButtonMulti::mousePressEvent(QMouseEvent */* unused */)
{
    ColorDialogMulti colorDialogWindow;
    colorDialogWindow.setModal(true);
    QObject::connect(&colorDialogWindow, SIGNAL(sendColor(quint8)),
                     this, SLOT(receiveColor(quint8)));
    colorDialogWindow.exec();
}

ColorDialogButtonMulti::ColorDialogButtonMulti(QWidget *parent) :
    ColorDialogButton(parent)
{}

ColorDialogButtonMulti::~ColorDialogButtonMulti()
{}

void ColorDialogButtonMulti::buttonColor(quint8 color)
{
    if( isEnabled() )
        scene->setBackgroundBrush( C64::ins().brush_multi(color) );
}
