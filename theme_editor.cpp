#include "theme_editor.h"

ThemeEditor::ThemeEditor(Props &props, QWidget *parent) :
    QMainWindow(parent),
    tmp_theme(),
    scrBgs((int)Theme::Enum::Size),
    props(props),
    theme_index(0),
    received_color(0),
    ui(new Ui::ThemeEditor)
{
    ui->setupUi(this);
    init();
}

ThemeEditor::~ThemeEditor()
{
    delete ui;
}

void ThemeEditor::init()
{
    for(int i=0; i<props.themes.size(); i++)
        ui->theme->addItem(props.themes.at(i)->name);

    for(int i=0; i<bgAreas.size(); i++)
        ui->color->addItem(bgAreas.at(i));

    initScrBgs();
    reload();

    for(int i=0; i<(int)Theme::Enum::Size; i++)
    {
        scrBgs[i]->setPen(QPen(Qt::NoPen));
        scrBgs[i]->setZValue(-2);
        ui->view->scene->addItem(scrBgs.at(i));
        connect(scrBgs[i], SIGNAL(clicked(int)), this, SLOT(scrBgClicked(int)));
    }

    setWindowTitle("Theme Editor");

    connect(ui->newTheme, SIGNAL(clicked()), this, SLOT(createNew()));
    connect(ui->checkDelete, SIGNAL(clicked(bool)), this, SLOT(enableDeletBtn(bool)));
    connect(ui->deleteTheme, SIGNAL(clicked()), this, SLOT(deleteTheme()));
    connect(ui->theme, SIGNAL(currentIndexChanged(int)), this, SLOT(selectTheme(int)));
    connect(ui->color, SIGNAL(activated(int)), this, SLOT(setAreaColor(int)));
    connect(ui->name, SIGNAL(textEdited(QString)), this, SLOT(nameEdited(QString)));
    connect(ui->btnApply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->btnReload, SIGNAL(clicked()), this, SLOT(reload()));
}

void ThemeEditor::enableDeletBtn(bool b) { ui->deleteTheme->setEnabled(b); }

void ThemeEditor::initScrBgs()
{
    scrBgs[(int)Theme::Enum::CeilingFg]       = new ClickableRectItem((int)Theme::Enum::CeilingFg,       -152, -72, 304,  24);
    scrBgs[(int)Theme::Enum::CeilingFgECM0]   = new ClickableRectItem((int)Theme::Enum::CeilingFgECM0,   -150, -70,  40,  20);
    scrBgs[(int)Theme::Enum::CeilingFgECM1]   = new ClickableRectItem((int)Theme::Enum::CeilingFgECM1,   -108, -70,  40,  20);
    scrBgs[(int)Theme::Enum::CeilingFgECM2]   = new ClickableRectItem((int)Theme::Enum::CeilingFgECM2,    -66, -70,  40,  20);
    scrBgs[(int)Theme::Enum::CeilingFgECM3]   = new ClickableRectItem((int)Theme::Enum::CeilingFgECM3,    -24, -70,  40,  20);
    scrBgs[(int)Theme::Enum::Ceiling0]        = new ClickableRectItem((int)Theme::Enum::Ceiling0,        -152, -48, 304,   3);
    scrBgs[(int)Theme::Enum::Ceiling1]        = new ClickableRectItem((int)Theme::Enum::Ceiling1,        -152, -45, 304,   2);
    scrBgs[(int)Theme::Enum::Ceiling2]        = new ClickableRectItem((int)Theme::Enum::Ceiling2,        -152, -43, 304,   2);
    scrBgs[(int)Theme::Enum::Ceiling3]        = new ClickableRectItem((int)Theme::Enum::Ceiling3,        -152, -41, 304,   2);
    scrBgs[(int)Theme::Enum::Ceiling4]        = new ClickableRectItem((int)Theme::Enum::Ceiling4,        -152, -39, 304,   2);
    scrBgs[(int)Theme::Enum::Ceiling5]        = new ClickableRectItem((int)Theme::Enum::Ceiling5,        -152, -37, 304,   2);
    scrBgs[(int)Theme::Enum::Ceiling6]        = new ClickableRectItem((int)Theme::Enum::Ceiling6,        -152, -35, 304,   3);
    scrBgs[(int)Theme::Enum::Background]      = new ClickableRectItem((int)Theme::Enum::Background,      -152, -32, 304,  64);
    scrBgs[(int)Theme::Enum::BackgroundComm0] = new ClickableRectItem((int)Theme::Enum::BackgroundComm0, -150, -32,  40,  20);
    scrBgs[(int)Theme::Enum::BackgroundComm1] = new ClickableRectItem((int)Theme::Enum::BackgroundComm1, -108, -32,  40,  20);
    scrBgs[(int)Theme::Enum::Floor6]          = new ClickableRectItem((int)Theme::Enum::Floor6,          -152,  32, 304,   3);
    scrBgs[(int)Theme::Enum::Floor5]          = new ClickableRectItem((int)Theme::Enum::Floor5,          -152,  35, 304,   2);
    scrBgs[(int)Theme::Enum::Floor4]          = new ClickableRectItem((int)Theme::Enum::Floor4,          -152,  37, 304,   2);
    scrBgs[(int)Theme::Enum::Floor3]          = new ClickableRectItem((int)Theme::Enum::Floor3,          -152,  39, 304,   2);
    scrBgs[(int)Theme::Enum::Floor2]          = new ClickableRectItem((int)Theme::Enum::Floor2,          -152,  41, 304,   2);
    scrBgs[(int)Theme::Enum::Floor1]          = new ClickableRectItem((int)Theme::Enum::Floor1,          -152,  43, 304,   2);
    scrBgs[(int)Theme::Enum::Floor0]          = new ClickableRectItem((int)Theme::Enum::Floor0,          -152,  45, 304,   3);
    scrBgs[(int)Theme::Enum::FloorFg]         = new ClickableRectItem((int)Theme::Enum::FloorFg,         -152,  48, 304,  24);
    scrBgs[(int)Theme::Enum::FloorFgECM0]     = new ClickableRectItem((int)Theme::Enum::FloorFgECM0,     -150,  50,  40,  20);
    scrBgs[(int)Theme::Enum::FloorFgECM1]     = new ClickableRectItem((int)Theme::Enum::FloorFgECM1,     -108,  50,  40,  20);
    scrBgs[(int)Theme::Enum::FloorFgECM2]     = new ClickableRectItem((int)Theme::Enum::FloorFgECM2,      -66,  50,  40,  20);
    scrBgs[(int)Theme::Enum::FloorFgECM3]     = new ClickableRectItem((int)Theme::Enum::FloorFgECM3,      -24,  50,  40,  20);
}

