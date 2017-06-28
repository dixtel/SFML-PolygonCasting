TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

OTHER_FILES += Levels/level1.txt

SOURCES += main.cpp \
    game.cpp \
    gameobject.cpp \
    player.cpp \
    render.cpp \
    world.cpp \
    wall.cpp \
    collision.cpp \
    polygoncast.cpp \
    toolkit.cpp

include(deployment.pri)
qtcAddDeployment()

LIBS += -L"/home/dixtel/Documents/SFML-master/lib"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

INCLUDEPATH += "/home/dixtel/Documents/SFML-master/include"
DEPENDPATH += "/home/dixtel/Documents/SFML-master/include"

HEADERS += \
    game.h \
    gameobject.h \
    player.h \
    render.h \
    world.h \
    wall.h \
    collision.h \
    polygoncast.h \
    toolkit.h
