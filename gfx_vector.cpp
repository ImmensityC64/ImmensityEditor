
#include "gfx.h"

GfxVector::GfxVector(int tSize) :
    pVector(tSize)
{
    pBrowser = nullptr;
}
GfxVector::~GfxVector() {}

Browser *GfxVector::browser(void) { return pBrowser; }
Browser *GfxVector::setBrowser(Browser *tBrowser) { return pBrowser = tBrowser; }

QString GfxVector::title(void) { return pTitle; }
QString GfxVector::setTitle(QString tTitle) { return pTitle = tTitle; }

GfxVector::Scope GfxVector::scope(void) { return pScope; }
GfxVector::Scope GfxVector::setScope(Scope tScope) { return pScope = tScope; }

Qt::Orientation GfxVector::ori(void) { return pOri; }
Qt::Orientation GfxVector::setOri(Qt::Orientation tOri) { return pOri = tOri; }

GfxImage::Mode GfxVector::mode(void) { return pMode; }
GfxImage::Mode GfxVector::setMode(GfxImage::Mode tMode) { return pMode = tMode; }

GfxData::Type GfxVector::type(void) { return pType; }
GfxData::Type GfxVector::setType(GfxData::Type tType) { return pType = tType; }

int GfxVector::size(void) { return pVector.size(); }

/* TODO: Do not delete read-only elements */
void GfxVector::clear(void)
{
    close();
    pVector.clear();
}

void GfxVector::close(void)
{
    /* Close all Editor windows */
    QVectorIterator<Element> i(pVector);
    while (i.hasNext())
    {
        Element e = i.next();
        if(e.editor) ((QMainWindow*)e.editor)->close();
    }
}

shared_ptr<GfxData> GfxVector::dataAt(int index)
{
    shared_ptr<GfxData> ret;
    if(0 <= index && pVector.size() > index)
        ret = pVector.at(index).data;
    return ret;
}
shared_ptr<GfxData> GfxVector::setDataAt(int index, shared_ptr<GfxData> tData)
{
    if((int)GfxData::Id::Append == index)
    {
        Element e;
        initElement(e);
        e.data = tData;
        pVector.append(e);
        emit newElementAppended();
    }
    else if(0 <= index && pVector.size() > index)
    {
        pVector.at(index).data->load(tData);
    }
    return tData;
}

Editor *GfxVector::editorAt(int index)
{
    Editor *e = nullptr;
    if(0 <= index && pVector.size() > index)
        e = pVector.at(index).editor;
    return e;
}

Editor *GfxVector::setEditorAt(int index, Editor *tEditor)
{
    Editor *e = nullptr;
    if(0 <= index && pVector.size() > index)
    {
        pVector.data()[index].editor = tEditor;
        e = pVector.at(index).editor;
    }
    return e;
}

void GfxVector::closeEditorAt(int index)
{
    setEditorAt(index, nullptr);
}

void GfxVector::initElement(Element &e)
{
    /* 'e.data' is already an empty shared pointer */
    e.editor = nullptr;
    e.read_only = false;
}
