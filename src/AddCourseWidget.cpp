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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QCompleter>
#include <QDebug>

#include "ConnectionPool.h"

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

QStringList AddCourseWidget::queryStringListByName(const QString &name)
{
    QStringList textList;
    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return textList;
    }
    //查询数据
    QSqlQuery query(QString("SELECT "
                            "course_info.Name "
                            "FROM "
                            "course_info "
                            "WHERE "
                            "course_info.Name LIKE '%%1%'").arg(name), db);
    while(query.next())
    {
        //qDebug() << __FILE__ << __LINE__ << "success";
        textList << query.value(0).toString();
    }

    //关闭数据库
    ConnectionPool::closeConnection(db);

    return textList;
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
    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return;
    }
    QSqlQuery query(db);
    //插入数据
    QString sql("INSERT INTO student_course(Student_Number, Course_Number, Score)VALUES");

    sql += QString("(%1, %2, '%3')")
            .arg("100000")
            .arg(m_pSpinBox_number->value())
            .arg(m_pLineEdit_score->text());

    if(!query.exec(sql))
    {
        qDebug() << __FILE__ << __LINE__ << "error:insert data to table failed!";
    }
    //关闭数据库
    ConnectionPool::closeConnection(db);
    accept();
}

void AddCourseWidget::cancel()
{
    reject();
}

void AddCourseWidget::scoreNumberInputFinished()
{
    int number = m_pSpinBox_number->value();

    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return ;
    }
    //查询数据
    QSqlQuery query(QString("SELECT "
                            "course_info.Name "
                            "FROM "
                            "course_info "
                            "WHERE "
                            "course_info.Number=%1").arg(number), db);
    if(query.next())
    {
        QString result = query.value(0).toString();
        m_pLineEdit_name->setText(result);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Try again, the number is not valid."));
        msgBox.exec();
    }

    //关闭数据库
    ConnectionPool::closeConnection(db);

}

void AddCourseWidget::queryTextChanged(const QString &text)
{
    QStringList textList = queryStringListByName(text);

    m_pStringListModel->setStringList(textList);
}

void AddCourseWidget::scoreNameInputFinished()
{
    QString strText = m_pLineEdit_name->text();
    if(strText.isEmpty())
    {
        //QMessageBox msgBox;
        //msgBox.setText(tr("Try again, the name is empty."));
        //msgBox.exec();
        return ;
    }

    //连接并打开数据库
    QSqlDatabase db = ConnectionPool::openConnection();
    if(!db.isValid())
    {
        qDebug() << __FILE__ << __LINE__ << "error: cannot open the database";
        return ;
    }
    //查询数据
    QSqlQuery query(QString("SELECT "
                            "course_info.Number "
                            "FROM "
                            "course_info "
                            "WHERE "
                            "course_info.Name='%1'").arg(strText), db);
    if(query.next())
    {
        int result = query.value(0).toInt();
        m_pSpinBox_number->setValue(result);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Try again, the name is not valid."));
        msgBox.exec();
    }

    //关闭数据库
    ConnectionPool::closeConnection(db);
}
