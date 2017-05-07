#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QFile>
#include <QDebug>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    //display style sheet
    QFile file(":/qss/default.qss");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << __FILE__ << __LINE__ << "qss file canot load!";
    }
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);

    //load plugins
    a.addLibraryPath("./plugins");

    //Qt
    QTranslator translatorQt;
    translatorQt.load(":/translate/qt_zh_CN");
    a.installTranslator(&translatorQt);

    //local
    QTranslator translator;
    translator.load(":/translate/SCSS_cn");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
