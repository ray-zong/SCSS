#ifndef TRAININGPROGRAMFILEANALYSIS_H
#define TRAININGPROGRAMFILEANALYSIS_H

#include "IFileAnalysis.h"

#include <QVector>

class QFile;
class PersonalTrainingProgram;

class TrainingProgramFileAnalysis : public IFileAnalysis
{
public:
    explicit TrainingProgramFileAnalysis();
    ~TrainingProgramFileAnalysis();

    bool analysis(const QString &fileName);

    QVector<IPersonalData*> getData();

private:
    //数据头
    bool analysisDataHead(QFile &file);
    //正文
    bool analysisDataContent(QFile &file);

private:
    QVector<IPersonalData*> m_vecPersonalTrainProgram;//本科生培养方案完成情况
};

#endif // TRAININGPROGRAMFILEANALYSIS_H
