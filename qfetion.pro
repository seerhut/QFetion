# -------------------------------------------------
# Project created by QtCreator 2008-11-28T13:49:42
# -------------------------------------------------
TARGET = qfetion
TEMPLATE = app
QT += network \
    xml
CONFIG += qt \
    release
DESTDIR = bin
SRC_DIR = src
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc
UI_DIR = form
UI_HEADERS_DIR = include/ui
RESOURCES += images/images.qrc
INCLUDEPATH += include
TRANSLATIONS = translations/qfetion_zh_CN.ts
!win32:QMAKE_CXXFLAGS += -Wno-deprecated
include(src/src.pri)
include(include/include.pri)
include(form/form.pri)

