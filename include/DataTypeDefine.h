#ifndef IFILEANALYSIS_H
#define IFILEANALYSIS_H

#include <QString>

class IFileAnalysis
{
public:
    enum FileType
    {
        TeachingPlanFile = 0,
        TrainingProgramFile = 1
    };

public:
    static IFileAnalysis *createFileAnalysis(const FileType &);

    virtual bool analysis(const QString &fileName) = 0;

protected:
    explicit IFileAnalysis();
    ~IFileAnalysis();

private:
};

#endif // IFILEANALYSIS_H
