#include "b_calts.h"
#include "ui_b_calts.h"

#include "about.h"
#include <QCP/qcustomplot.h>

#include <QSettings>
#include <QDesktopServices>

#include <QUrl>
#include <QDir>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QDebug>
#include <QScreen>
#include <QMetaEnum>


B_CALTS::B_CALTS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::B_CALTS)
{

    ui->setupUi(this);

    QMainWindow::centralWidget()->layout()->setContentsMargins(1, 3, 1, 3);
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");




    test_set_is_available = false ;
    test_set_port_name ="";
    trx_Available = true ;
    data_arrived = false ;
    serialBuffer = "";
    conter = 0;
    average_index =0;
    test_parsing_index=0;
    test_repeted_check=0;
    switch_sync = 0;
    start_condtion = false;
    VOLTAGE_SENSOR_VAL.reserve(8);
    LCD_SENSOR_VAL.reserve(8);
    sensors_tol.reserve(8);
    passed.reserve(15);
    failed.reserve(15);
    average.resize(8);
    average_temp.resize(8);
    sensors_tol.insert(0,7,0.06);
    average_temp.insert(0,7,0);
    average.insert(0,7,0);
    passed.insert(0,14,0);
    failed.insert(0,14,0);
    label_VAL.reserve(8);

    timer = 2000; //default timer pitch 2S
    serial = new QSerialPort(this);
    status = new QLabel;
    suzzy  = new select_t(this);
    selected_test_num =0;
    test_file_path << "./TEST_config/Insulation_t_F.ini"
                  << "./TEST_config/Bonding_t_F.ini"
                  << "./TEST_config/Continuity_t_F.ini"
                  << "./TEST_config/Operating_logic_t_F.ini"
                  << "./TEST_config/Fast_c_termi_F.ini"
                  << "./TEST_config/charge_inhibit_F.ini"
                  << "./TEST_config/self_test_fun_F.ini"
                  << "./TEST_config/output_F.ini"
                  << "./TEST_config/preliminary_op_F.ini"
                  << "./TEST_config/charge.ini"
                  << "./TEST_config/Discharge.ini"
                  << "./TEST_config/Serviceability.ini"
                  << "./TEST_config/Conditioning.ini";

    test_name     << "Insulation_t_F.ini"
                  << "Bonding_t_F.ini"
                  << "Continuity_t_F.ini"
                  << "Operating_logic_t_F.ini"
                  << "Fast_c_termi_F.ini"
                  << "charge_inhibit_F.ini"
                  << "self_test_fun_F.ini"
                  << "output_F.ini"
                  << "preliminary_op_F.ini"
                  << "charge.ini"
                  << "Discharge.ini"
                  << "Serviceability.ini"
                  << "Conditioning.ini";


    initActionsConnections();

    ui->statusBar->addWidget(status);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionTools->setEnabled(true);
    realtimeVoltage(ui->Voltage);
    realtimeCurrent(ui->Current);

connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),this, &B_CALTS::handleError);
connect(serial, &QSerialPort::readyRead, this, &B_CALTS::readData);
connect(ui->actionNEW_TEST, &QAction::triggered, suzzy, &B_CALTS::show);
connect(&auto_test_timer, SIGNAL(timeout()), this, SLOT(test_file_parassing()));
connect(this,SIGNAL(TX_ROOT_signal(QByteArray)),this,SLOT(TX_ROOT(QByteArray)));
connect(this,SIGNAL(DataArrived()),this,SLOT(serial_buff_full())) ;

//connect(ui->groupBox_3,&QAc,this,SLOT(hold_s7s14());

instruction_hash["S1H"] = 834972 ;instruction_hash["S2H"] = 835072 ;instruction_hash["S3H"] = 835172 ;instruction_hash["S4H"] = 835272 ;
instruction_hash["S5H"] = 835372 ;instruction_hash["S6H"] = 835472 ;instruction_hash["S7H"] = 835572 ;instruction_hash["S8H"] = 835672 ;
instruction_hash["S9H"] = 835772 ;instruction_hash["SAH"] = 836572 ;instruction_hash["SBH"] = 836672 ;instruction_hash["SCH"] = 836772 ;
instruction_hash["SDH"] = 836872 ;instruction_hash["SEH"] = 836972 ;

instruction_hash["S1L"] = 834976 ;instruction_hash["S2L"] = 835076 ;instruction_hash["S3L"] = 835176 ;instruction_hash["S4L"] = 835276 ;
instruction_hash["S5L"] = 835376 ;instruction_hash["S6L"] = 835476 ;instruction_hash["S7L"] = 835576 ;instruction_hash["S8L"] = 835676 ;
instruction_hash["S9L"] = 835776 ;instruction_hash["SAL"] = 836576 ;instruction_hash["SBL"] = 836676 ;instruction_hash["SCL"] = 836776 ;
instruction_hash["SDL"] = 836876 ;instruction_hash["SEL"] = 836976 ;


instruction_hash["S1"] = 8349 ;instruction_hash["S2"] = 8350 ;instruction_hash["S3"] = 8351 ;instruction_hash["S4"] = 8352 ;
instruction_hash["S5"] = 8353 ;instruction_hash["S6"] = 8354 ;instruction_hash["S7"] = 8355 ;instruction_hash["S8"] = 8356 ;
instruction_hash["S9"] = 8357 ;instruction_hash["SA"] = 8365 ;instruction_hash["SB"] = 8366 ;instruction_hash["SC"] = 8367 ;
instruction_hash["SD"] = 8368 ;instruction_hash["SE"] = 8369 ;

