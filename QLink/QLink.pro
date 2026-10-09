QT += widgets

CONFIG += c++17

TARGET = QLink
TEMPLATE = app

INCLUDEPATH += src/core src/ui

SOURCES += \
    src/main.cpp \
    src/core/Board.cpp \
    src/ui/GameWidget.cpp

HEADERS += \
    src/core/TileType.h \
    src/core/Board.h \
    src/ui/GameWidget.h
