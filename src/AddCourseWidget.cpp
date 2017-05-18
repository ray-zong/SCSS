#include "AddCourseWidget.h"


#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QStringListModel>
#include <QMessageBox>
#include <QCompleter>
#include <QDebug>

#include "DatabaseQuery.h"

AddCourseWidget::AddCourseWidget(QWidget *parent)
    : QDialog(parent)
    , m_pSpinBox_number(nullptr)
    , m_pLineEdit_name(nullptr)
    , m_pLineEdit_score(nullptr)
{
    initWidget();
}

AddCourseWidget::~AddCourseWidget()
{

}

void AddCourseWidget::initWidget()
{
    QLabel *pLabel_nubmer = new QLabel(tr("Course Number"));
    m_pSpinBox_number = new QSpinBox;
    m_pSpinBox_number->setRange(0, 99999999);
    m_pSpinBox_number->setButtonSymbols(QSpinBox::NoButtons);
    m_pSpinBox_number->setReadOnly(true);
    m_pSpinBox_number->setDisabled(true);

    //connect(m_pSpinBox_number, SIGNAL(editingFinished()), this, SLOT(scoreNumberInputFinished()));

    QLabel *pLabel_name = new QLabel(tr("Course Name"));
    m_pLineEdit_name = new QLineEdit;
    QCompleter *pCompleter = new QCompleter;
    m_pStringListModel = new QStringListModel;
    pCompleter->setModel(m_pStringListModel);
    m_pLineEdit_name->setCompleter(pCompleter);
    connect(m_pLineEdit_name, SIGNAL(textEdited(const QString &)), this, SLOT(queryTextChanged(const QString &)));
    connect(m_pLineEdit_name, SIGNAL(editingFinished()), this, SLOT(scoreNameInputFinished()));

    QLabel *pLabel_score = new QLabel(tr("Course Score"));
    m_pLineEdit_score = new QLineEdit;

    QPushButton *pPushButton_ok = new QPushButton(tr("Ok"));
    connect(pPushButton_ok, SIGNAL(clicked()), this, SLOT(ok()));
    QPushButton *pPushButton_cancel = new QPushButton(tr("Cancel"));
    connect(pPushButton_cancel, SIGNAL(clicked()), this, SLOT(cancel()));

    QGridLayout *pGridLayout= new QGridLayout(this);
    pGridLayout->addWidget(pLabel_nubmer, 0, 0);
    pGridLayout->addWidget(m_pSpinBox_number, 0, 1);

    pGridLayout->addWidget(pLabel_name, 1, 0);
    pGridLayout->addWidget(m_pLineEdit_name, 1, 1);

    pGridLayout->addWidget(pLabel_score, 2, 0);
    pGridLayout->addWidget(m_pLineEdit_score, 2, 1);

    pGridLayout->addWidget(pPushButton_ok, 3, 0);
    pGridLayout->addWidget(pPushButton_cancel, 3, 1);
}

void AddCourseWidget::ok()
{
    if(m_pSpinBox_number->value() == 0
            || m_pLineEdit_name->text().isEmpty()
            || m_pLineEdit_score->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Try again, Input is not valid."));
        msgBox.exec();
        return;
    }

    if(!DatabaseQuery::instance()->insertDataIntoStudentCourse(m_pSpinBox_number->value(),
                                                           m_pLineEdit_score->text()))
    {
        qDebug() << __FILE__ << __LINE__ << "error";
    }

    accept();
}

void AddCourseWidget::cancel()
{
    reject();
}

void AddCourseWidget::queryTextChanged(const QString &text)
{
    QStringList textList = DatabaseQuery::instance()->queryStringListByName(text);

    m_pStringListModel->setStringList(textList);
}

void AddCourseWidget::scoreNameInputFinished()
{
    QString strText = m_pLineEdit_name->text();
    if(strText.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Try again, the name is empty."));
        msgBox.exec();
        return ;
    }

    int number = DatabaseQuery::instance()->queryNumberByName(strText);
    if(number == -1)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Try again, the name is not valid."));
        msgBox.exec();
    }
}