//ST1 ST2 ...
instruction_hash["SENS_T1"] = 838449 ;instruction_hash["SENS_T2"] = 838450 ;instruction_hash["SENS_T3"] = 838451 ;instruction_hash["SENS_T4"] = 838452 ;
instruction_hash["SENS_T5"] = 838453 ;instruction_hash["SENS_T6"] = 838454 ;instruction_hash["SENS_T7"] = 838455 ;instruction_hash["SENS_T8"] = 838456 ;

//S1S S2S ...
instruction_hash["SENS1_SUP"] = 834983 ;instruction_hash["SENS2_SUP"] = 835083 ;instruction_hash["SENS3_SUP"] = 835183 ;instruction_hash["SENS4_SUP"] = 835283 ;
instruction_hash["SENS5_SUP"] = 835383 ;instruction_hash["SENS6_SUP"] = 835483 ;instruction_hash["SENS7_SUP"] = 835583 ;instruction_hash["SENS8_SUP"] = 835683 ;

//S1I S2I ...
instruction_hash["SENS1_INF"] = 834973 ;instruction_hash["SENS2_INF"] = 835073 ;instruction_hash["SENS3_INF"] = 835173 ;instruction_hash["SENS4_INF"] = 835273 ;
instruction_hash["SENS5_INF"] = 835373 ;instruction_hash["SENS6_INF"] = 835473 ;instruction_hash["SENS7_INF"] = 835573 ;instruction_hash["SENS8_INF"] = 835673 ;


//S1N S2N ...
instruction_hash["SENS1"] = 834978 ;instruction_hash["SENS2"] = 835078 ;instruction_hash["SENS3"] = 835178 ;instruction_hash["SENS4"] = 835278 ;
instruction_hash["SENS5"] = 835378 ;instruction_hash["SENS6"] = 835478 ;instruction_hash["SENS7"] = 835578 ;instruction_hash["SENS8"] = 835678 ;



instruction_hash["START"] = 1992 ;instruction_hash["STOP"] = 1993 ;
instruction_hash["STR"] = 1994 ;instruction_hash["STP"] = 1995 ;

instruction_hash["TIMER"] = 1996 ;instruction_hash["msg"] = 778371 ;

instruction_hash["test_name_index"] = 2000 ;

}


B_CALTS::~B_CALTS()
{

    delete ui;
}


void B_CALTS::TX_ROOT(const QByteArray &key)  // DATA SEND FUNCTION

{
    trx_Available = false ; //desactivi
    qDebug()<<" great data was sent"<<serial->write(key) ;

    trx_Available = true ; //reactivé
    switch_sync=0;
}


bool B_CALTS::Send_Key(QByteArray key)
{
    int i=0;
                while(i<20)
                {
                    i++;
                        if (trx_Available && test_set_is_available)
                        {
                         this->TX_ROOT(key);
                         i=1993 ;
                        }
                }

                if (i == 1993){
                        return true;
                }else{
                        return false;
                }

}


//**************************************************

void B_CALTS::on_actionAbout_EPS_A_T_S_Project_triggered()
{
    About abt;
    abt.setModal(true);  // open about project window 
    abt.exec();
}


void B_CALTS::on_actionCMM_triggered()
{
    QDir directory("./file/CMM-24-35-01-R.-011-At-Elect.pdf");
    QString path = directory.filePath("CMM-24-35-01-R.-011-At-Elect.pdf");
    QString absolutePath = directory.absoluteFilePath("");
    QDesktopServices::openUrl(QUrl::fromLocalFile("file:///"+absolutePath));

}

void B_CALTS::on_actionNEW_TEST_triggered()
{

}

//-------------------------------------------------------serial start here :


void B_CALTS::openSerialPort()
{
     // test if serial port is avialibale using vendor and product id ;

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
         if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
             qDebug()<< serialPortInfo.vendorIdentifier() << serialPortInfo.productIdentifier();
            if(serialPortInfo.vendorIdentifier() == test_set_vendor_id){
                if(serialPortInfo.productIdentifier() == test_set_product_id){
                         test_set_port_name = serialPortInfo.portName();
                         test_set_is_available=true;
                         qDebug()<< test_set_is_available << test_set_port_name ;
                }
             }
          }
       }

     if (test_set_is_available){

         //serial port configuration configuration
         serial->setPortName(test_set_port_name);
         serial->open(QSerialPort::ReadWrite);
         serial->setBaudRate(QSerialPort::Baud115200);
         serial->setDataBits(QSerialPort::Data8);
         serial->setParity(QSerialPort::NoParity);
         serial->setStopBits(QSerialPort::OneStop);
         serial->setFlowControl(QSerialPort::NoFlowControl);
         ui->actionConnect->setEnabled(false); //desabele connect action
         ui->actionDisconnect->setEnabled(true); //enable disconnect action
         showStatusMessage(tr("Connected to %1 : 115200, 8, NoParity, one, NoFlowControl").arg(test_set_port_name));
       }else{
         QMessageBox::critical(this, tr("Error"), serial->errorString());
         showStatusMessage(tr("Open error"));

     }
     for(int i=0;i<15;i++)
     {
         SW_STATE[i]=false;
         if(i<8)
         {
             Sensor_enable[i]=0;
             sensors_tol.append(0);
             passed.append(0);
         }
     }


}

