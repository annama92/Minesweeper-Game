TEMPLATE = app
TARGET = as1
INCLUDEPATH += .
# RESOURCES += /home/anna/as1/resources.qrc
RESOURCES += /Users/annama/4TH_YEAR_CS/Minesweeper/resources.qrc
DEFINES += QT_DEPRECATED_WARNINGS

# Input
HEADERS += \
    button.h \
    minesweeper.h 

SOURCES += \
    button.cpp \
    minesweeper.cpp 

#Qt modules
QT += widgets
QT += core gui