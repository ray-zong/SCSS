#ifndef DATATYPEDEFINE_H
#define DATATYPEDEFINE_H

#include <QPair>
#include <QString>

//学期
enum TermSeason
{
    Spring = 0,//春
    Summer = 1,//夏
    Autumn = 2,//秋
    Winter = 3 //冬
};

//课程属性
enum CourseAttribute
{
    Required = 0, //必修
    Elective = 1, //选修
    Optional = 2, //任选
    Limited = 3   //限选
};

/////////////////学生数据基类/////////////////////////////////
class IPersonalData
{
public:
    IPersonalData(){}
    virtual ~IPersonalData(){}
};
/////////本科生教学计划完成情况//////////////////////////////////
class PersonalTeachingPlan : public IPersonalData
{
public:
    QPair<int , int> year;//学年
    int term;//学期
    int number;//课程号
    QString name;//课程名
    int courseAttribute;//课程属性
    int credit;//学分
    double score;//总成绩
    QString group;//所属课组

    PersonalTeachingPlan* clone()
    {
        PersonalTeachingPlan* pCloneClass = new PersonalTeachingPlan;
        pCloneClass->year = this->year;
        pCloneClass->term = this->term;
        pCloneClass->number = this->number;
        pCloneClass->name = this->name;
        pCloneClass->courseAttribute = this->courseAttribute;
        pCloneClass->credit = this->credit;
        pCloneClass->score = this->score;
        pCloneClass->group = this->group;
        return pCloneClass;
    }

    virtual ~PersonalTeachingPlan(){}
};

const char c_szYearRegExp[] = "\\d{4}-\\d{4}学年";
const char c_szTermRegExp[] = "春|夏|秋|冬";
const char c_szNumberRegExp[] = "\\d{8}";
/////////////////////////////////////////////////////////////////


////////////////////////本科生培养方案/////////////////////////////
class PersonalTrainingProgram : public IPersonalData
{
public:
    //int courseAttribute;       //课程属性
    //QString group;             //课组名
    int number;                  //课程号
    QString name;                //课程名
    int credit;                  //学分
    QString score;               //成绩
    //int RepairedCredit;        //应修学分
    //int TakedCredit;           //完成学分
    //int RepairedCourseCount;   //应修门数
    //int TakedCourseCount;      //完成门数
    //bool isFinished;           //是否完成
    PersonalTrainingProgram* clone()
    {
        PersonalTrainingProgram* pCloneClass = new PersonalTrainingProgram;
        pCloneClass->number = this->number;
        pCloneClass->name = this->name;
        pCloneClass->credit = this->credit;
        pCloneClass->score = this->score;
        return pCloneClass;
    }

    virtual ~PersonalTrainingProgram(){}
};

const char c_szIsFinishedRegExp[] = "是|否";
const char c_szCourseAttrRegExp[] = "必修|限选|任选";
const char c_szScoreRegExp[] = "(\\d{1}|[1-9]\\d{1}|100)|(未修|选课|通过)";
/////////////////////////////////////////////////////////////////




///////////////////////数据库//////////////////////////////////
//specialty_course
class DB_SpecialtyCourse
{
public:
    int number;     //课程号
    int attribute;  //课程属性
    int specialty;  //所属专业
    int credit;     //学分
    int creditGroup;//选课分组
    QString term;   //学期 以','相连的数字串
};

//specialty_creditGroup
class DB_SpecialtyCreditGroup
{
public:
    int id;             //序列号
    int specialty;      //专业
    int group;          //学分组
    int requiredCredit; //应修学分
    int requiredNumber; //应修门数
};

/////////////////////////////////////////////////////////////////

#endif // DATATYPEDEFINE_H