void B_CALTS::closeSerialPort()
{
   if (serial->isOpen())
    {
        serial->close();
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        showStatusMessage(tr("Disconnected"));
        test_set_is_available = false ;
        this->switch_state_clear();
    }
}

void B_CALTS::readData()
{
trx_Available = false ;

if(start_condtion)
{
    while(!serial->atEnd()){
   serialData = serial->read(1);
   //qDebug()<<serialData;
        if(serialData[0]=='M' || OK_you_CAN )
        {
            OK_you_CAN = true;
            serialBuffer=serialBuffer+QString::fromStdString(serialData.toStdString());
            if(serialData[0] == 'Y')
            {
                OK_you_CAN = false ; data_arrived =true ;
                buffer_split = serialBuffer.split(',') ;
                serialBuffer.clear();
                this->DataArrived();
            }

        }

        if( buffer_split.size() >71)
        {
            OK_you_CAN = false ;
            serialBuffer.clear();
            this->Send_Key("STP");
            this->Send_Key("RRR");
        }
        serialData.clear();
    }    
}else{
    trx_Available = true ; //enable transmission
    this->Send_Key("STP");
}
}

void B_CALTS::serial_buff_full()
{
    qDebug()<<buffer_split;

            double temp ;
            VOLTAGE_SENSOR_VAL.clear();
                for(int i=0;i<4;i++){
                    if(Sensor_enable[i])
                    {
                    temp = (buffer_split[i+1].toInt())*0.053767 ;  // VOLTAGE CALCULATION
                    VOLTAGE_SENSOR_VAL.append(temp);//r1+r2/r2
                    //qDebug()<<temp;
                   if(temp < 1){
                        label_VAL.append("mV");
                        double tt = temp*1000;
                        LCD_SENSOR_VAL.append(tt);
                    }else{
                        label_VAL.append("V") ;
                        LCD_SENSOR_VAL.append(VOLTAGE_SENSOR_VAL[i]);
                    }
                    }else{
                        VOLTAGE_SENSOR_VAL.append(0);//r1+r2/r2
                        label_VAL.append("mV");
                        LCD_SENSOR_VAL.append(0);

                    }
                }

                for(int i=4;i<8;i++){ //
                    if(Sensor_enable[i])
                    {
                    temp = ((buffer_split[i+1].toInt())*0.0049) ;  //*0.02648-13.51 0.0264*anl-13.51 currents CALCULATION
                    VOLTAGE_SENSOR_VAL.append(temp);//r1+r2/r2
                   if(temp< 1){

                       double tt = temp*1000;
                       LCD_SENSOR_VAL.append(tt);
                        label_VAL.append("mA");

                    }else{
                        label_VAL.append("A") ;
                        LCD_SENSOR_VAL.append(VOLTAGE_SENSOR_VAL[i]);
                    }
                    }else{
                        VOLTAGE_SENSOR_VAL.append(0);//r1+r2/r2
                        label_VAL.append("mA");
                        LCD_SENSOR_VAL.append(0);

                    }
                }


                ui->lcdNumber_voltage1->display(LCD_SENSOR_VAL.at(0));
                ui->lcdNumber_voltage2->display(LCD_SENSOR_VAL.at(1));
                ui->lcdNumber_voltage3->display(LCD_SENSOR_VAL.at(2));
                ui->lcdNumber_voltage4->display(LCD_SENSOR_VAL.at(3));

                ui->lcdNumber_sensor1->display(LCD_SENSOR_VAL.at(4));
                ui->lcdNumber_sensor2->display(LCD_SENSOR_VAL.at(5));
                ui->lcdNumber_sensor3->display(LCD_SENSOR_VAL.at(6));
                ui->lcdNumber_sensor4->display(LCD_SENSOR_VAL.at(7));

                ui->label_v1->setText(label_VAL.at(0));
                ui->label_v2->setText(label_VAL.at(1));
                ui->label_v3->setText(label_VAL.at(2));
                ui->label_v4->setText(label_VAL.at(3));

                ui->label_s1->setText(label_VAL.at(4));
                ui->label_s2->setText(label_VAL.at(5));
                ui->label_s3->setText(label_VAL.at(6));
                ui->label_s4->setText(label_VAL.at(7));
            this->realtimeDataSlot();
                // calculate avreag value
                average_index++;
                if(average_index<11)
                {
                    for(int i=0;i<8;i++)
                    {
                    average_temp[i]=VOLTAGE_SENSOR_VAL[i];
                    }
                }else{
                    average_index =0;
                    for(int i=0;i<8;i++)
                    {
                    average[i]= average.at(i)/10 ;

                    }
                }



            LCD_SENSOR_VAL.clear();
            label_VAL.clear();
            buffer_split.clear();
            trx_Available = true ; //reactivé
}


