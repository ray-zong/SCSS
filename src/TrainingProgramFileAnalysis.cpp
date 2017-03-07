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
            qDebug() << strData;
            return true;
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
        QString strData = codec->toUnicode(line);

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
    }
    return true;
}
