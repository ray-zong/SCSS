#include "OptionTreeWidget.h"

#include <QVBoxLayout>
#include <QTreeWidget>
#include <QHeaderView>

OptionTreeWidget::OptionTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_pTreeWidget(nullptr)
{
    createTreeWidget();
}

OptionTreeWidget::~OptionTreeWidget()
{

}

void OptionTreeWidget::createTreeWidget()
{
    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    m_pTreeWidget = new QTreeWidget(this);
    pVLayout->addWidget(m_pTreeWidget);
    m_pTreeWidget->setColumnCount(1);
    m_pTreeWidget->header()->setHidden(true);

    connect(m_pTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

    //TODO:临时添加，演示使用
    {
        //已修课程
        QTreeWidgetItem *pItem = new QTreeWidgetItem(m_pTreeWidget);
        pItem->setText(0, tr("taked course"));
        pItem->setData(0, Qt::UserRole, 0);
        //可选课程
        pItem = new QTreeWidgetItem(m_pTreeWidget);
        pItem->setText(0, tr("selectable course"));
        pItem->setData(0, Qt::UserRole, 1);
        //重修课程
        //pItem = new QTreeWidgetItem(m_pTreeWidget);
        //pItem->setText(0, tr("retake course"));
        //pItem->setData(0, Qt::UserRole, 2);
    }
}

void OptionTreeWidget::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    int currentItemId = -1;
    int previouseItemId = -1;

    Q_ASSERT(current != nullptr);
    currentItemId = current->data(0, Qt::UserRole).toInt();

    if(previous != nullptr)
    {
        previouseItemId = previous->data(0, Qt::UserRole).toInt();
    }

    emit currentOptionChanged(currentItemId, previouseItemId);
}
