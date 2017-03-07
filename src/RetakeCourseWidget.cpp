#include "RetakeCourseWidget.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>

RetakeCourseWidget::RetakeCourseWidget(QWidget *parent)
    : QWidget(parent)
    , m_pTableWidget(nullptr)
{
    createTableWidget();
}

RetakeCourseWidget::~RetakeCourseWidget()
{

}

void RetakeCourseWidget::createTableWidget()
{
    m_pTableWidget = new QTableWidget(this);
    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->addWidget(m_pTableWidget);
    {
        QHeaderView *pHeader = m_pTableWidget->horizontalHeader();
        pHeader->setStretchLastSection(true);
        pHeader->setSectionResizeMode(QHeaderView::Stretch);

        //排序
        //pHeader->setSortIndicator(0, Qt::AscendingOrder);
        //pHeader->setSortIndicatorShown(true);
        //connect(pHeader, SIGNAL(sectionClicked(int)), pTableWidget, SLOT(sortByColumn(int)));


        //不可编辑//
        m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //课程号、课程名、学分、成绩、课程属性、学年、学期//
        QStringList list;
        list << tr("Course ID")
             << tr("Course Name")
             << tr("Course Credit")
             << tr("Course Grade")
             << tr("Course Attribute")
             << tr("Year")
             << tr("Term");
        //设置列数//
        m_pTableWidget->setColumnCount(list.count());
        m_pTableWidget->setHorizontalHeaderLabels(list);

        //设置行数//
        //m_pTableWidget->setRowCount();
    }
}
