#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T15:07:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImmensityEditor
TEMPLATE = app

CONFIG += c++11 \
          -Wstrict-overflow

SOURCES += main.cpp\
    mainwindow.cpp \
    sprite_editor.cpp \
    file.cpp \
    browser.cpp \
    color_dialog.cpp \
    general.cpp \
    toolbox.cpp \
    tools.cpp \
    gfx_data.cpp \
    gfx_image.cpp \
    gfx_rect_item.cpp \
    gfx_view.cpp \
    editor.cpp \
    gfx_vector.cpp \
    sketch_editor.cpp \
    cnfsketch_editor.cpp \
    menu.cpp \
    serialize.cpp \
    scenery.cpp \
    map.cpp \
    mainw_editor.cpp \
    map_settings.cpp \
    grid.cpp \
    theme_editor.cpp \
    scenery_editor.cpp \
    props_map2img.cpp \
    props_img2map.cpp \
    char_set_w.cpp

HEADERS  += general.h \
    theme_editor.h \
    scenery_editor.h \
    mainwindow.h \
    color_dialog.h \
    toolbox.h \
    browser.h \
    tools.h \
    editor.h \
    gfx_data.h \
    gfx.h \
    map_settings.h \
    map_general.h \
    grid.h \
    human.h \
    cyber.h \
    enemy.h \
    new_dialog.h \
    char_set_w.h

FORMS    += mainwindow.ui \
    color_dialog.ui \
    editor.ui \
    sprite_editor.ui \
    sketch_editor.ui \
    cnfsketch_editor.ui \
    map_settings.ui \
    new_dialog.ui \
    theme_editor.ui \
    scenery_editor.ui \
    char_set_w.ui

OTHER_FILES +=
