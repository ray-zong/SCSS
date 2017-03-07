#include "TeachingPlanFileAnalysis.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QTextCodec>
#include <QStringList>

#include "DataTypeDefine.h"

TeachingPlanFileAnalysis::TeachingPlanFileAnalysis()
{

}

TeachingPlanFileAnalysis::~TeachingPlanFileAnalysis()
{
    //删除数据
    for(auto i = m_vecPersonalTeachPlan.begin();
             i != m_vecPersonalTeachPlan.end(); ++i)
    {
        delete *i;
    }
    m_vecPersonalTeachPlan.clear();
}

bool TeachingPlanFileAnalysis::analysis(const QString &fileName)
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

QVector<IPersonalData *> TeachingPlanFileAnalysis::getData()
{
    return m_vecPersonalTeachPlan;
}

bool TeachingPlanFileAnalysis::analysisDataHead(QFile &file)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");//指定QString的编码方式

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString strData = codec->toUnicode(line);
        if(strData.contains(codec->toUnicode("所属课组")))
        {
            qDebug() << __FILE__ << __LINE__ << strData;
            return true;
        }
    }
    return false;
}

bool TeachingPlanFileAnalysis::analysisDataContent(QFile &file)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");//指定QString的编码方式

    PersonalTeachingPlan *pPersonalTeachPlan = new PersonalTeachingPlan;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString strData = codec->toUnicode(line);
        //学年
        if(strData.contains(QRegExp(codec->toUnicode(c_szYearRegExp))))
        {
            QStringList list = strData.split('-');
            if(list.size() == 2)
            {
                pPersonalTeachPlan->year.first = list.at(0).toInt();
                pPersonalTeachPlan->year.second = list.at(1).left(4).toInt();
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << "year error";
            }
        }else if(strData.contains(QRegExp(codec->toUnicode(c_szTermRegExp))))
        {
            //学期
            if(strData.trimmed() == codec->toUnicode("春"))
            {
                pPersonalTeachPlan->term = Spring;
            }
            else if(strData.trimmed() == codec->toUnicode("夏"))
            {
                pPersonalTeachPlan->term = Summer;
            }
            else if(strData.trimmed() == codec->toUnicode("秋"))
            {
                pPersonalTeachPlan->term = Autumn;
            }
            else if(strData.trimmed() == codec->toUnicode("冬"))
            {
                pPersonalTeachPlan->term = Winter;
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << "term error";
            }
        }
        else if(strData.contains(QRegExp(codec->toUnicode(c_szNumberRegExp))))
        {
            //课程号
            pPersonalTeachPlan->number = strData.toInt();
            //课程名
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                pPersonalTeachPlan->name = codec->toUnicode(line).trimmed();
            }
            //课程属性
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString strData = codec->toUnicode(line).trimmed();
                if(strData == codec->toUnicode("必修"))
                {
                    pPersonalTeachPlan->courseAttribute = Required;
                }
                else if(strData == codec->toUnicode("选修"))
                {
                    pPersonalTeachPlan->courseAttribute = Elective;
                }
                else if(strData == codec->toUnicode("任选"))
                {
                    pPersonalTeachPlan->courseAttribute = Optional;
                }
                else if(strData == codec->toUnicode("限选"))
                {
                    pPersonalTeachPlan->courseAttribute = Limited;
                }
                else
                {
                    qDebug() << __FILE__ << __LINE__ << "coureAttribute error";
                    continue;
                }
            }
            //学分
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString strData = codec->toUnicode(line);
                pPersonalTeachPlan->credit = strData.toInt();
            }
            //总成绩
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString strData = codec->toUnicode(line);
                pPersonalTeachPlan->score = strData.toInt();
            }
            //所属课组
            if(!file.atEnd())
            {
                QByteArray line = file.readLine();
                pPersonalTeachPlan->group = codec->toUnicode(line).trimmed();

                m_vecPersonalTeachPlan.push_back(pPersonalTeachPlan->clone());
            }
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << "coureAttribute error";
        }
    }
    return true;
}
