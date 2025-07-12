QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    functions.cpp \
    graphinfo.cpp \
    homepage.cpp \
    main.cpp \
    menu.cpp \
    mgraph.cpp

HEADERS += \
    Parameter.h \
    functions.h \
    graphinfo.h \
    homepage.h \
    menu.h \
    mgraph.h

FORMS += \
    functions.ui \
    graphinfo.ui \
    homepage.ui \
    menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
