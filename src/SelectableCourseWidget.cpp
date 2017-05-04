#include "SelectableCourseWidget.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QMap>

#include "ConnectionPool.h"
#include "DataTypeDefine.h"
#include "SelectCourseDelegate.h"

SelectableCourseWidget::SelectableCourseWidget(QWidget *parent)
    : QWidget(parent)
    , m_pLabel_advise(NULL)
    , m_pTableWidget(NULL)
    , m_nSpecialty(0)
    , m_nTerm(0)
{
    m_pLabel_advise = new QLabel(this);
    m_pLabel_advise->setText(tr("Advise") + ":" + "\n");
    m_pTableWidget = createTableWidget();
    m_pTableWidget->setItemDelegateForColumn(5, new SelectCourseDelegate);

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pLabel_advise);
    pVLayout->addWidget(m_pTableWidget);
}

SelectableCourseWidget::~SelectableCourseWidget()
{

}

QTableWidget * SelectableCourseWidget::createTableWidget()
{
    QTableWidget *pTableWidget = new QTableWidget(this);
    {
        QHeaderView *pHeader = pTableWidget->horizontalHeader();
        pHeader->setStretchLastSection(true);
        pHeader->setSectionResizeMode(QHeaderView::Stretch);

        //不可编辑//
        //pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //课程号、课程名、课程属性、学分、学期//
        QStringList list;
        list << tr("Course ID")
             << tr("Course Name")
             << tr("Course Attribute")
             << tr("Course Credit")
             << tr("Term")
             << tr("Select");
        //设置列数//
        pTableWidget->setColumnCount(list.count());
        pTableWidget->setHorizontalHeaderLabels(list);
    }
    return pTableWidget;
}

bool SelectableCourseWidget::selectCourseAnalysis()
{
    Q_ASSERT(m_pTableWidget != NULL);
    //连接数据库
    QSqlDatabase db;
    if(!createConnection(db))
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return false;
    }

    QTableWidgetItem *pItem = NULL;
    //1.依据专业查找所有课程
    QMap<int, QVector<DB_SpecialtyCourse> > mapToVecSpecialtyCourse;
    QSqlQuery query(QString("SELECT "
                                "Number,Attribute,Credit,Term,CreditGroup "
                            "FROM "
                                "specialty_course "
                            "WHERE "
                                "Specialty=%1").arg(m_nSpecialty), db);
    int i = 0;
    m_pTableWidget->setRowCount(0);
    m_pTableWidget->clearContents();
    for(; query.next(); ++i)
    {
        DB_SpecialtyCourse specialtyCourse;
        specialtyCourse.number = query.value(0).toInt();
        specialtyCourse.attribute = query.value(1).toInt();
        specialtyCourse.credit = query.value(2).toInt();
        specialtyCourse.term = query.value(3).toString();
        specialtyCourse.creditGroup = query.value(4).toInt();
        specialtyCourse.specialty = m_nSpecialty;

        m_pTableWidget->insertRow(i);
        QStringList items;
        //课程号
        items << query.value(0).toString();
        //课程名
        QSqlQuery queryName(QString("SELECT Name,\"Group\" FROM course_info WHERE Number=%1").arg(query.value(0).toInt()), db);
        if(queryName.next())
        {
            items << queryName.value(0).toString();
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << "Course number:" << query.value(0);
            items << "";
        }
        //课程属性
        QSqlQuery queryAttribute(QString("SELECT Attribute FROM course_attribute WHERE ID=%1").arg(query.value(1).toInt()), db);
        if(queryAttribute.next())
        {
            items << queryAttribute.value(0).toString();
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << "Course Attribute error:" << query.value(1);
        }
        //学分
        items << query.value(2).toString();
        //学期
        items << query.value(3).toString();

        for(int j = 0; j < items.size(); ++j)
        {
            //课程号、课程名、课程属性、学分、学期//
            pItem = new QTableWidgetItem(items.at(j));
            pItem->setTextAlignment(Qt::AlignCenter);
            m_pTableWidget->setItem(i, j, pItem);
        }
        //选课
        pItem = new QTableWidgetItem();
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 5, pItem);
        m_pTableWidget->openPersistentEditor(pItem);


        //first
        if(mapToVecSpecialtyCourse.find(specialtyCourse.creditGroup) == mapToVecSpecialtyCourse.end())
        {
            mapToVecSpecialtyCourse[specialtyCourse.creditGroup] = QVector<DB_SpecialtyCourse>();
        }

        mapToVecSpecialtyCourse[specialtyCourse.creditGroup].push_back(specialtyCourse);
    }

    if(i == 0)
    {
        return false;
    }

    QMap<int, DB_SpecialtyCreditGroup> mapCreditGroup;
    //2.查找不同的学分分组应修学分,应修门数
    query.prepare(QString("SELECT Group,Required_credit,Required_number FROM specialty_creditGroup WHERE Specialty=%1").arg(m_nSpecialty));
    while(query.next())
    {
        DB_SpecialtyCreditGroup creditGroup;
        creditGroup.group = query.value(0).toInt();
        creditGroup.requiredCredit = query.value(1).toInt();
        creditGroup.requiredNumber = query.value(2).toInt();
        creditGroup.specialty = m_nSpecialty;
        mapCreditGroup[creditGroup.group] = creditGroup;
    }

    //3.依据选课分组排列所有课程
    QString text = m_pLabel_advise->text();
    for(int i = 1; i < 9; ++i)
    {
        auto ite = mapCreditGroup.find(i);
        if(ite != mapCreditGroup.end())
        {
            //QVector<DB_SpecialtyCourse> &vecSpecialtyCourse = mapToVecSpecialtyCourse[ite.value()];
            //int creditResult = 0;
            //for(auto vecIte = vecSpecialtyCourse.begin(); vecIte != vecSpecialtyCourse.end(); ++vecIte)
            //{
            //    creditResult += vecIte->credit;
            //    if(creditResult >= ite.value().requiredCredit)
            //    {
            //        //已修够学分
            //        break;
            //    }
            //}
        }
    }

    //连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return true;
}

