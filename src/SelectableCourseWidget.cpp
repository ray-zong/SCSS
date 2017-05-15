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
#include <QCheckBox>
#include <QPushButton>
#include <QTabWidget>
#include <QVector3D>
#include <QMessageBox>

#include "ConnectionPool.h"
#include "DataTypeDefine.h"
#include "SelectCourseDelegate.h"
#include "SelectedCourseResult.h"

SelectableCourseWidget::SelectableCourseWidget(QWidget *parent)
    : QWidget(parent)
    , m_pLabel_advise(NULL)
    , m_pPushButton(NULL)
    , m_pTabWidget(NULL)
    , m_pSelectedCourseResult(NULL)
    , m_nSpecialty(0)
    , m_nTerm(0)
{
    m_pLabel_advise = new QLabel(this);

    m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->setTabPosition(QTabWidget::South);

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pLabel_advise);
    pVLayout->addWidget(m_pTabWidget);

    QHBoxLayout *pHLayout = new QHBoxLayout;
    m_pPushButton = new QPushButton(this);
    connect(m_pPushButton, &QPushButton::clicked, this, &SelectableCourseWidget::checkSelectedCourse);
    m_pPushButton->setText(tr("Check"));
    pHLayout->addStretch();
    pHLayout->addWidget(m_pPushButton);

    pVLayout->addLayout(pHLayout);
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
        pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //课程号、课程名、课程属性、学分、建议学期、选课//
        QStringList list;
        list << tr("Course ID")
             << tr("Course Name")
             << tr("Course Attribute")
             << tr("Course Credit")
             << tr("Advise Term")
             << tr("Select");
        //设置列数//
        pTableWidget->setColumnCount(list.count());
        pTableWidget->setHorizontalHeaderLabels(list);
    }
    return pTableWidget;
}

QWidget *SelectableCourseWidget::addCheckWidget(int index, int group, int row)
{
    QWidget *pWidget = new QWidget();
    QCheckBox *pCheckBox = new QCheckBox();
    m_mapCheckboxToGroupRow[pCheckBox] = QVector3D(index, group, row);
    connect(pCheckBox, SIGNAL(clicked()), this, SLOT(checkboxClicked()));

    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(pCheckBox);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0,0,0,0);
    pWidget->setLayout(pLayout);
    return pWidget;
}

void SelectableCourseWidget::setSelectableCourseByGroup(int group, const QVector<DB_SpecialtyCourse> &vecSpecialtyCourse)
{
    Q_ASSERT(m_pTabWidget);

    QTableWidget *pTableWidget = createTableWidget();
    int tabIndex = m_pTabWidget->addTab(pTableWidget, QString("%1").arg(group));

    int row = 0;
    auto ite = vecSpecialtyCourse.begin();
    for(; ite != vecSpecialtyCourse.end(); ++ite, ++row)
    {
        pTableWidget->insertRow(row);
        //课程号//
        QTableWidgetItem *pItem = new QTableWidgetItem(QString::number(ite->number));
        pItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(row, 0, pItem);
        //课程名//
        pItem = new QTableWidgetItem(ite->name);
        pItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(row, 1, pItem);
        //课程属性//
        pItem = new QTableWidgetItem(ite->attribute);
        pItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(row, 2, pItem);
        //学分//
        pItem = new QTableWidgetItem(QString::number(ite->credit));
        pItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(row, 3, pItem);
        //学期//
        pItem = new QTableWidgetItem(ite->term);
        pItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(row, 4, pItem);
        //选课
        QWidget *pWidget = addCheckWidget(tabIndex, group, row);
        pTableWidget->setCellWidget(row, 5, pWidget);
    }
}

void SelectableCourseWidget::displayNotSelecteCourseMsg(const QString &strMsg)
{
    QMessageBox msgBox;
    msgBox.setText(strMsg);
    msgBox.exec();
}

QString SelectableCourseWidget::queryNameByNumber(int number)
{
    QString strName;

    QSqlDatabase db;
    if(!createConnection(db))
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return false;
    }

    QSqlQuery query(QString("SELECT DISTINCT "
                                "course_info.Name "
                            "FROM "
                                "course_info "
                            "WHERE "
                                "course_info.Number = %1").arg(number), db);
    while(query.next())
    {
        strName = query.value(0).toString();
    }

    //连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
    return strName;
}

