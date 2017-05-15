#include "SelectedCourseResult.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QHeaderView>
#include <QTableWidget>

SelectedCourseResult::SelectedCourseResult(QWidget *parent)
    : QDialog(parent)
    , m_pLabel(NULL)
    , m_pTableWidget(NULL)
{
    initWidget();
}

SelectedCourseResult::~SelectedCourseResult()
{

}

void SelectedCourseResult::displaySelectedCourse(const QVector<DB_SpecialtyCourse> &vecCourse)
{
    Q_ASSERT(m_pLabel);

    m_pTableWidget->clearContents();
    m_pTableWidget->setRowCount(0);

    int row = 0;
    int sum = 0;
    for(auto ite = vecCourse.begin(); ite != vecCourse.end(); ++ite, ++row)
    {
        m_pTableWidget->insertRow(row);
        //课程号//
        QTableWidgetItem *pItem = new QTableWidgetItem(QString::number(ite->number));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(row, 0, pItem);
        //课程名//
        pItem = new QTableWidgetItem(ite->name);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(row, 1, pItem);
        //课程属性//
        pItem = new QTableWidgetItem(ite->attribute);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(row, 2, pItem);
        //学分//
        pItem = new QTableWidgetItem(QString::number(ite->credit));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(row, 3, pItem);

        sum += ite->credit;
    }

    if(sum == 0)
    {
        m_pLabel->setText(tr("You didn't take courses."));
    }
    else
    {
        m_pLabel->setText(tr("Current Selected Course") + "(" + tr("Total credits") + ":" + QString::number(sum) + ")" + ":" + "\n");
    }
}

void SelectedCourseResult::initWidget()
{
    m_pLabel = new QLabel(this);

    m_pTableWidget = new QTableWidget(this);
    {
        QHeaderView *pHeader = m_pTableWidget->horizontalHeader();
        pHeader->setStretchLastSection(true);
        pHeader->setSectionResizeMode(QHeaderView::Stretch);

        //不可编辑//
        m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //课程号、课程名、课程属性、学分//
        QStringList list;
        list << tr("Course ID")
             << tr("Course Name")
             << tr("Course Attribute")
             << tr("Course Credit");
        //设置列数//
        m_pTableWidget->setColumnCount(list.count());
        m_pTableWidget->setHorizontalHeaderLabels(list);
    }

    QVBoxLayout *pVLayout = new QVBoxLayout(this);

    QHBoxLayout *pHLayout = new QHBoxLayout;
    QPushButton *pPB = new QPushButton;
    connect(pPB, SIGNAL(clicked()), this, SLOT(close()));

    pPB->setText(tr("ok"));
    pHLayout->addStretch();
    pHLayout->addWidget(pPB);

    pVLayout->addWidget(m_pLabel);
    pVLayout->addWidget(m_pTableWidget);
    pVLayout->addLayout(pHLayout);
}

