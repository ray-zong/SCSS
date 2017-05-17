#include "ModifyCourseWidget.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "ConnectionPool.h"

ModifyCourseWidget::ModifyCourseWidget(QWidget *parent)
    : QDialog(parent)
    , m_pSpinBox_number(nullptr)
    , m_pLineEdit_name(nullptr)
    , m_pLineEdit_score(nullptr)
{
    initWidget();
}

ModifyCourseWidget::~ModifyCourseWidget()
{

}

void ModifyCourseWidget::setCourseNumber(int number)
{
    m_pSpinBox_number->setValue(number);
}

void ModifyCourseWidget::setCourseName(const QString &name)
{
    m_pLineEdit_name->setText(name);
}

void ModifyCourseWidget::setCourseScore(const QString &score)
{
    m_pLineEdit_score->setText(score);
}

int ModifyCourseWidget::getCourseNumber()
{
    return m_pSpinBox_number->value();
}

QString ModifyCourseWidget::getCourseName()
{
    return m_pLineEdit_name->text();
}

QString ModifyCourseWidget::getCourseScore()
{
    return m_pLineEdit_score->text();
}

void ModifyCourseWidget::initWidget()
{
    QLabel *pLabel_number = new QLabel(tr("Course Number"));
    m_pSpinBox_number = new QSpinBox;
    m_pSpinBox_number->setRange(0, 99999999);
    m_pSpinBox_number->setReadOnly(true);
    m_pSpinBox_number->setButtonSymbols(QSpinBox::NoButtons);
    m_pSpinBox_number->setReadOnly(true);
    m_pSpinBox_number->setDisabled(true);

    QLabel *pLabel_name = new QLabel(tr("Course Name"));
    m_pLineEdit_name = new QLineEdit;
    m_pLineEdit_name->setReadOnly(true);
    m_pLineEdit_name->setDisabled(true);

    QLabel *pLabel_score = new QLabel(tr("Course Score"));
    m_pLineEdit_score = new QLineEdit;

    QPushButton *pPushButton_ok = new QPushButton(tr("Ok"));
    connect(pPushButton_ok, SIGNAL(clicked()), this, SLOT(ok()));
    QPushButton *pPushButton_cancel = new QPushButton(tr("Cancel"));
    connect(pPushButton_cancel, SIGNAL(clicked()), this, SLOT(cancel()));

    QGridLayout *pGridLayout= new QGridLayout(this);
    pGridLayout->addWidget(pLabel_number, 0, 0);
    pGridLayout->addWidget(m_pSpinBox_number, 0, 1);

    pGridLayout->addWidget(pLabel_name, 1, 0);
    pGridLayout->addWidget(m_pLineEdit_name, 1, 1);

    pGridLayout->addWidget(pLabel_score, 2, 0);
    pGridLayout->addWidget(m_pLineEdit_score, 2, 1);

    pGridLayout->addWidget(pPushButton_ok, 3, 0);
    pGridLayout->addWidget(pPushButton_cancel, 3, 1);
}

bool ModifyCourseWidget::modifyCourseScoreByNumber(int number, const QString &score)
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
    QSqlQuery query(QString("UPDATE student_course "
                            "SET Score = '%1' "
                            "WHERE "
                                "student_course.Course_Number = %2").arg(score).arg(number), db);
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

void ModifyCourseWidget::ok()
{
    if(modifyCourseScoreByNumber(m_pSpinBox_number->value(), m_pLineEdit_score->text()))
    {
        accept();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Try again, the score is not valid."));
        msgBox.exec();
    }
}

void ModifyCourseWidget::cancel()
{
    reject();
}