void B_CALTS::switch_state_clear ()
{

            QString switch_style ="color: #ffffff; background-color: #353535" ;
              SW_STATE[1]=false; ui->SW1->setStyleSheet(switch_style);
              SW_STATE[2]=false; ui->SW2->setStyleSheet(switch_style);
              SW_STATE[3]=false; ui->SW3->setStyleSheet(switch_style);
              SW_STATE[4]=false; ui->SW4->setStyleSheet(switch_style);
              SW_STATE[5]=false; ui->SW5->setStyleSheet(switch_style);
              SW_STATE[6]=false; ui->SW6->setStyleSheet(switch_style);
              SW_STATE[7]=false; ui->SW7->setStyleSheet(switch_style);
              SW_STATE[8]=false; ui->SW8->setStyleSheet(switch_style);
              SW_STATE[9]=false; ui->SW9->setStyleSheet(switch_style);
              SW_STATE[10]=false;ui->SWA->setStyleSheet(switch_style);
              SW_STATE[11]=false;ui->SWB->setStyleSheet(switch_style);
              SW_STATE[12]=false;ui->SWC->setStyleSheet(switch_style);
              SW_STATE[13]=false;ui->SWD->setStyleSheet(switch_style);
              SW_STATE[14]=false;ui->SWE->setStyleSheet(switch_style);
}



void B_CALTS::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}


void B_CALTS::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &B_CALTS::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &B_CALTS::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &B_CALTS::close);
    //connect(ui->actionConfigure, &QAction::triggered, settings, &B_CALTS::show);
   // connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    //connect(ui->actionAbout, &QAction::triggered, this, &B_CALTS::about);
   // connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void B_CALTS::showStatusMessage(const QString &message)
{
    status->setText(message);
}

//-------------------------------------------------------------------------------------------------SERIAL END




//-************************************************************************************************PLOTE START*************** plote voltage

void B_CALTS::realtimeVoltage(QCustomPlot *Voltage)
{
    Voltage->addGraph(); // blue line#F7BC05
    Voltage->graph(0)->setPen(QPen(QColor(247,208,13)));
    Voltage->graph(0)->setAntialiasedFill(false);

    Voltage->addGraph(); // red line#FFF45774
    Voltage->graph(1)->setPen(QPen(QColor(72,173,1)));
    Voltage->graph(0)->setChannelFillGraph(Voltage->graph(1));

    Voltage->addGraph(); // red line#b9ca4a185202
    Voltage->graph(2)->setPen(QPen(QColor(233,24,24)));
    Voltage->graph(0)->setChannelFillGraph(Voltage->graph(2));

    Voltage->addGraph(); // red line
    Voltage->graph(3)->setPen(QPen(QColor(253,91,3)));
    Voltage->graph(0)->setChannelFillGraph(Voltage->graph(3));

    //---------------------------------------testting

    Voltage->xAxis->setBasePen(QPen(Qt::white, 1));
    Voltage->yAxis->setBasePen(QPen(Qt::white, 1));
    Voltage->xAxis->setTickPen(QPen(Qt::white, 1));
    Voltage->yAxis->setTickPen(QPen(Qt::white, 1));
    Voltage->xAxis->setSubTickPen(QPen(Qt::white, 1));
    Voltage->yAxis->setSubTickPen(QPen(Qt::white, 1));
    Voltage->xAxis->setTickLabelColor(Qt::white);
    Voltage->yAxis->setTickLabelColor(Qt::white);

    Voltage->xAxis2->setBasePen(QPen(Qt::white, 1));
    Voltage->yAxis2->setBasePen(QPen(Qt::white, 1));
    Voltage->xAxis2->setTickPen(QPen(Qt::white, 1));
    Voltage->yAxis2->setTickPen(QPen(Qt::white, 1));
    Voltage->xAxis2->setSubTickPen(QPen(Qt::white, 1));
    Voltage->yAxis2->setSubTickPen(QPen(Qt::white, 1));
    Voltage->xAxis2->setTickLabelColor(Qt::white);
    Voltage->yAxis2->setTickLabelColor(Qt::white);

    Voltage->xAxis->grid()->setSubGridVisible(false);
    Voltage->yAxis->grid()->setSubGridVisible(false);
    Voltage->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    Voltage->xAxis->setDateTimeFormat("hh:mm:ss");
    Voltage->xAxis->setAutoTickStep(false);
    Voltage->xAxis->setLabel("Times");
    Voltage->yAxis->setLabel("Voltages");
    Voltage->axisRect()->setupFullAxesBox();
    QLinearGradient axisRectGradient;
    /*
     *3,19,22 good for back ground
     *
    */
    axisRectGradient.setColorAt(0, QColor(15,20,20));
    Voltage->axisRect()->setBackground(axisRectGradient);
    ui->Voltage->xAxis->grid()->setVisible(false);
    ui->Voltage->yAxis->grid()->setVisible(false);

    QLinearGradient backgroundGradient;
    backgroundGradient.setColorAt(0, QColor(53,53,53));

    Voltage->setBackground(backgroundGradient);
    Voltage->yAxis->setRange(0, 55);


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(Voltage->xAxis, SIGNAL(rangeChanged(QCPRange)), Voltage->xAxis2, SLOT(setRange(QCPRange)));
    connect(Voltage->yAxis, SIGNAL(rangeChanged(QCPRange)), Voltage->yAxis2, SLOT(setRange(QCPRange)));

}


//-***************************************************************************************************PLOTE START************ plote Current

