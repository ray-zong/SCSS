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
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

#include "DataTypeDefine.h"
#include "SelectCourseDelegate.h"
#include "ConnectionPool.h"
#include "AddCourseWidget.h"
#include "ModifyCourseWidget.h"

TakedCourseWidget::TakedCourseWidget(QWidget *parent)
    : QWidget(parent)
    , m_pCourseTable(nullptr)
    , m_pPushButton_Add(nullptr)
    , m_pPushButton_Delete(nullptr)
    , m_pPushButton_Modify(nullptr)
    , m_pAddCourseWidget(nullptr)
    , m_pModifyCourseWidget(nullptr)
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


    m_pCourseTable->clearContents();
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
}

bool TakedCourseWidget::deleteCourseByNumber(int number)
{
    bool result = false;
    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return result;
    }
    //查询数据
    QSqlQuery query(QString("DELETE "
                            "FROM "
                                "student_course "
                            "WHERE "
                                "student_course.Course_Number = %1").arg(number), db);
    if(query.exec())
    {
        //qDebug() << __FILE__ << __LINE__ << "success";
        result  = true;
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << "failed";
        result  = false;
    }

    //关闭数据库
    ConnectionPool::closeConnection(db);
    return result;
}

void TakedCourseWidget::deleteCourse()
{
    Q_ASSERT(m_pCourseTable);

    //当前行号
    int row = m_pCourseTable->currentRow();
    if(row == -1)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Please select a row of the table!"));
        msgBox.exec();
        return;
    }
    //课程号
    int number = m_pCourseTable->item(row, 0)->text().toInt();
    //课程名
    QString name = m_pCourseTable->item(row, 1)->text();

    QMessageBox msgBox;
    msgBox.setText(tr("course ID") + ":" + QString::number(number) + "\n" + tr("course name") + ":" + name);
    msgBox.setInformativeText(tr("Do you want to delete the course?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        //ok:执行删除操作
        if(deleteCourseByNumber(number))
        {
            m_pCourseTable->removeRow(row);
            emit takedCourseChanged();
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << "error";
        }
    }
    else if(ret == QMessageBox::Cancel)
    {
        //cancel:不进行任何操作
        return;
    }
    else
    {
        //error
        qDebug() << __FILE__ << __LINE__ << "error";
    }
}

void TakedCourseWidget::modifyCourse()
{
    if(m_pModifyCourseWidget == nullptr)
    {
        m_pModifyCourseWidget = new ModifyCourseWidget(this);
    }
    Q_ASSERT(m_pCourseTable);

    //当前行号
    int row = m_pCourseTable->currentRow();
    if(row == -1)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Please select a row of the table!"));
        msgBox.exec();
        return;
    }
    //课程号
    int number = m_pCourseTable->item(row, 0)->text().toInt();
    //课程名
    QString name = m_pCourseTable->item(row, 1)->text();
    //成绩
    QString score = m_pCourseTable->item(row, 3)->text();

    m_pModifyCourseWidget->setCourseNumber(number);
    m_pModifyCourseWidget->setCourseName(name);
    m_pModifyCourseWidget->setCourseScore(score);
    int result = m_pModifyCourseWidget->exec();

    if(result == QDialog::Accepted)
    {
        displayTakedCourseInfo();
        emit takedCourseChanged();
    }
    else if(result == QDialog::Rejected)
    {
        qDebug() << __FILE__ << __LINE__ << "Rejected";
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << "error";
    }
}

void TakedCourseWidget::addCourse()
{
    if(m_pAddCourseWidget == nullptr)
    {
        m_pAddCourseWidget = new AddCourseWidget(this);
    }

    int result = m_pAddCourseWidget->exec();

    if(result == QDialog::Accepted)
    {
        displayTakedCourseInfo();
        emit takedCourseChanged();
    }
    else if(result == QDialog::Rejected)
    {
        qDebug() << __FILE__ << __LINE__ << "Rejected";
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << "error";
    }
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

    m_pPushButton_Add = new QPushButton(tr("Add Course"));
    connect(m_pPushButton_Add, SIGNAL(clicked()), this, SLOT(addCourse()));
    m_pPushButton_Delete = new QPushButton(tr("Delete Course"));
    connect(m_pPushButton_Delete, SIGNAL(clicked()), this, SLOT(deleteCourse()));
    m_pPushButton_Modify = new QPushButton(tr("Modify Score"));
    connect(m_pPushButton_Modify, SIGNAL(clicked()), this, SLOT(modifyCourse()));
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addStretch();
    pHLayout->addWidget(m_pPushButton_Add);
    pHLayout->addWidget(m_pPushButton_Delete);
    pHLayout->addWidget(m_pPushButton_Modify);

    pVLayout->addLayout(pHLayout);
}

