#include "IFileAnalysis.h"

#include "TeachingPlanFileAnalysis.h"
#include "TrainingProgramFileAnalysis.h"

IFileAnalysis::IFileAnalysis()
{

}

IFileAnalysis::~IFileAnalysis()
{

}

IFileAnalysis *IFileAnalysis::createFileAnalysis(const FileType &fileType)
{
    switch(fileType)
    {
    case TeachingPlanFile:
        return new TeachingPlanFileAnalysis;

    case TrainingProgramFile:
        return new TrainingProgramFileAnalysis;

    default:
        return NULL;
    }

    return NULL;
}


