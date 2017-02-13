#ifndef COLOR_DIALOG_H
#define COLOR_DIALOG_H

#include "ui_color_dialog.h"
#include "general.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QVector>

 /*================================================================================*\
( *     Color Dialog Rectangle
 \*================================================================================*/

/* ColorDialogRect is a painted rectangle
 * that represents a selectable color
 * in the ColorDialog window.
 */

class ColorDialogRect : public QObject, public  QGraphicsRectItem
{
    Q_OBJECT

private:
    void mousePressEvent(QGraphicsSceneMouseEvent */* unused */);

public:
    quint8 id;
    static const int ConstW=50;
    static const int ConstH=30;
    explicit ColorDialogRect(int id, int x, int y);

signals:
    void clicked(quint8 id);
};

 /*================================================================================*\
( *     Color Dialog Window
 \*================================================================================*/

/* ColorDialog window is used to select
 * a color from the C64 color palette.
 */

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorDialog(int quantity, QWidget *parent);
    virtual ~ColorDialog();

protected:
    Ui::ColorDialog *ui;
    QGraphicsScene *scene;
    QVector<ColorDialogRect *> rect;

public slots:
    void receiveColor(quint8 i);

signals:
    void sendColor(quint8 i);
};

class ColorDialogHires : public ColorDialog
{
    Q_OBJECT
public:
    explicit ColorDialogHires(QWidget *parent = 0);
};

class ColorDialogMulti : public ColorDialog
{
    Q_OBJECT
public:
    explicit ColorDialogMulti(QWidget *parent = 0);
};

class ColorDialogAlpha : public ColorDialog
{
    Q_OBJECT
public:
    explicit ColorDialogAlpha(QWidget *parent = 0);
};

 /*================================================================================*\
( *     Color Dialog Button
 \*================================================================================*/

/* ColorDialogButton is a button
* which pops-up a ColorDialog window on click
* and displays the selected color.
*/

/* Pop-up default ColorDialog window with 16 HiRes colors */
class ColorDialogButton : public QGraphicsView {
   Q_OBJECT
private:
   virtual void mousePressEvent(QMouseEvent */* unused */);
protected:
   QGraphicsScene *scene;
public:
   explicit ColorDialogButton(QWidget *parent);
   virtual ~ColorDialogButton();
   void setEnabled(bool val);

public slots:
   void receiveColor(quint8 color);
   virtual void buttonColor(quint8 color);
signals:
   void sendColor(quint8 color);
};


/* Pop-up an extended ColorDialog window with
 * 16 HiRes colors and 16 additional colors for
 * the background of an editor area.
 */
class ColorDialogButtonAlpha : public ColorDialogButton {
   Q_OBJECT
private:
   virtual void mousePressEvent(QMouseEvent */* unused */);
public:
   explicit ColorDialogButtonAlpha(QWidget *parent);
   virtual ~ColorDialogButtonAlpha();
public slots:
   virtual void buttonColor(quint8 color) override;
};


/* Pop-up ColorDialog window with 16 MC colors */
class ColorDialogButtonMulti : public ColorDialogButton {
   Q_OBJECT
private:
   virtual void mousePressEvent(QMouseEvent */* unused */);
public:
   explicit ColorDialogButtonMulti(QWidget *parent);
   virtual ~ColorDialogButtonMulti();
public slots:
   virtual void buttonColor(quint8 color) override;
};

#endif // COLOR_DIALOG_H
