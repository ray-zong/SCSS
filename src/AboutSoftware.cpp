#include "AboutSoftware.h"

#include <QLabel>
#include <QHBoxLayout>

AboutSoftware::AboutSoftware(QWidget *parent)
    : QDialog(parent)
    , m_pText(NULL)
{
    initUI();
}

AboutSoftware::~AboutSoftware()
{

}

void AboutSoftware::initUI()
{
    m_pText = new QLabel(this);


    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->addWidget(m_pText);
}
