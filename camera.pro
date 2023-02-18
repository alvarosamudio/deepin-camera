TEMPLATE = app
TARGET = deepin-camera
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

QT += multimedia multimediawidgets

HEADERS = \
    camera.h \
    imagesettings.h \
    videosettings.h

SOURCES = \
    main.cpp \
    camera.cpp \
    imagesettings.cpp \
    videosettings.cpp

FORMS += \
    camera.ui

RESOURCES += camera.qrc

isEmpty(PREFIX) {
    PREFIX = /usr
}

target.path = $$PREFIX/bin

desktop.path = $$PREFIX/share/applications/
desktop.files = deepin-camera.desktop

icons.path = $$PREFIX/share/icons/hicolor/scalable/apps/
icons.files = images/logo.svg

INSTALLS += target desktop icons
