#ifndef ADDCOURSEWIDGET_H
#define ADDCOURSEWIDGET_H

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QStringListModel;

class AddCourseWidget : public QDialog
{
    Q_OBJECT

public:
    AddCourseWidget(QWidget *parent = nullptr);
    ~AddCourseWidget();

private:
    void initWidget();

private slots:
    void ok();
    void cancel();
    void queryTextChanged(const QString &);
    void scoreNameInputFinished();

private:
    QSpinBox *m_pSpinBox_number;
    QLineEdit *m_pLineEdit_name;
    QLineEdit *m_pLineEdit_score;

    QStringListModel *m_pStringListModel;
};


#endif