bool SelectableCourseWidget::queryTakedCourseByNumber(int number)
{
    bool isFinished = false;
    QSqlDatabase db;
    if(!createConnection(db))
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return false;
    }

    QSqlQuery query(QString("SELECT DISTINCT "
                                "* "
                            "FROM "
                                "student_course "
                            "WHERE "
                                "student_course.Course_Number = %1").arg(number), db);
    while(query.next())
    {
        isFinished = true;
    }

    //连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return isFinished;
}

void SelectableCourseWidget::checkboxClicked()
{
    QCheckBox* pCheckBox = qobject_cast<QCheckBox *>(QObject::sender());

    bool isChecked = pCheckBox->isChecked();

    if(m_mapCheckboxToGroupRow.find(pCheckBox) == m_mapCheckboxToGroupRow.end())
    {
        qDebug() << __FILE__ << __LINE__ << "error";
        return;
    }

    QVector3D data = m_mapCheckboxToGroupRow[pCheckBox];
    //获取标签
    QTableWidget *pTableWidget = dynamic_cast<QTableWidget*>(m_pTabWidget->widget(data.x()));
    if(pTableWidget == NULL)
    {
        qDebug() << __FILE__ << __LINE__ << "error";
        return;
    }

    if(isChecked)
    {
        //检查先修课程是否完成
        //1.获取课程号
        //2.查找是否有先修课程
        //int number = pTableWidget->item(data.z(), 0)->text().toInt();
        int prerequisiteCourse_1 = m_mapGroupToSelectableCourse[data.y()].at(data.z()).prerequisiteCourse_1;
        int prerequisiteCourse_2 = m_mapGroupToSelectableCourse[data.y()].at(data.z()).prerequisiteCourse_2;
        bool bPreFinished = true;
        //3.先修课程是否完成
        if(prerequisiteCourse_1 != 0)
        {
            bPreFinished = queryTakedCourseByNumber(prerequisiteCourse_1);
        }

        if(!bPreFinished && prerequisiteCourse_2 != 0)
        {
            bPreFinished = queryTakedCourseByNumber(prerequisiteCourse_2);
        }

        if(!bPreFinished)
        {
            //完成先修课程
            QString strMsg = tr("You first need to finish the Course") + ":" + "\n";
            if(prerequisiteCourse_1 != 0)
            {
                strMsg += QString::number(prerequisiteCourse_1) + "\t" + queryNameByNumber(prerequisiteCourse_1);
                strMsg += "\n";
            }
            if(prerequisiteCourse_2 != 0)
            {
                strMsg += tr("Or") + "\n";
                strMsg += QString::number(prerequisiteCourse_2) + "\t" + queryNameByNumber(prerequisiteCourse_2);
            }

            displayNotSelecteCourseMsg(strMsg);
            pCheckBox->setChecked(false);
            return;
        }

        //检查当前选课是否与学期冲突
        QString strTerm = pTableWidget->item(data.z(), 4)->text().trimmed();
        QStringList strList = strTerm.split(',', QString::SkipEmptyParts);

        if(strList.count() == 1)
        {
            //奇数、偶数校验
            bool ok = false;
            int nAdviseTerm = strList.at(0).toInt(&ok);
            if(ok)
            {
                int diff = nAdviseTerm - m_nTerm - 1;
                if(diff % 2 != 0)
                {
                    displayNotSelecteCourseMsg(tr("You can not selecte the course") + "!");
                    pCheckBox->setChecked(false);
                    return;
                }
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << "error";
            }

        }
        else if(strList.count() > 1)
        {
            //包含
            qDebug() << __FILE__ << __LINE__ << "error";
            /*
            auto ite = strList.begin();
            for(; ite != strList.end(); ++ite)
            {
                if(ite->toInt() == m_nTerm)
                {
                    break;
                }
            }

            if(ite == strList.end())
            {
                //未找到
                displayNotSelecteCourseMsg("The course can not selected in the term!");
            }
            */
        }
        else
        {
            //非法
            qDebug() << __FILE__ << __LINE__ << "error";
        }
    }

    //设置选中行的颜色//
    {
        for(int i = 0; i < pTableWidget->columnCount() - 1; ++i)
        {
            QTableWidgetItem *pItem = pTableWidget->item(data.z(), i);
            if(pItem != NULL)
            {
                pItem->setBackgroundColor(isChecked ? QColor(255, 200, 200) : QColor(255, 255, 255));
            }
        }
    }


    if(isChecked)
    {
        //加入已选课组//
        if(m_mapGroupToSpecialtyCourse.find(data.y()) ==
                m_mapGroupToSpecialtyCourse.end())
        {
            m_mapGroupToSpecialtyCourse[data.y()] = QVector<DB_SpecialtyCourse>();
        }

        DB_SpecialtyCourse specialtyCourse;
        specialtyCourse.number = pTableWidget->item(data.z(), 0)->text().toInt();
        specialtyCourse.name = pTableWidget->item(data.z(), 1)->text();
        specialtyCourse.attribute = pTableWidget->item(data.z(), 2)->text();
        specialtyCourse.credit = pTableWidget->item(data.z(), 3)->text().toInt();

        m_mapGroupToSpecialtyCourse[data.y()].push_back(specialtyCourse);
    }
    else
    {
        //删除
        if(m_mapGroupToSpecialtyCourse.find(data.y()) ==
                m_mapGroupToSpecialtyCourse.end())
        {
            qDebug() << __FILE__ << __LINE__ << "error";
            return;
        }

        for(auto ite = m_mapGroupToSpecialtyCourse[data.y()].begin();
            ite != m_mapGroupToSpecialtyCourse[data.y()].end();
            ++ite)
        {
            if(ite->number == pTableWidget->item(data.z(), 0)->text().toInt()
                    && ite->name == pTableWidget->item(data.z(), 1)->text()
                    && ite->credit == pTableWidget->item(data.z(), 3)->text().toInt())
            {
                m_mapGroupToSpecialtyCourse[data.y()].erase(ite);
                break;
            }
        }
    }
}

