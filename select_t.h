#ifndef SELECT_T_H
#define SELECT_T_H
#include <QDialog>
#include <QMessageBox>


namespace Ui {
class select_t;
}

class select_t : public QDialog
{
    Q_OBJECT

public:
    explicit select_t(QWidget *parent = 0);
    ~select_t();
    void m_cheked_up();
    bool temp_selected_disp(int i);


private slots:
    void on_buttonBox_accepted();



signals:
    void OK_pressed();

private:
    Ui::select_t *ui;

    bool temp_selected[13];


};

#endif // SELECT_T_H
