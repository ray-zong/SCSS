#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

#include "DataTypeDefine.h"
#include "ConnectionPool.h"

//单例实现
class DatabaseQuery
{
public:
    static DatabaseQuery *instance();

    static void disInstance();

    //模糊匹配:课程名(course_info)
    QStringList queryStringListByName(const QString &name);
    //插入数据（student_course）
    bool insertDataIntoStudentCourse(int number, const QString &score);
    //通过课程名查找课程号(course_info)
    int queryNumberByName(const QString &name);

protected:
    explicit DatabaseQuery();
    ~DatabaseQuery();

private:
    static DatabaseQuery *m_pDatabaseQuery;
};

#endif // DATABASEQUERY_H
