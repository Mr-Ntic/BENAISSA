#ifndef B_CALTS_H
#define B_CALTS_H

#include <QtCore/QtGlobal>
#include <QCP/qcustomplot.h>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include "select_t.h"
#include <QVector>
QT_BEGIN_NAMESPACE

class QLabel;


namespace Ui {
class B_CALTS;
}

QT_END_NAMESPACE

class B_CALTS : public QMainWindow
{
    Q_OBJECT

public:
    explicit B_CALTS(QWidget *parent = 0);
    ~B_CALTS();


    void realtimeVoltage(QCustomPlot *Voltage);
    void realtimeCurrent(QCustomPlot *Current) ;
    bool trx_Available ;
    int conter ;
    QStringList label_VAL ;
    QVector<double>  VOLTAGE_SENSOR_VAL;
    QVector<double>  LCD_SENSOR_VAL;
    unsigned short int  Sensor_enable[8];
    unsigned short int  switch_sync;
    bool start_condtion;
    bool SW_STATE[15];

    QVector<float> average;
    QVector<float> average_temp;
    QVector<float> sensors_tol;
    QVector<short int> passed ;
    QVector<short int> failed;

    short int test_pf;
    short int test_repeted_check;
    short  int average_index;

private slots:

    //test file parassing ------------------------------------------------ parassing
    void Sensors_state_changed(unsigned short int i);
    int test_file_check();
    int test_file_parassing();
    int test_condition(int d);
    void test_msg(int v);
    void switch_state_clear();

    unsigned int switch_auto(int i);

    void sensor_tolerance(int i);
    int sensor(int i);
    void auto_test_validation(int i);

    //les Action --------------------------------------------------------- Action

    void on_actionAbout_EPS_A_T_S_Project_triggered();

    void on_actionCMM_triggered();

    void on_actionNEW_TEST_triggered();

    //QCplot --------------------------------------------------------- QCplot

    void realtimeDataSlot();




    //Serial --------------------------------------------------------- Serial__

    void handleError(QSerialPort::SerialPortError error);

    void openSerialPort();
    void closeSerialPort();

    void readData();

    void TX_ROOT(const QByteArray &data) ;
    bool Send_Key(QByteArray Key) ;
    void serial_buff_full();






    //Switches --------------------------------------------------------- Switches__




    //signals --------------------------------------------------------- signals___


    void on_START_STR_clicked();

    void on_STOP_STP_clicked();

    void on_SW1_clicked();

    void on_SW2_clicked();

    void on_SW3_clicked();

    void on_SW4_clicked();

    void on_SW5_clicked();

    void on_SW8_clicked();

    void on_SW9_clicked();

    void on_SWA_clicked();

    void on_SWB_clicked();

    void on_SWC_clicked();

    void on_SWD_clicked();

    void on_SW7_clicked();

    void on_SWE_clicked();

    void on_SW6_clicked();


    void on_groupBox_15_clicked(); //sensor 1 grroupbox
    void on_groupBox_16_clicked();
    void on_groupBox_17_clicked();
    void on_groupBox_18_clicked();
    void on_groupBox_19_clicked();
    void on_groupBox_20_clicked();
    void on_groupBox_21_clicked();
    void on_groupBox_22_clicked();



    void on_actionIndex_triggered();

signals:
    void DataArrive(QString data );
    void TX_ROOT_signal(QByteArray data) ;
    void DataArrived();


private:

    Ui::B_CALTS *ui;
    QTimer auto_test_timer;
    QLabel *status;
    QSerialPort *serial;
    select_t *suzzy;
    void showStatusMessage(const QString &message);
    void initActionsConnections();

    static const quint16 test_set_vendor_id = 1659; // ard 9025 1659
    static const quint16 test_set_product_id = 8963; //ard 67 8963
    QString test_set_port_name ;
    bool test_set_is_available ;


    QString serialBuffer;
    QStringList buffer_split;



    QVector<float> SWITCHE_VAL[12];

    QStringList values;
    QStringList keys;

    bool OK_you_CAN ;
    bool data_arrived ;

    QByteArray serialData ;
    QStringList test_file_path;
    QStringList test_name;
    short int selected_test_num;

    QHash<QString, int> instruction_hash;
    short int timer;

    short int test_parsing_index;
};


#endif // B_CALTS_H