void B_CALTS::realtimeCurrent(QCustomPlot *Current)
{
    Current->addGraph(); // blue line#F7BC05
    Current->graph(0)->setPen(QPen(QColor(247,208,13)));
    Current->graph(0)->setAntialiasedFill(false);

    Current->addGraph(); // red line#FFF45774
    Current->graph(1)->setPen(QPen(QColor(72,173,1)));
    Current->graph(0)->setChannelFillGraph(Current->graph(1));

    Current->addGraph(); // red line#b9ca4a185202
    Current->graph(2)->setPen(QPen(QColor(233,24,24)));
    Current->graph(0)->setChannelFillGraph(Current->graph(2));
    Current->addGraph(); // red line
    Current->graph(3)->setPen(QPen(QColor(253,91,3)));
    Current->graph(0)->setChannelFillGraph(Current->graph(3));
/*
    Current->addGraph(); // blue dot
    Current->graph(2)->setPen(QPen(Qt::blue));
    Current->graph(2)->setLineStyle(QCPGraph::lsNone);
    Current->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

    Current->addGraph(); // red dot
    Current->graph(3)->setPen(QPen(Qt::red));
    Current->graph(3)->setLineStyle(QCPGraph::lsNone);
    Current->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
*/
    //************************************************************************-
    Current->xAxis->setBasePen(QPen(Qt::white, 1));
    Current->yAxis->setBasePen(QPen(Qt::white, 1));
    Current->xAxis->setTickPen(QPen(Qt::white, 1));
    Current->yAxis->setTickPen(QPen(Qt::white, 1));
    Current->xAxis->setSubTickPen(QPen(Qt::white, 1));
    Current->yAxis->setSubTickPen(QPen(Qt::white, 1));
    Current->xAxis->setTickLabelColor(Qt::white);
    Current->yAxis->setTickLabelColor(Qt::white);

    Current->xAxis2->setBasePen(QPen(Qt::white, 1));
    Current->yAxis2->setBasePen(QPen(Qt::white, 1));
    Current->xAxis2->setTickPen(QPen(Qt::white, 1));
    Current->yAxis2->setTickPen(QPen(Qt::white, 1));
    Current->xAxis2->setSubTickPen(QPen(Qt::white, 1));
    Current->yAxis2->setSubTickPen(QPen(Qt::white, 1));
    Current->xAxis2->setTickLabelColor(Qt::white);
    Current->yAxis2->setTickLabelColor(Qt::white);

    Current->xAxis->grid()->setSubGridVisible(false);
    Current->yAxis->grid()->setSubGridVisible(false);
    Current->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    Current->xAxis->setDateTimeFormat("hh:mm:ss");
    Current->xAxis->setAutoTickStep(false);
    Current->xAxis->setLabel("Times");
    Current->yAxis->setLabel("Currents");
    Current->axisRect()->setupFullAxesBox();
    QLinearGradient axisRectGradient;
    /*
     *3,19,22 good for back ground
     *
    */
    axisRectGradient.setColorAt(0, QColor(15,20,20));
    Current->axisRect()->setBackground(axisRectGradient);
    ui->Current->xAxis->grid()->setVisible(false);
    ui->Current->yAxis->grid()->setVisible(false);

    QLinearGradient backgroundGradient;
    backgroundGradient.setColorAt(0, QColor(53,53,53));

    Current->setBackground(backgroundGradient);
    Current->yAxis->setRange(0, 10);


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(Current->xAxis, SIGNAL(rangeChanged(QCPRange)), Current->xAxis2, SLOT(setRange(QCPRange)));
    connect(Current->yAxis, SIGNAL(rangeChanged(QCPRange)), Current->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
        ui->Current->replot();
        ui->Voltage->replot();
}

void B_CALTS::realtimeDataSlot()
{

  // calculate two new data points:²
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/100.0;

  ui->Voltage->graph(0)->addData(key, VOLTAGE_SENSOR_VAL.at(0));
  ui->Voltage->graph(1)->addData(key, VOLTAGE_SENSOR_VAL.at(1));
  ui->Voltage->graph(2)->addData(key, VOLTAGE_SENSOR_VAL.at(2));
  ui->Voltage->graph(3)->addData(key, VOLTAGE_SENSOR_VAL.at(3));

  // remove data of lines that's outside visible range:
  ui->Voltage->graph(0)->removeDataBefore(key-8);
  ui->Voltage->graph(1)->removeDataBefore(key-8);
  ui->Voltage->graph(2)->removeDataBefore(key-8);
  ui->Voltage->graph(3)->removeDataBefore(key-8);

  // rescale value (vertical) axis to fit the current data:
  /*
  ui->Voltage->graph(0)->rescaleValueAxis();
  ui->Voltage->graph(1)->rescaleValueAxis(true);
  ui->Voltage->graph(2)->rescaleValueAxis();
  ui->Voltage->graph(3)->rescaleValueAxis(true);
*/
// make key axis range scroll with the data (at a constant range size of 8):

  ui->Voltage->xAxis->setRange(key+0.25, 5, Qt::AlignRight);
  ui->Voltage->replot();

    //-------------------------------------------------------------------------------------- current

    ui->Current->graph(0)->addData(key, VOLTAGE_SENSOR_VAL.at(4));
    ui->Current->graph(1)->addData(key, VOLTAGE_SENSOR_VAL.at(5));
    ui->Current->graph(2)->addData(key, VOLTAGE_SENSOR_VAL.at(6));
    ui->Current->graph(3)->addData(key, VOLTAGE_SENSOR_VAL.at(7));


    // remove data of lines that's outside visible range:
    ui->Current->graph(0)->removeDataBefore(key-8);
    ui->Current->graph(1)->removeDataBefore(key-8);
    ui->Current->graph(2)->removeDataBefore(key-8);
    ui->Current->graph(3)->removeDataBefore(key-8);

    // rescale value (vertical) axis to fit the current data:
    ui->Current->graph(0)->rescaleValueAxis();
    ui->Current->graph(1)->rescaleValueAxis(true);
    ui->Current->graph(2)->rescaleValueAxis();
    ui->Current->graph(3)->rescaleValueAxis(true);

  // make key axis range scroll with the data (at a constant range size of 8):

    ui->Current->xAxis->setRange(key+0.25, 5, Qt::AlignRight);
    ui->Current->replot();


}



// TEST START PARASING HERE -------------------------------------------------------------


int B_CALTS::test_file_check()
{
    keys.clear();
    values.clear();
    qDebug()<<keys<<values;

for(int i=0;i<13;i++)
{
    if(suzzy->temp_selected_disp(i))
        {
        if(QFile(test_file_path.at(i)).exists())
        {
            QSettings settings(test_file_path.at(i),QSettings::IniFormat);
            settings.sync();
            keys.append("test_name_index");
            values.append(test_name.value(i));

                    foreach (const QString &group, settings.childGroups())
                    {

                    settings.beginGroup(group);



                const QStringList childKeys = settings.allKeys();
                      keys.append(childKeys);


                    foreach (const QString &childKey, childKeys)
                    {
                    values.append(settings.value(childKey).toString());
                              //  qDebug() << settings.childKeys();
                    }

                    settings.endGroup();
                     }

                    selected_test_num ++;


        }else{
            qDebug() << "not exist";
            return 1;

        }
    }

}
qDebug()<<keys<<values;

return 0;
}


int B_CALTS::test_file_parassing()
 {

    if (test_parsing_index<keys.size())
    {

  int temp_key_hash = instruction_hash.value(keys[test_parsing_index]);

       if (temp_key_hash != instruction_hash["test_name_index"])
       {

        qDebug() <<"raho hna"<<keys[test_parsing_index]<<temp_key_hash;
        switch (temp_key_hash) {

                case 778371://778371
                    this->test_msg(test_parsing_index);
                    test_parsing_index++;
                break;
                    //S1 S2 S3 ...
                case 8349: case 8350: case 8351: case 8352: case 8353: case 8354: case 8355: case 8356:
                case 8357: case 8365: case 8366: case 8367: case 8368: case 8369: case 8370:
        {
                    int   temp_val_hash = instruction_hash.value(values[test_parsing_index]);
                   qDebug()<< instruction_hash.key(temp_val_hash).toStdString().c_str();
               this->Send_Key(instruction_hash.key(temp_val_hash).toStdString().c_str());

                    test_parsing_index++;
}
                break;
                      // SENS1.........
                case 834978: case 835078: case 835178:  case 835278:
                case 835378: case 835478: case 835578:  case 835678:
                   qDebug()<<"SENS1 raho fe sens ";

                    this->sensor(temp_key_hash);
                    test_parsing_index++;
                  break;

                    // sens_t1 ...
                    case 838449: case 838450: case 838451: case 838452:
                    case 838453: case 838454: case 838455: case 838456:
                        qDebug()<<"sens_t1";
                        this->sensor_tolerance(temp_key_hash);
                       test_parsing_index++; // chechk it
                      break;

                //SENS_sup
            case 834983: case 835083: case 835183: case 835283:
            case 835383: case 835483: case 835583: case 835683:

            this->test_condition(test_parsing_index);
            test_parsing_index++; // chechk it
              break;

                //SENS_inf
            case 834973: case 835073: case 835173: case 835273:
            case 835373: case 835473: case 835573: case 835673:
            this->test_condition(test_parsing_index);
            test_parsing_index++; // chechk it
              break;

            case 1996: //timer
                timer = values[test_parsing_index].toInt();
                auto_test_timer.stop();
                auto_test_timer.start(timer);
                qDebug()<<timer;
               test_parsing_index++; // chechk it
              break;


            case 1992: //start
                test_parsing_index++; // chechk it
              break;


            case 1993: //stop
                test_parsing_index++; // chechk it
              break;

            case 2000: //test_name_index
                this->auto_test_validation(2000);
                test_parsing_index++; // chechk it
              break;

                default:
                        QMessageBox msgBox;
                        msgBox.setText("Sorry instruction not found check TEST_CONFIG files");
                        msgBox.exec();

        }

       }else{


            return 0;
       }

    }
    return 1;
}


void B_CALTS::sensor_tolerance(int i)
{

    float temp = values[test_parsing_index].toFloat();
    switch (i) {

    case 838449:
        sensors_tol.insert(0,temp);
        break;

    case 838450 :
        sensors_tol.insert(1,temp);
        break;
    case 838451:
        sensors_tol.insert(2,temp);
        break;

    case 838452:
        sensors_tol.insert(3,temp);
        break;

    case 838453:
        sensors_tol.insert(4,temp);
        break;

    case 838454:
        sensors_tol.insert(5,temp);
        break;

    case 838455:
        sensors_tol.insert(6,temp);
        break;

    case 838456 :
        sensors_tol.insert(7,temp);
        break;


    default:
        break;
    }
   //qDebug()<<sensors_tol;
}


int B_CALTS::sensor(int i)
{
    float values_temp =values[test_parsing_index].toFloat();
    int v;
    switch (i) {
    case 834978:
        if(average[0]<(values_temp+sensors_tol.at(0))&&average[0]>(values_temp-sensors_tol.at(0))){test_pf++;}else {test_pf--;}
        v=0;
        break;

    case 835078:
        if(average[1]<(values_temp+sensors_tol.at(1))&&average[1]>(values_temp-sensors_tol.at(1))){test_pf++;}else {test_pf--;}
        v=1;
        break;
    case 835178:
        if(average[2]<(values_temp+sensors_tol.at(2))&&average[2]>(values_temp-sensors_tol.at(2))){test_pf++;}else {test_pf--;}
        v=2;
        break;

    case 835278:
        if(average[3]<(values_temp+sensors_tol.at(3))&&average[3]>(values_temp-sensors_tol.at(3))){test_pf++;}else {test_pf--;}
        v=3;
        break;

    case 835378:
        if(average[4]<(values_temp+sensors_tol.at(4))&&average[4]>(values_temp-sensors_tol.at(4))){test_pf++;}else {test_pf--;}
        v=4;
        break;

    case 835478:
        if(average[5]<(values_temp+sensors_tol.at(5))&&average[5]>(values_temp-sensors_tol.at(5))){test_pf++;}else {test_pf--;}
        v=5;
        break;

    case 835578:
        if(average[6]<(values_temp+sensors_tol.at(6))&&average[6]>(values_temp-sensors_tol.at(6))){test_pf++;}else {test_pf--;}
        v=6;
        break;

    case 835678:
        if(average[7]<(values_temp+sensors_tol.at(7))&&average[7]>(values_temp-sensors_tol.at(7))){test_pf++;}else {test_pf--;}
        v=7;
        break;
    default:
        break;
    }
test_repeted_check++;
    if(test_repeted_check<5)
    {
        qDebug()<<"test_parsing_index"<<test_repeted_check<<test_parsing_index--<<test_pf;
    }else{
        qDebug()<<"passed-failed vector";
        if(test_pf>0){passed[0]=1;}else{ failed[0]=1;}
        test_pf = 0;
        test_repeted_check =0;
        qDebug()<<"passed-failed vector"<<passed<<failed;
        return 0;
    }

return 1;
}

void B_CALTS::auto_test_validation(int i)
{
    if (i=2000)
    {
        this->ui->test_name->setText(values[test_parsing_index]);
    }

}

int B_CALTS::test_condition(int d)
{
    return 0;
}

void B_CALTS::test_msg(int v)

{
    QMessageBox msgBox;
    msgBox.setText(values[v]);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Abort );
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
}

