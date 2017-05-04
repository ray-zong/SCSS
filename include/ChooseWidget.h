#ifndef CHOOSEWIDGET_H
#define CHOOSEWIDGET_H

#include <QWidget>

class QLabel;
class QComboBox;
class QSqlDatabase;

/**
 * @brief 专业和学期选择
 */

class ChooseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseWidget(QWidget *parent = 0);
    ~ChooseWidget();
    //当前专业
    int currentSpecialty();
    //当前学期
    int currentTerm();

private:
    void initWidget();
    void querySpecialty();
    bool createConnection(QSqlDatabase& db);

private slots:
    void emitCurrentSpecialtyOrTermChanged(int index);

signals:
    void currentSpecialtyOrTermChanged(int specialty, int term);

private:
    QComboBox *m_pSpecialtyComboBox;
    QComboBox *m_pTermComboBox;
};

#endif // CHOOSEWIDGET_H
