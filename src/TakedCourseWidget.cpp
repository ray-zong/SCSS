#include "TakedCourseWidget.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidgetItem>

#include "DataTypeDefine.h"

TakedCourseWidget::TakedCourseWidget(QWidget *parent)
    : QWidget(parent)
    , m_pTableWidget_teachingPlan(NULL)
    , m_pTableWidget_trainingProgram(NULL)
{
    initWidget();
}

TakedCourseWidget::~TakedCourseWidget()
{

}

void TakedCourseWidget::displayTeachingPlan(QVector<IPersonalData *> vecTeachingPlan)
{
    Q_ASSERT(m_pTableWidget_teachingPlan != NULL);

    //设置行数
    m_pTableWidget_teachingPlan->setRowCount(vecTeachingPlan.size());

    QTableWidgetItem *pItem = NULL;

    int i = 0;
    for(auto ite = vecTeachingPlan.begin();
        ite != vecTeachingPlan.end(); ++i, ++ite)
    {
        PersonalTeachingPlan *pTeachingPlan = dynamic_cast<PersonalTeachingPlan*>(*ite);
        if(pTeachingPlan == NULL)
            continue;
        //学年
        pItem = new QTableWidgetItem(QString::number(pTeachingPlan->year.first) + "-" + QString::number(pTeachingPlan->year.second));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 0, pItem);

        //学期
        pItem = new QTableWidgetItem(QString::number(pTeachingPlan->term));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 1, pItem);

        //课程号
        pItem = new QTableWidgetItem(QString::number(pTeachingPlan->number));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 2, pItem);

        //课程名
        pItem = new QTableWidgetItem(pTeachingPlan->name);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 3, pItem);

        //课程属性
        pItem = new QTableWidgetItem(QString::number(pTeachingPlan->courseAttribute));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 4, pItem);

        //学分
        pItem = new QTableWidgetItem(QString::number(pTeachingPlan->credit));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 5, pItem);

        //总成绩
        pItem = new QTableWidgetItem(QString::number(pTeachingPlan->score));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 6, pItem);

        //所属课组
        pItem = new QTableWidgetItem(pTeachingPlan->group);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_teachingPlan->setItem(i, 7, pItem);
    }
}

void TakedCourseWidget::displayTrainingProgram(const QVector<IPersonalData *> &vecTrainingProgram)
{
    Q_ASSERT(m_pTableWidget_trainingProgram != NULL);

    //设置行数
    m_pTableWidget_trainingProgram->setRowCount(vecTrainingProgram.size());

    QTableWidgetItem *pItem = NULL;

    int i = 0;
    for(auto ite = vecTrainingProgram.begin();
        ite != vecTrainingProgram.end(); ++ite, ++i)
    {
        PersonalTrainingProgram *pTrainingProgram = dynamic_cast<PersonalTrainingProgram*>(*ite);
        if(pTrainingProgram == NULL)
            continue;
        //课程号
        pItem = new QTableWidgetItem(QString::number(pTrainingProgram->number));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_trainingProgram->setItem(i, 0, pItem);

        //课程名
        pItem = new QTableWidgetItem(pTrainingProgram->name);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_trainingProgram->setItem(i, 1, pItem);

        //学分
        pItem = new QTableWidgetItem(QString::number(pTrainingProgram->credit));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_trainingProgram->setItem(i, 2, pItem);

        //成绩
        pItem = new QTableWidgetItem(pTrainingProgram->score);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget_trainingProgram->setItem(i, 3, pItem);
    }
}

void TakedCourseWidget::initWidget()
{
    //教学计划//
    QLabel *pLabel_teachingPlan = new  QLabel(tr("Teaching Plan"));
    m_pTableWidget_teachingPlan = new QTableWidget(this);
    {
        QHeaderView *pHeader = m_pTableWidget_teachingPlan->horizontalHeader();
        pHeader->setStretchLastSection(true);
        pHeader->setSectionResizeMode(QHeaderView::Stretch);
        //不可编辑//
        m_pTableWidget_teachingPlan->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //学年、学期、课程号、课程名、课程属性、学分、成绩、所属课组//
        QStringList list;
        list << tr("Year")
             << tr("Term")
             << tr("Course ID")
             << tr("Course Name")
             << tr("Course Attribute")
             << tr("Course Credit")
             << tr("Course Grade")
             << tr("Course Group");

        //设置列数//
        m_pTableWidget_teachingPlan->setColumnCount(list.count());
        m_pTableWidget_teachingPlan->setHorizontalHeaderLabels(list);
    }


    //培训方案//
    QLabel *pLabel_trainingProgram = new QLabel(tr("Training Program"));
    m_pTableWidget_trainingProgram = new QTableWidget(this);
    {
        QHeaderView *pHeader = m_pTableWidget_trainingProgram->horizontalHeader();
        pHeader->setStretchLastSection(true);
        pHeader->setSectionResizeMode(QHeaderView::Stretch);

        //不可编辑//
        m_pTableWidget_trainingProgram->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //课程号、课程名、学分、成绩//
        QStringList list;
        list << tr("Course ID")
             << tr("Course Name")
             << tr("Course Credit")
             << tr("Course Grade");

        //设置列数//
        m_pTableWidget_trainingProgram->setColumnCount(list.count());
        m_pTableWidget_trainingProgram->setHorizontalHeaderLabels(list);

        //设置行数//
        //m_pTableWidget->setRowCount();
    }

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(pLabel_teachingPlan);
    pVLayout->addWidget(m_pTableWidget_teachingPlan);

    pVLayout->addWidget(pLabel_trainingProgram);
    pVLayout->addWidget(m_pTableWidget_trainingProgram);
}