unsigned int B_CALTS::switch_auto(int i)
{

return 0;
}


//------------------------------------------------------------------------------------------------
void B_CALTS::on_START_STR_clicked()
{


 if(ui->groupBox_2->isChecked())
 {
     this->Send_Key("STR");
     this->Send_Key("RRR");
     start_condtion = true;
 }else{
   //  qDebug()<<suzzy->temp_selected_disp(0)<<suzzy->temp_selected_disp(1)<<suzzy->temp_selected_disp(2)<<suzzy->temp_selected_disp(3)<<suzzy->temp_selected_disp(4)<<suzzy->temp_selected_disp(5)<<suzzy->temp_selected_disp(4)<<suzzy->temp_selected_disp(5)<<suzzy->temp_selected_disp(6)<<suzzy->temp_selected_disp(7)<<suzzy->temp_selected_disp(8)<<suzzy->temp_selected_disp(9)<<suzzy->temp_selected_disp(10)<<suzzy->temp_selected_disp(11)<<suzzy->temp_selected_disp(12);

     if(this->test_file_check()==0)
     {
             this->Send_Key("STR");
             this->Send_Key("RRR");
             start_condtion = true;
           auto_test_timer.start(timer);
     }

 }



}

void B_CALTS::on_STOP_STP_clicked()
{
    this->Send_Key("STP");
    start_condtion = false;
    this->switch_state_clear();
    test_parsing_index =0;
    timer = 2000;
    auto_test_timer.stop();
    qDebug()<<SW_STATE[1]<<SW_STATE[2]<<SW_STATE[2]<<SW_STATE[3]<<SW_STATE[4]<<SW_STATE[5]<<SW_STATE[6]<<SW_STATE[7]<<SW_STATE[8]<<SW_STATE[9]<<SW_STATE[10]<<SW_STATE[11]<<SW_STATE[12]<<SW_STATE[13]<<SW_STATE[14];
}

