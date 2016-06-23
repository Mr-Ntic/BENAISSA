#include "select_t.h"
#include "ui_select_t.h"
#include<qdebug>

select_t::select_t(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::select_t)
{
    ui->setupUi(this);
    for(int i=0;i<13;i++)
    {
        temp_selected[i]=false;

    }
    qDebug()<<temp_selected[0]<<temp_selected[1]<<temp_selected[2]<<temp_selected[3]<<temp_selected[4]<<temp_selected[5]<<temp_selected[6]<<temp_selected[7]<<temp_selected[8]<<temp_selected[9]<<temp_selected[10]<<temp_selected[11]<<temp_selected[12];

}

select_t::~select_t()
{
    delete ui;
}

bool select_t::temp_selected_disp(int i)
{

return select_t::temp_selected[i];

}


void select_t::on_buttonBox_accepted()
{



// checkbox configuration ;

//---------------------------------------------------------------------------test 1



    if(ui->Insulation_t->isChecked())
            {
                    select_t::temp_selected[0]=true;
            }else{

                    select_t::temp_selected[0]=false;
                 }


//---------------------------------------------------------------------------test 2


            if(ui->Bonding_t->isChecked())
            {
                      select_t::temp_selected[1]=true;
            }else{

                      select_t::temp_selected[1]=false;
                 }



//---------------------------------------------------------------------------test 3

            if(ui->Continuity_t->isChecked())
            {
                      select_t::temp_selected[2]=true;
            }else{

                      select_t::temp_selected[2]=false;
                 }




//---------------------------------------------------------------------------test 4
            if(ui->Operating_logic_t->isChecked())
            {
                     select_t::temp_selected[3]=true;
            }else{

                     select_t::temp_selected[3]=false;
                 }




//---------------------------------------------------------------------------test 5
            if(ui->Fast_c_termi->isChecked())
            {
                      select_t::temp_selected[4]=true;
            }else{

                      select_t::temp_selected[4]=false;
                 }


//---------------------------------------------------------------------------test 6
            if(ui->charge_inhibit->isChecked())
            {
                      select_t::temp_selected[5]=true;
            }else{

                      select_t::temp_selected[5]=false;
                 }


//---------------------------------------------------------------------------test 7
            if(ui->self_test_fun->isChecked())
            {
                     select_t::temp_selected[6]=true;
            }else{

                     select_t::temp_selected[6]=false;
                 }


//---------------------------------------------------------------------------test 8

            if(ui->output->isChecked())
            {
                     select_t::temp_selected[7]=true;
            }else{

                     select_t::temp_selected[7]=false;
                 }


//---------------------------------------------------------------------------test 9
            if(ui->preliminary_op->isChecked())
            {
                     select_t::temp_selected[8]=true;
            }else{

                     select_t::temp_selected[8]=false;
                 }



//---------------------------------------------------------------------------test 10
            if(ui->charge->isChecked())
            {
                      select_t::temp_selected[9]=true;
            }else{

                      select_t::temp_selected[9]=false;
                 }



//---------------------------------------------------------------------------test 11
            if(ui->Discharge->isChecked())
            {
                     select_t::temp_selected[10]=true;
            }else{

                     select_t::temp_selected[10]=false;
                 }


//---------------------------------------------------------------------------test 12
            if(ui->Serviceability->isChecked())
            {
                      select_t::temp_selected[11]=true;
            }else{

                      select_t::temp_selected[11]=false;
                 }


//---------------------------------------------------------------------------test 13
             if(ui->Conditioning->isChecked())
             {
                      select_t::temp_selected[12]=true;
             }else{

                      select_t::temp_selected[12]=false;
                  }

qDebug()<<this->temp_selected_disp(0);

//qDebug()<<temp_selected[0]<<temp_selected[1]<<temp_selected[2]<<temp_selected[3]<<temp_selected[4]<<temp_selected[5]<<temp_selected[6]<<temp_selected[7]<<temp_selected[8]<<temp_selected[9]<<temp_selected[10]<<temp_selected[11]<<temp_selected[12];

}