void ThemeEditor::refreshScrBgColors()
{
    for(int i=0; i<(int)Theme::Enum::Size; i++)
        scrBgs[i]->setBrush(C64::ins().brush_hires(tmp_theme.colors.at(i)));
}

void ThemeEditor::apply()
{
    QString tName = ui->name->text();

    if(tName != props.themes.at(theme_index)->name)
    {
        if(tName.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("Name field is empty!");
            msgBox.exec();
            ui->name->setFocus();
            return;
        }

        /* verify that no other theme has the same name */
        for(int i=0; i<props.themes.size(); i++)
        {
            if(tName == props.themes.at(i)->name)
            {
                QMessageBox msgBox;
                msgBox.setText("A theme has already been created with this name.\nName must be unique.\nPlease, change it!");
                msgBox.exec();
                ui->name->setFocus();
                return;
            }
        }

        tmp_theme.name = tName;
        ui->theme->setItemText(theme_index, tName);
    }

    *props.themes.at(theme_index) = tmp_theme;
    thereAreChanges(false);
    emit themeChanged(theme_index);
}

void ThemeEditor::reload()
{
    tmp_theme = *props.themes.at(theme_index);
    ui->name->setText(tmp_theme.name);
    refreshScrBgColors();
    if(ui->theme->currentIndex() != theme_index)
        ui->theme->setCurrentIndex(theme_index);
    thereAreChanges(false);
    ui->checkDelete->setChecked(false);
    enableDeletBtn(false);
}

void ThemeEditor::deleteTheme()
{
    if(props.deleteTheme(theme_index))
        ui->theme->removeItem(theme_index); // this will trigger the currentIndexChanged signal
}

void ThemeEditor::thereAreChanges(bool b)
{
    ui->btnApply->setEnabled(b);
    ui->btnReload->setEnabled(b);
    ui->theme->setEnabled(!b);
    ui->newTheme->setEnabled(!b);
}

void ThemeEditor::selectTheme(int index)
{
    /* Clearing combobox also triggers the signal which is connected to this slot,
     * thus we have to be sure that the combobox is not empty!
     */
    if(0 <= index)
    {
        theme_index = index;
        reload();
    }
}

void ThemeEditor::setAreaColor(int i)
{
    ColorDialogHires colorDialogWindow;
    colorDialogWindow.setModal(true);
    QObject::connect(&colorDialogWindow, SIGNAL(sendColor(quint8)),
                     this, SLOT(receiveColor(quint8)));
    colorDialogWindow.setWindowTitle(bgAreas.at(i));
    colorDialogWindow.exec();

    if(tmp_theme.colors.at(i) != received_color)
    {
        tmp_theme.colors[i] = received_color;
        refreshScrBgColors();
        thereAreChanges(true);
    }
}

void ThemeEditor::nameEdited(const QString &/* unused */) { thereAreChanges(true); }

void ThemeEditor::receiveColor(quint8 i) { received_color = i; }

void ThemeEditor::createNew()
{
    QString name = "";
    int copy = -1;
    NewDialog dialog((QVector<VE *> *) &props.themes, &name, &copy, this);
    dialog.exec();

    if(!name.isEmpty())
    {
        Theme *tTheme;
        if(-1 == copy) tTheme = new Theme();
        else tTheme = props.themes.at(copy)->copy();
        tTheme->name = name;
        theme_index = props.themes.size(); // will be the index of last element after append
        props.themes.append(tTheme);
        ui->theme->addItem(name);
        reload();
    }
}

void ThemeEditor::scrBgClicked(int i)
{
    ui->color->setCurrentIndex(i);
    setAreaColor(i);
}