void B_CALTS::on_SW1_clicked()
{
  if(SW_STATE[1]==1){
      SW_STATE[1] = false ;
      this->Send_Key("D1L");ui->SW1->setStyleSheet("color: #ffffff; background-color: #D35400");
  }else{
      SW_STATE[1] = true ;

      this->Send_Key("D1H");ui->SW1->setStyleSheet("color: #ffffff; background-color: #2a82da");
  }
}

void B_CALTS::on_SW2_clicked()
{
    if(SW_STATE[2]==1){
        SW_STATE[2] = false ;
        this->Send_Key("D2L");ui->SW2->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[2] = true ;
        this->Send_Key("D2H");ui->SW2->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SW3_clicked()
{
    if(SW_STATE[3]==1){
        SW_STATE[3] = false ;
        this->Send_Key("D3L");ui->SW3->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[3] = true ;
        this->Send_Key("D3H");ui->SW3->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SW4_clicked()
{
    if(SW_STATE[4]==1){
        SW_STATE[4] = false ;
        this->Send_Key("D4L");ui->SW4->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[4] = true ;
        this->Send_Key("D4H");ui->SW4->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SW5_clicked()
{
    if(SW_STATE[5]==1){
        SW_STATE[5] = false ;
        this->Send_Key("D5L");ui->SW5->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[5] = true ;
        this->Send_Key("D5H");ui->SW5->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SW8_clicked()
{
    if(SW_STATE[8]==1){
        SW_STATE[8] = false ;
        this->Send_Key("D8L");ui->SW8->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[8] = true ;
        this->Send_Key("D8H");ui->SW8->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SW9_clicked()
{
    if(SW_STATE[9]==1){
        SW_STATE[9] = false ;
        this->Send_Key("D9L");ui->SW9->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[9] = true ;
        this->Send_Key("D9H");ui->SW9->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SWA_clicked()
{
    if(SW_STATE[10]==1){
        SW_STATE[10] = false ;
        this->Send_Key("DAL");ui->SWA->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[10] = true ;
        this->Send_Key("DAH");ui->SWA->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SWB_clicked()
{
    if(SW_STATE[11]==1){
        SW_STATE[11] = false ;
        this->Send_Key("DBL");ui->SWB->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[11] = true ;
        this->Send_Key("DBH");ui->SWB->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SWC_clicked()
{
    if(SW_STATE[12]==1){
        SW_STATE[12] = false ;
        this->Send_Key("DCL");ui->SWC->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[12] = true ;
        this->Send_Key("DCH");ui->SWC->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SWD_clicked()
{
    if(SW_STATE[13]==1){
        SW_STATE[13] = false ;
        this->Send_Key("DDL");ui->SWD->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[13] = true ;
        this->Send_Key("DDH");ui->SWD->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}
void B_CALTS::on_SW7_clicked()
{
    if(SW_STATE[7]==1){
        SW_STATE[7] = false ;
        this->Send_Key("D7L");ui->SW7->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[7] = true ;
        this->Send_Key("D7H");ui->SW7->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}

void B_CALTS::on_SWE_clicked()
{
    if(SW_STATE[14]==1){
        SW_STATE[14] = false ;
        this->Send_Key("DEL");ui->SWE->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[14] = true ;
        this->Send_Key("DEH");ui->SWE->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}


void B_CALTS::on_SW6_clicked()
{
    if(SW_STATE[6]==1){
        SW_STATE[6] = false ;
        this->Send_Key("D6L");ui->SW6->setStyleSheet("color: #ffffff; background-color: #D35400");
    }else{
        SW_STATE[6] = true ;
        this->Send_Key("D6H");ui->SW6->setStyleSheet("color: #ffffff; background-color: #2a82da");
    }
}


void B_CALTS::Sensors_state_changed(unsigned short int i){

    switch (i) {

    case 1:
        if(ui->groupBox_15->isChecked())
        {
            Sensor_enable[4]=1;
        }else{
            Sensor_enable[4]=0;        }
        break;

    case 2:
        if(ui->groupBox_16->isChecked())
        {
            Sensor_enable[5]=1;
        }else{
            Sensor_enable[5]=0;

        }
        break;

    case 3:
        if(ui->groupBox_17->isChecked())
        {
            Sensor_enable[6]=1;
        }else{
            Sensor_enable[6]=0;

        }
        break;

    case 4:
        if(ui->groupBox_18->isChecked())
        {
            Sensor_enable[7]=1;
        }else{
            Sensor_enable[7]=0;

        }
        break;

    case 5:
        if(ui->groupBox_19->isChecked())
        {
            Sensor_enable[0]=1;
        }else{
            Sensor_enable[0]=0;

        }
        break;

    case 6:
        if(ui->groupBox_20->isChecked())
        {
            Sensor_enable[1]=1;
        }else{
            Sensor_enable[1]=0;
        }
        break;

    case 7:
        if(ui->groupBox_21->isChecked())
        {
            Sensor_enable[2]=1;
        }else{
            Sensor_enable[2]=0;
        }
        break;

    case 8:
        if(ui->groupBox_22->isChecked())
        {
            Sensor_enable[3]=1;
        }else{
            Sensor_enable[3]=0;
        }
        break;
    default:
        break;
    }
qDebug()<<Sensor_enable[0]<<Sensor_enable[1]<<Sensor_enable[2]<<Sensor_enable[3]<<Sensor_enable[4]<<Sensor_enable[5]<<Sensor_enable[6]<<Sensor_enable[7];
}

//SW_STATE[]

void B_CALTS::on_groupBox_15_clicked()
{
   this->Sensors_state_changed(1);
}
void B_CALTS::on_groupBox_16_clicked()
{
    this->Sensors_state_changed(2);

}
void B_CALTS::on_groupBox_17_clicked()
{
    this->Sensors_state_changed(3);

}
void B_CALTS::on_groupBox_18_clicked()
{
    this->Sensors_state_changed(4);

}
void B_CALTS::on_groupBox_19_clicked()
{
    this->Sensors_state_changed(5);

}
void B_CALTS::on_groupBox_20_clicked()
{
    this->Sensors_state_changed(6);

}
void B_CALTS::on_groupBox_21_clicked()
{
    this->Sensors_state_changed(7);

}
void B_CALTS::on_groupBox_22_clicked()
{
    this->Sensors_state_changed(8);

}


void B_CALTS::on_actionIndex_triggered()
{
    QString link = "https://github.com/Mr-Ntic/BENAISSA";
    QDesktopServices::openUrl(QUrl(link));
}
