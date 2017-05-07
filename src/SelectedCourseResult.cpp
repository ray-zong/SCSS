#include "SelectedCourseResult.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

SelectedCourseResult::SelectedCourseResult(QWidget *parent)
    : QDialog(parent)
    , m_pLabel(NULL)
{
    initWidget();
}

SelectedCourseResult::~SelectedCourseResult()
{

}

void SelectedCourseResult::displaySelectedCourse(const QVector<DB_SpecialtyCourse> &vecCourse)
{
    Q_ASSERT(m_pLabel);

    QString strText = tr("Current Selected Course:") + "\n";
    for(auto ite = vecCourse.begin(); ite != vecCourse.end(); ++ite)
    {
        strText += tr("Course ID:") + QString::number(ite->number) + "\t" +
                tr("Course Name:") + ite->name + "\t" +
                tr("Course Credit:") + QString::number(ite->credit) +
                "\n";
    }
    m_pLabel->setText(strText);
}

void SelectedCourseResult::initWidget()
{
    m_pLabel = new QLabel(this);
    m_pLabel->setText(tr("Current Selected Course:") + "\n");
    QVBoxLayout *pVLayout = new QVBoxLayout(this);

    QHBoxLayout *pHLayout = new QHBoxLayout;
    QPushButton *pPB = new QPushButton;
    connect(pPB, SIGNAL(clicked()), this, SLOT(close()));

    pPB->setText(tr("ok"));
    pHLayout->addStretch();
    pHLayout->addWidget(pPB);

    pVLayout->addWidget(m_pLabel);
    pVLayout->addLayout(pHLayout);
}

