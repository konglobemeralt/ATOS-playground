#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<VOBJ_DATA>();

    Chronos *chronos = new Chronos();


    //PacketInterface mPacketInt;
    MapWidget *map = ui->widget;
    /* Listen for Chronos OSEM signal */
    //connect(chronos,SIGNAL(handle_osem(chronos_osem)),
    //        this,SLOT(updateLabelOSEM(chronos_osem)));
    connect(chronos, SIGNAL(handle_osem(chronos_osem)),
            map,SLOT(chronosOSEM(chronos_osem)));
    connect(chronos,SIGNAL(handle_dopm(QVector<chronos_dopm_pt>)),
            map,SLOT(chronosDOPM(QVector<chronos_dopm_pt>)));
    /* Start the chronos object */
    chronos->startServer(53240, 53241);


}

MainWindow::~MainWindow()
{
    delete chronos;
    delete ui;
}

void MainWindow::on_updateButton_clicked(){
    //ui->label->setText(QString("apa\t Bepa"));
    OSEM_DATA temp;
    /*
    temp.latitude = 4.56789f;//(float)(rand() % 100) / 100.0f;
    temp.longitude= 5.1f;
    temp.altitude = 5.3f;
    temp.heading = 1;
*/
    char c_lat[LABEL_TEXT_LENGTH];
    char c_long[LABEL_TEXT_LENGTH];
    char c_alt[LABEL_TEXT_LENGTH];
    char c_head[LABEL_TEXT_LENGTH];

    MapWidget* map = ui->widget;

    temp.latitude   = map->getRefLat();
    temp.longitude  = map->getRefLon();
    temp.altitude   = map->getRefAlt();
    temp.heading    = 0;

    snprintf(c_lat,LABEL_TEXT_LENGTH,"%g",temp.latitude);
    snprintf(c_long,LABEL_TEXT_LENGTH,"%g",temp.longitude);
    snprintf(c_alt,LABEL_TEXT_LENGTH,"%g",temp.altitude);
    snprintf(c_head,LABEL_TEXT_LENGTH,"%d",temp.heading);

    ui->lab_lat->setText(c_lat);
    ui->lab_lon->setText(c_long);
    ui->lab_alt->setText(c_alt);
    ui->lab_head->setText(c_head);
}

void MainWindow::on_playButton_clicked(){

    qint8 ID = 0;
    // Create virtual object as a new Thread
    VirtualObject *vobj = new VirtualObject(ID);
    MapWidget *map = ui->widget;


    // Add a car to the map
    // NOTE: the car and the thread must have the same ID
    //CarInfo* car = new CarInfo(ID);
    map->removeCar(ID);
    map->addCar(ID);



    // Set SLOT to delete the object once it is finished running
    connect(vobj, SIGNAL(finished()), vobj, SLOT(deleteLater()));

    // Disable the starting of a virtual object until it is done
    ui->playButton->setEnabled(false);
    // Set SLOT to enable the start of a virtual object upon termination
    connect(vobj, SIGNAL(finished()),this,SLOT(unlockRun()));

    // Set SLOT to update the running time when signal is received from the virtual object
    //connect(vobj, SIGNAL(updated_state(int,qint32,double,double)),
    //        this,SLOT(displayTime(int,qint32,double,double)));
    // Set SLOT to update the visual car with the virtual object state
    //connect(vobj,SIGNAL(updated_state(int,qint32,double,double)),
    //        map,SLOT(handleUpdatedCarState(int,qint32,double,double)));


    connect(vobj,SIGNAL(updated_state(VOBJ_DATA)),
            this,SLOT(handleUpdateState(VOBJ_DATA)));
    // Start the thread with the highest priority
    vobj->start(QThread::TimeCriticalPriority);


    //delete vobj;
}

void MainWindow::updateLabelOSEM(double lat, double lon, double alt) {
    //on_updateButton_clicked();

    char c_lat[LABEL_TEXT_LENGTH];
    char c_long[LABEL_TEXT_LENGTH];
    char c_alt[LABEL_TEXT_LENGTH];
    //char c_head[LABEL_TEXT_LENGTH];

    snprintf(c_lat,LABEL_TEXT_LENGTH,"%g",lat);
    snprintf(c_long,LABEL_TEXT_LENGTH,"%g",lon);
    snprintf(c_alt,LABEL_TEXT_LENGTH,"%g",alt);
    //snprintf(c_head,LABEL_TEXT_LENGTH,"%g",heading);

    ui->lab_lat->setText(c_lat);
    ui->lab_lon->setText(c_long);
    ui->lab_alt->setText(c_alt);
    //ui->lab_head->setText(c_head);
}

void MainWindow::unlockRun(){
    ui->playButton->setEnabled(true);
}

void MainWindow::displayTime(qint64 t){
    // Display the time sent from the object
    char buffer[20];
    double d_t = (double) t/1000.0f;
    snprintf(buffer,20,"%g",d_t);
    ui->lab_runtime->setText(buffer);

}

void MainWindow::handleUpdateState(VOBJ_DATA data){
    MapWidget *map = ui->widget;

    // Display time in the label
    displayTime(data.time);

    // Show the server reference values
    updateLabelOSEM(data.mRefLat,data.mRefLon,data.mRefAlt);
    // Update the map with the reference values
    // map->setEnuRef(data.mRefLat,dlata.mRefLon,data.mRefAlt);

    // Update the car position
    LocPoint pos;

    pos.setTime(data.time);
    pos.setXY(data.x,data.y);

    map->updateCarState(data.ID,pos);
    map->update();



}


