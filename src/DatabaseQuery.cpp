#include "DatabaseQuery.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "ConnectionPool.h"

DatabaseQuery *DatabaseQuery::m_pDatabaseQuery = nullptr;

DatabaseQuery::DatabaseQuery()
{

}

DatabaseQuery::~DatabaseQuery()
{

}

DatabaseQuery *DatabaseQuery::instance()
{
    return m_pDatabaseQuery;
}

void DatabaseQuery::disInstance()
{
    if(m_pDatabaseQuery != nullptr)
    {
        delete m_pDatabaseQuery;
        m_pDatabaseQuery = nullptr;
    }
}

QStringList DatabaseQuery::queryStringListByName(const QString &name)
{
    QStringList textList;
    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return textList;
    }
    //查询数据
    QSqlQuery query(QString("SELECT "
                            "course_info.Name "
                            "FROM "
                            "course_info "
                            "WHERE "
                            "course_info.Name LIKE '%%1%'").arg(name), db);
    while(query.next())
    {
        //qDebug() << __FILE__ << __LINE__ << "success";
        textList << query.value(0).toString();
    }

    //关闭数据库
    ConnectionPool::closeConnection(db);

    return textList;
}

bool DatabaseQuery::insertDataIntoStudentCourse(int number, const QString &score)
{
    bool result = false;
    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return result;
    }
    QSqlQuery query(db);
    //插入数据
    QString sql("INSERT INTO student_course(Student_Number, Course_Number, Score)VALUES");

    sql += QString("(%1, %2, '%3')")
            .arg("100000")
            .arg(number)
            .arg(score);

    if(!query.exec(sql))
    {
        qDebug() << __FILE__ << __LINE__ << "error:insert data to table failed!";
        result = true;
    }
    //关闭数据库
    ConnectionPool::closeConnection(db);

    return result;
}

int DatabaseQuery::queryNumberByName(const QString &name)
{
    int number = -1;
    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return number;
    }
    //查询数据
    QSqlQuery query(QString("SELECT "
                            "course_info.Number "
                            "FROM "
                            "course_info "
                            "WHERE "
                            "course_info.Name='%1'").arg(name), db);
    if(query.next())
    {
        number = query.value(0).toInt();
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << "error";
    }

    //关闭数据库
    ConnectionPool::closeConnection(db);

    return number;
}
