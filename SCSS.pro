#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T10:54:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SCSS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += resource/translate/SCSS_cn.ts
RC_ICONS = resource/image/SCSS.ico
INCLUDEPATH += include

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/OptionTreeWidget.cpp \
    src/RetakeCourseWidget.cpp \
    src/SelectableCourseWidget.cpp \
    src/TakedCourseWidget.cpp \
    src/IFileAnalysis.cpp \
    src/TeachingPlanFileAnalysis.cpp \
    src/TrainingProgramFileAnalysis.cpp \
    src/ChooseWidget.cpp \
    src/ConnectionPool.cpp

HEADERS  += include/MainWindow.h \
    include/OptionTreeWidget.h \
    include/RetakeCourseWidget.h \
    include/SelectableCourseWidget.h \
    include/TakedCourseWidget.h \
    include/IFileAnalysis.h \
    include/TeachingPlanFileAnalysis.h \
    include/TrainingProgramFileAnalysis.h \
    include/DataTypeDefine.h \
    include/ChooseWidget.h \
    include/ConnectionPool.h

FORMS    += ui/MainWindow.ui

RESOURCES += \
    resource/scss.qrc
