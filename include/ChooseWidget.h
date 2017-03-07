#ifndef CHOOSEWIDGET_H
#define CHOOSEWIDGET_H

#include <QWidget>

class QLabel;
class QComboBox;
class QSqlDatabase;

class ChooseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseWidget(QWidget *parent = 0);
    ~ChooseWidget();

private:
    void initWidget();
    void querySpecialty();
    bool createConnection(QSqlDatabase& db);

private slots:
    void emitCurrentSpecialtyChanged(int index);
    void emitCurrentTermChanged(int index);

signals:
    void currentSpecialtyChanged(int index);
    void currentTermChanged(int index);

private:
    QComboBox *m_pSpecialtyComboBox;
    QComboBox *m_pTermComboBox;
};

#endif // CHOOSEWIDGET_H
