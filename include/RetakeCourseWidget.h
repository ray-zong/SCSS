#ifndef RETAKECOURSEWIDGET_H
#define RETAKECOURSEWIDGET_H

#include <QWidget>

class QTableWidget;

class RetakeCourseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RetakeCourseWidget(QWidget *parent = 0);
    ~RetakeCourseWidget();

private:
    void createTableWidget();


private slots:

private:
    QTableWidget *m_pTableWidget;
};

#endif // RETAKECOURSEWIDGET_H