void SelectableCourseWidget::checkSelectedCourse()
{
    if(m_pSelectedCourseResult == NULL)
    {
        m_pSelectedCourseResult = new SelectedCourseResult(this);
    }

    QVector<DB_SpecialtyCourse> vecCourse;
    for(auto i = m_mapGroupToSpecialtyCourse.begin(); i != m_mapGroupToSpecialtyCourse.end(); ++i)
    {
        vecCourse.append(i.value());
    }

    if(vecCourse.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Nothing"));
        msgBox.exec();
        return;
    }
    m_pSelectedCourseResult->displaySelectedCourse(vecCourse);
    m_pSelectedCourseResult->show();
}

bool SelectableCourseWidget::displaySelectCourseInfo(int specialty, int term)
{
    m_nSpecialty = specialty;
    m_nTerm = term;
    m_mapGroupToTakedCourse.clear();
    m_mapCheckboxToGroupRow.clear();
    m_mapGroupToSpecialtyCourse.clear();
    m_mapGroupToSelectableCourse.clear();
    m_mapGroupTospecialtyCreditGroup.clear();


    QSqlDatabase db;
    if(!createConnection(db))
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return false;
    }

    //课程号、学分、学分分组
    //1.根据专业选择所有课程(已修)
    QSqlQuery takedCourseQuery(QString("SELECT DISTINCT "
                                       "student_course.Course_Number,"
                                       "specialty_course.Credit,"
                                       "specialty_course.CreditGroup "
                                       "FROM "
                                       "student_course,"
                                       "specialty_course "
                                       "WHERE "
                                       "student_course.Course_Number = specialty_course.Number "
                                       "AND "
                                       "specialty_course.Specialty = %1").arg(specialty), db);
    while(takedCourseQuery.next())
    {
        DB_SpecialtyCourse specialtyCourse;
        specialtyCourse.number = takedCourseQuery.value(0).toInt();
        specialtyCourse.credit = takedCourseQuery.value(1).toInt();
        specialtyCourse.creditGroup = takedCourseQuery.value(2).toInt();

        if(m_mapGroupToTakedCourse.find(specialtyCourse.creditGroup) ==
                m_mapGroupToTakedCourse.end())
        {
            m_mapGroupToTakedCourse[specialtyCourse.creditGroup] = QVector<DB_SpecialtyCourse>();
        }

        m_mapGroupToTakedCourse[specialtyCourse.creditGroup].push_back(specialtyCourse);
    }

    //2.根据专业选择所有课程(未修)
    QSqlQuery query(QString("SELECT DISTINCT "
                            "specialty_course.Number,"
                            "course_info.Name,"
                            "course_attribute.Attribute,"
                            "specialty_course.Credit,"
                            "specialty_course.Term,"
                            "specialty_course.CreditGroup,"
                            "specialty_course.PrerequisiteCourse_1,"
                            "specialty_course.PrerequisiteCourse_2 "
                            "FROM "
                            "specialty_course,"
                            "course_info,"
                            "course_attribute "
                            "WHERE "
                            "specialty_course.Specialty = %1 "
                            "AND "
                            "course_attribute.ID = specialty_course.Attribute "
                            "AND "
                            "specialty_course.Number = course_info.Number "
                            "AND (specialty_course.Number NOT IN "
                            "(SELECT "
                            "student_course.Course_Number "
                            "FROM "
                            "student_course))").arg(specialty), db);

    while(query.next())
    {
        DB_SpecialtyCourse specialtyCourse;
        specialtyCourse.number = query.value(0).toInt();
        specialtyCourse.name = query.value(1).toString();
        specialtyCourse.attribute = query.value(2).toString();
        specialtyCourse.credit = query.value(3).toInt();
        specialtyCourse.term = query.value(4).toString();
        specialtyCourse.creditGroup = query.value(5).toInt();
        specialtyCourse.prerequisiteCourse_1 = query.value(6).toInt();
        specialtyCourse.prerequisiteCourse_2 = query.value(7).toInt();
        if(m_mapGroupToSelectableCourse.find(specialtyCourse.creditGroup) ==
                m_mapGroupToSelectableCourse.end())
        {
            m_mapGroupToSelectableCourse[specialtyCourse.creditGroup] = QVector<DB_SpecialtyCourse>();
        }

        m_mapGroupToSelectableCourse[specialtyCourse.creditGroup].push_back(specialtyCourse);
    }

    //查询课组学分要求//
    QSqlQuery queryGroup(QString("SELECT DISTINCT "
                                 "\"Group\", Required_credit, Required_number "
                                 "FROM "
                                 "specialty_creditGroup "
                                 "WHERE "
                                 "specialty=%1").arg(specialty), db);

    while(queryGroup.next())
    {
        DB_SpecialtyCreditGroup specialtyCreditGroup;
        specialtyCreditGroup.group = queryGroup.value(0).toInt();
        specialtyCreditGroup.requiredCredit = queryGroup.value(1).toInt();
        specialtyCreditGroup.requiredNumber = queryGroup.value(2).toInt();
        specialtyCreditGroup.specialty = specialty;

        m_mapGroupTospecialtyCreditGroup[specialtyCreditGroup.group] = specialtyCreditGroup;
    }

    m_pTabWidget->clear();
    //必修课学分不做判读直接加入表中
    setSelectableCourseByGroup(0, m_mapGroupToSelectableCourse[0]);
    //根据每个选课分组，查询已修的学分是否满足要求，若满足提示通过，若不满足，提示还需多少学分
    QString strText = tr("Advise") + ":" + "\n";
    for(auto i = m_mapGroupTospecialtyCreditGroup.begin();
        i != m_mapGroupTospecialtyCreditGroup.end();
        ++i)
    {
        if(!m_mapGroupToTakedCourse[i.key()].isEmpty())
        {
            int sum = 0;
            for(auto j = m_mapGroupToTakedCourse[i.key()].begin(); j != m_mapGroupToTakedCourse[i.key()].end(); ++j)
            {
                sum += (*j).credit;
            }

            //学分不够
            if(sum < i.value().requiredCredit)
            {
                strText += tr("CreditGroup:") + QString("%1").arg(i.key()) + "\t" + tr("Required credits:") + QString("%1").arg(i.value().requiredCredit)
                        + "\t" + tr("Completed credits:") + QString("%1").arg(sum) + "\t" + tr("Remain credits:") + QString("%1").arg(i.value().requiredCredit - sum) + "\n";
                //在标签页中添加选课数据
                setSelectableCourseByGroup(i.key(), m_mapGroupToSelectableCourse[i.key()]);
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << "CreditGroup" << i.key() << "credit has required!";
            }
        }
        else
        {
            //未修该课组
            strText += tr("CreditGroup:") + QString("%1").arg(i.key()) + "\t" + tr("Required credits:") + QString("%1").arg(i.value().requiredCredit)
                    + "\t" + tr("Completed credits:") +"0" + "\t" + tr("Remain credits:") + QString("%1").arg(i.value().requiredCredit) + "\n";
            //在标签页中添加选课数据
            setSelectableCourseByGroup(i.key(), m_mapGroupToSelectableCourse[i.key()]);
        }
    }
    m_pLabel_advise->setText(strText);

    //连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return true;
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
