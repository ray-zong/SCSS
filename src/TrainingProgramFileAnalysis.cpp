#include "TrainingProgramFileAnalysis.h"


#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QTextCodec>
#include <QStringList>

#include "DataTypeDefine.h"

TrainingProgramFileAnalysis::TrainingProgramFileAnalysis()
{

}

TrainingProgramFileAnalysis::~TrainingProgramFileAnalysis()
{
    //删除数据
    for(auto i = m_vecPersonalTrainProgram.begin();
        i != m_vecPersonalTrainProgram.end(); ++i)
    {
        delete *i;
    }
    m_vecPersonalTrainProgram.clear();
}

bool TrainingProgramFileAnalysis::analysis(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    //数据头
    analysisDataHead(file);
    //课程信息
    analysisDataContent(file);

    return true;
}

QVector<IPersonalData *> TrainingProgramFileAnalysis::getData()
{
    return m_vecPersonalTrainProgram;
}

bool TrainingProgramFileAnalysis::analysisDataHead(QFile &file)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");//指定QString的编码方式

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString strData = codec->toUnicode(line);
        if(strData.contains(codec->toUnicode("是否完成")))
        {
            qDebug() << __FILE__ << __LINE__ << strData;
            return true;
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << strData;
        }
    }
    return false;
}

bool TrainingProgramFileAnalysis::analysisDataContent(QFile &file)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");//指定QString的编码方式

    PersonalTrainingProgram *pPersonalTrainingProgram = new PersonalTrainingProgram;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString strData = codec->toUnicode(line).trimmed();

#if 0
        if(strData.contains(QRegExp(codec->toUnicode(c_szCourseAttrRegExp))))
        {
            //课程属性
            qDebug() << strData.trimmed();
        }
        else if(!strData.contains(QRegExp(codec->toUnicode(c_szCourseAttrRegExp)))
                && !strData.contains(QRegExp(codec->toUnicode(c_szIsFinishedRegExp)))
                && strData.contains(QRegExp("[\\x4e00-\\x9fa5]+"))
                )
        {
            //课组名
            qDebug() << strData.trimmed();
        }
        else if(strData.contains(QRegExp(codec->toUnicode(c_szNumberRegExp))))
        {
            //课程号
            pPersonalTrainingProgram->number = strData.toInt();
            //课程名
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                pPersonalTrainingProgram->name = codec->toUnicode(line).trimmed();
            }
            //学分
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString strData = codec->toUnicode(line);
                bool bOk;
                pPersonalTrainingProgram->credit = strData.toInt(&bOk);
                if(!bOk)
                {
                    continue;
                }
            }
            //成绩
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString strData = codec->toUnicode(line).trimmed();
                if(strData.contains(QRegExp(codec->toUnicode(c_szScoreRegExp))))
                {
                    pPersonalTrainingProgram->score = strData.trimmed();
                    //}
                    //else if(strData == codec->toUnicode("未修"))
                    //{
                    //    pPersonalTrainingProgram->score = strData.trimmed();
                    //}
                    //else if(strData == codec->toUnicode("选课"))
                    //{
                    //    pPersonalTrainingProgram->score = strData.trimmed();
                    //}
                    //else if(strData == codec->toUnicode("通过"))
                    //{
                    //    pPersonalTrainingProgram->score = strData.trimmed();
                }
                else
                {
                    qDebug() << __FILE__ << __LINE__ << "error:" << strData;
                    continue;
                }

                m_vecPersonalTrainProgram.push_back(pPersonalTrainingProgram->clone());
            }
        }
        else if(0)
        {
            qDebug() << strData.trimmed();
            //应修学分
            //完成学分
            //应修门数
            //完成门数
            //是否完成
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << "error:" << strData.trimmed();
        }
#else
        //QString strRegExp = QString(c_szNumberRegExp) + "\\s+" + codec->toUnicode("[\\x4e00-\\x9fa5]+")
        //        + "\\s+" + "\\d" + "\\s+" + codec->toUnicode(c_szScoreRegExp);

        QString strRegExp = QString(c_szNumberRegExp) + "\\s+" + "\\S+"
                + "\\s+" + "\\d" + "\\s+" + codec->toUnicode(c_szScoreRegExp);

        if(strData.contains(QRegExp(strRegExp)))
        {
            QStringList stringList = strData.split(" ", QString::SkipEmptyParts);
            if(stringList.size() == 4)
            {
                //
                pPersonalTrainingProgram->number = stringList.at(0).toInt();
                pPersonalTrainingProgram->name = stringList.at(1);
                pPersonalTrainingProgram->credit = stringList.at(2).toInt();
                pPersonalTrainingProgram->score = stringList.at(3);
            }
            else if(stringList.size() > 4)
            {
                if(stringList.at(1).toInt() != 0)
                {
                    pPersonalTrainingProgram->number = stringList.at(1).toInt();
                    pPersonalTrainingProgram->name = stringList.at(2);
                    pPersonalTrainingProgram->credit = stringList.at(3).toInt();
                    pPersonalTrainingProgram->score = stringList.at(4);
                }
                else if(stringList.at(2).toInt() != 0)
                {
                    pPersonalTrainingProgram->number = stringList.at(2).toInt();
                    pPersonalTrainingProgram->name = stringList.at(3);
                    pPersonalTrainingProgram->credit = stringList.at(4).toInt();
                    pPersonalTrainingProgram->score = stringList.at(5);
                }
                else
                {
                    qDebug() << __FILE__ << __LINE__ << strData.trimmed();
                    continue;
                }
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << strData.trimmed();
                continue;
            }

            m_vecPersonalTrainProgram.push_back(pPersonalTrainingProgram->clone());
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << strData.trimmed();
        }
#endif
    }
    return true;
}
