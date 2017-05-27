#ifndef TOOLS_H
#define TOOLS_H

#include "toolbox.h"

 /*================================================================================*\
( *     D R A W
 \*================================================================================*/

class GfxEditorToolBitDraw : public GfxEditorTool
{
    Q_OBJECT
public:
    explicit GfxEditorToolBitDraw(GfxImage *img, QObject *editor);
    virtual ~GfxEditorToolBitDraw();
public slots:
    void gfxEditorToolPressEvent(QPoint p, int m) override;
    void gfxEditorToolMoveEvent(QPoint p) override;
    void gfxEditorToolReleaseEvent(QPoint /* unused */) override;
};

 /*================================================================================*\
( *     D R A W - R E C T
 \*================================================================================*/

class GfxEditorToolBitRect : public GfxEditorTool
{
   Q_OBJECT
public:
   explicit GfxEditorToolBitRect(GfxImage *img, QObject *editor);
   virtual ~GfxEditorToolBitRect();
public slots:
   void gfxEditorToolPressEvent(QPoint p, int m) override;
   void gfxEditorToolMoveEvent(QPoint p) override;
   void gfxEditorToolReleaseEvent(QPoint /* unused */) override;
};

 /*================================================================================*\
( *     P A I N T
 \*================================================================================*/

class GfxEditorToolClrDraw : public GfxEditorTool
{
 Q_OBJECT
public:
 explicit GfxEditorToolClrDraw(GfxImage *img, QObject *editor);
 virtual ~GfxEditorToolClrDraw();
public slots:
 void gfxEditorToolPressEvent(QPoint p, int m) override;
 void gfxEditorToolMoveEvent(QPoint p) override;
 void gfxEditorToolReleaseEvent(QPoint /* unused */) override;
};

 /*================================================================================*\
( *     P A I N T - R E C T
 \*================================================================================*/

class GfxEditorToolClrRect : public GfxEditorTool
{
  Q_OBJECT
public:
  explicit GfxEditorToolClrRect(GfxImage *img, QObject *editor);
  virtual ~GfxEditorToolClrRect();
public slots:
  void gfxEditorToolPressEvent(QPoint p, int m) override;
  void gfxEditorToolMoveEvent(QPoint p) override;
  void gfxEditorToolReleaseEvent(QPoint /* unused */) override;
};

/*================================================================================*\
( *     I N V E R T - R E C T
\*================================================================================*/

class GfxEditorToolInvRect : public GfxEditorTool
{
 Q_OBJECT
public:
 explicit GfxEditorToolInvRect(GfxImage *img, QObject *editor);
 virtual ~GfxEditorToolInvRect();
public slots:
 void gfxEditorToolPressEvent(QPoint p, int) override;
 void gfxEditorToolMoveEvent(QPoint p) override;
 void gfxEditorToolReleaseEvent(QPoint /* unused */) override;
};

 /*================================================================================*\
( *     S E L E C T
 \*================================================================================*/

class GfxEditorToolSelect : public GfxEditorTool
{
  Q_OBJECT
public:
  explicit GfxEditorToolSelect(GfxImage *img, QObject *editor);
  virtual ~GfxEditorToolSelect();
public slots:
  void gfxEditorToolPressEvent(QPoint p, int m) override;
  void gfxEditorToolMoveEvent(QPoint p) override;
  void open(void) override;
  void close(void) override;
};

#endif // TOOLS_H
