#include "ChooseWidget.h"

#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>

#include "ConnectionPool.h"

ChooseWidget::ChooseWidget(QWidget *parent)
    : QWidget(parent)
    , m_pSpecialtyComboBox(NULL)
    , m_pTermComboBox(NULL)
{
    initWidget();
    querySpecialty();
}

ChooseWidget::~ChooseWidget()
{

}

int ChooseWidget::currentSpecialty()
{
    return m_pSpecialtyComboBox->currentIndex();
}

int ChooseWidget::currentTerm()
{
    return m_pTermComboBox->currentIndex();
}

void ChooseWidget::initWidget()
{
    QLabel *pLabel_specialty = new QLabel(this);
    pLabel_specialty->setText(tr("Specialty") + ":");

    m_pSpecialtyComboBox = new QComboBox(this);
    connect(m_pSpecialtyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCurrentSpecialtyOrTermChanged(int)));

    QLabel *pLabel_term = new QLabel(this);
    //pLabel_term->hide();
    pLabel_term->setText(tr("Term") + ":");
    m_pTermComboBox = new QComboBox(this);
    connect(m_pTermComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(emitCurrentSpecialtyOrTermChanged(int)));
    //m_pTermComboBox->hide();

    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->addStretch();
    pHLayout->addWidget(pLabel_specialty);
    pHLayout->addWidget(m_pSpecialtyComboBox);

    pHLayout->addWidget(pLabel_term);
    pHLayout->addWidget(m_pTermComboBox);
    pHLayout->addStretch();
}

void ChooseWidget::querySpecialty()
{
    Q_ASSERT(m_pSpecialtyComboBox != NULL);
    Q_ASSERT(m_pTermComboBox != NULL);

    //连接数据库
    QSqlDatabase db;
    if(!createConnection(db))
    {
        qDebug() << __FILE__ << __LINE__ << "error";
        return ;
    }

    //查询
    QSqlQuery query("SELECT Name FROM specialty_info", db);

    while(query.next())
    {
        QString item = query.value(0).toString();
        m_pSpecialtyComboBox->addItem(item);
    }

    query.exec("SELECT Name FROM course_term");
    while(query.next())
    {
        QString item = query.value(0).toString();
        m_pTermComboBox->addItem(item);
    }

    //连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

bool ChooseWidget::createConnection(QSqlDatabase &db)
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

void ChooseWidget::emitCurrentSpecialtyOrTermChanged(int index)
{
    Q_UNUSED(index);

    emit currentSpecialtyOrTermChanged(m_pSpecialtyComboBox->currentIndex(),
                                 m_pTermComboBox->currentIndex());
}
