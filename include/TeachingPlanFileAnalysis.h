#ifndef TEACHINGPLANFILEANALYSIS_H
#define TEACHINGPLANFILEANALYSIS_H

#include "IFileAnalysis.h"

#include <QVector>

class QFile;

class TeachingPlanFileAnalysis : public IFileAnalysis
{
public:
    explicit TeachingPlanFileAnalysis();
    ~TeachingPlanFileAnalysis();

    bool analysis(const QString &fileName);

    QVector<IPersonalData*> getData();

private:
    //数据头
    bool analysisDataHead(QFile &file);
    //正文
    bool analysisDataContent(QFile &file);

private:
    QVector<IPersonalData*> m_vecPersonalTeachPlan;//本科生教学计划完成情况
};

#endif // TEACHINGPLANFILEANALYSIS_H
