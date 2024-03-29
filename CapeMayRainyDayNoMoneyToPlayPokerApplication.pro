#-------------------------------------------------
#
# Project created by QtCreator 2019-07-23T19:29:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CapeMayRainyDayNoMoneyToPlayPokerApplication
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        action.cpp \
        actionhistoric.cpp \
        actionnavigator.cpp \
        initialmenu.cpp \
        main.cpp \
        partywidget.cpp \
        playerbox.cpp \
        size.cpp \
        widget.cpp

HEADERS += \
        action.h \
        actionhistoric.h \
        actionnavigator.h \
        initialmenu.h \
        partywidget.h \
        playerbox.h \
        size.h \
        widget.h

FORMS += \
        widget.ui

CONFIG += mobility
MOBILITY = 

RC_ICONS = icons/icons8-cards-96.ico

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

TRANSLATIONS = translations/CapeMayRainyDayNoMoneyToPlayPokerApplication-en_US.ts
TRANSLATIONS += translations/CapeMayRainyDayNoMoneyToPlayPokerApplication-fr_FR.ts

android: DEFINES += ANDROID_SIZE
else: DEFINES += DESKTOP_SIZE


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    Material-blue.qss \
    Material-white.qss \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
