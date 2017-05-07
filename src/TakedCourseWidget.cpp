#include "TakedCourseWidget.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextCodec>
#include <QTableWidgetItem>
#include <QTableView>
#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "DataTypeDefine.h"
#include "SelectCourseDelegate.h"
#include "ConnectionPool.h"

TakedCourseWidget::TakedCourseWidget(QWidget *parent)
    : QWidget(parent)
    , m_pCourseTable(nullptr)
{
    initWidget();
}

TakedCourseWidget::~TakedCourseWidget()
{

}

void TakedCourseWidget::displayTakedCourseInfo()
{
    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return;
    }
    //查询数据
    QSqlQuery query(QString("SELECT DISTINCT "
                                "student_course.Course_Number,"
                                "course_info.Name,"
                                "student_course.Score,"
                                "specialty_course.Credit "
                            "FROM "
                                "student_course,"
                                "course_info,"
                                "specialty_course "
                            "WHERE "
                                "student_course.Course_Number = course_info.Number "
                            "AND "
                                "course_info.Number = specialty_course.Number "), db);

    QVector<IPersonalData *> vecTeachingPlan;
    while(query.next())
    {
        IPersonalData *pPersonalData = new IPersonalData;
        pPersonalData->number = query.value(0).toInt();
        pPersonalData->name = query.value(1).toString();
        pPersonalData->score = query.value(2).toString();
        pPersonalData->credit = query.value(3).toInt();

        vecTeachingPlan.push_back(pPersonalData);
    }

    if(!vecTeachingPlan.isEmpty())
    {
        setData(vecTeachingPlan);
        //释放vec
        foreach(IPersonalData *pPersonalData, vecTeachingPlan)
        {
            delete pPersonalData;
        }
        vecTeachingPlan.clear();
    }

    //关闭数据库
    ConnectionPool::closeConnection(db);
}

void TakedCourseWidget::setData(const QVector<IPersonalData *> &vecData)
{
    //指定QString的编码方式
    QTextCodec *codec = QTextCodec::codecForName("GBK");


    m_pCourseTable->setRowCount(0);

    QTableWidgetItem *pItem = NULL;

    int i = 0;
    for(auto ite = vecData.begin();
        ite != vecData.end(); ++ite)
    {
        if((*ite)->score == codec->toUnicode("未修"))
        {
            continue;
        }
        //设置行数
        m_pCourseTable->insertRow(i);

        //课程号
        pItem = new QTableWidgetItem(QString::number((*ite)->number));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pCourseTable->setItem(i, 0, pItem);

        //课程名
        pItem = new QTableWidgetItem((*ite)->name);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pCourseTable->setItem(i, 1, pItem);

        //学分
        pItem = new QTableWidgetItem(QString::number((*ite)->credit));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pCourseTable->setItem(i, 2, pItem);

        //成绩
        pItem = new QTableWidgetItem((*ite)->score);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pCourseTable->setItem(i, 3, pItem);

        ++i;
    }

    updateCourseData(vecData);
}

void TakedCourseWidget::updateCourseData(QVector<IPersonalData *> vecData)
{

    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return;
    }
    //删除课程表中数据
    QSqlQuery query(QString("DELETE FROM student_course"), db);
    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << "error:delete table(student_course) data failed!";
    }
    //插入数据
    if(!vecData.isEmpty())
    {
        //指定QString的编码方式
        QTextCodec *codec = QTextCodec::codecForName("GBK");

        QString sql("INSERT INTO student_course(Student_Number, Course_Number, Score)VALUES");
        foreach(IPersonalData *pPersonalData, vecData)
        {
            if(pPersonalData->score == codec->toUnicode("未修"))
            {
                continue;
            }

            sql += QString("(%1, %2, '%3')")
                    .arg("100000")
                    .arg(pPersonalData->number)
                    .arg(pPersonalData->score);
            sql += ",";
        }
        sql.remove(sql.size() - 1, 1);

        if(!query.exec(sql))
        {
            qDebug() << __FILE__ << __LINE__ << "error:insert data to table failed!";
        }
    }
    //关闭数据库
    ConnectionPool::closeConnection(db);
}

void TakedCourseWidget::initWidget()
{
    //课程信息//
    m_pCourseTable = new QTableWidget(this);
    {
        QHeaderView *pHeader = m_pCourseTable->horizontalHeader();
        pHeader->setStretchLastSection(true);
        pHeader->setSectionResizeMode(QHeaderView::Stretch);

        //不可编辑//
        m_pCourseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //课程号、课程名、学分、成绩//
        QStringList list;
        list << tr("Course ID")
             << tr("Course Name")
             << tr("Course Credit")
             << tr("Course Grade");

        //设置列数//
        m_pCourseTable->setColumnCount(list.count());
        m_pCourseTable->setHorizontalHeaderLabels(list);
    }

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pCourseTable);
}