bool SelectableCourseWidget::displaySelectCourseInfo(int specialty, int term)
{
    m_nSpecialty = specialty;
    m_nTerm = term;

    QSqlDatabase db;
    if(!createConnection(db))
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return false;
    }

    //课程号、课程名、课程属性、学分、学期
    //1.根据专业选择所有课程
    QSqlQuery query(QString("SELECT DISTINCT "
                    "specialty_course.Number,"
                    "course_info.Name,"
                    "course_attribute.Attribute,"
                    "specialty_course.Credit,"
                    "specialty_course.Term "
                "FROM "
                    "specialty_course,"
                    "course_info,"
                    "course_attribute "
                "WHERE "
                    "specialty_course.Specialty = %1 "
                "AND "
                  "course_attribute.ID = specialty_course.Attribute "
                "AND "
                    "specialty_course.Number = course_info.Number").arg(specialty), db);
    //2.根据已修课程移除课程
    //3.根据学期移除课程

    QTableWidgetItem *pItem = NULL;
    m_pTableWidget->setRowCount(0);
    m_pTableWidget->clearContents();
    int i = 0;
    while(query.next())
    {
        m_pTableWidget->insertRow(i);
        //课程号//
        //课程名//
        //课程属性//
        //学分//
        //学期//
        for(int j = 0; j < 5; ++j)
        {
            pItem = new QTableWidgetItem(query.value(j).toString());
            pItem->setTextAlignment(Qt::AlignCenter);
            m_pTableWidget->setItem(i, j, pItem);
        }
        //选课
        pItem = new QTableWidgetItem();
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 5, pItem);
        m_pTableWidget->openPersistentEditor(pItem);

        ++i;
    }

    if(i == 0)
    {
        qDebug() << __FILE__ << __LINE__ << "error";
        return false;
    }

    //连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return true;
}

void SelectableCourseWidget::setTakedCourseData(const QVector<IPersonalData *> &vecPersonalData)
{
   m_vecPersonalData = vecPersonalData;
}

bool SelectableCourseWidget::createConnection(QSqlDatabase &db)
{
    //从数据库连接池里取得连接
    db = ConnectionPool::openConnection();

    if(!db.isValid())
    {
        QMessageBox::critical(0, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connecttion.\n"
                                          "This example needs SQLite support. Please read"
                                          "the Qt SQL driver documentation for information how"
                                          "to build it.\n\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    return true;
}
