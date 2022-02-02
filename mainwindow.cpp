#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Simulador de Procesos");
    setFixedSize(1280,720);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addProcess()
{
    Proceso p;
    std::string operation;
    int result,number1,number2;

    number1 = ui->number1SP->value();
    number2 = ui->number2SP->value();

    switch (ui->symbolsCB->currentIndex()){
    case 0:
        result = number1 + number2;
        operation = std::to_string(number1)+" + "+std::to_string(number2);
        break;
    case 1:
        result = number1 - number2;
        operation = std::to_string(number1)+" - "+std::to_string(number2);
        break;
    case 2:
        result = number1 * number2;
        operation = std::to_string(number1)+" * "+std::to_string(number2);
        break;
    case 3:
        result = number1 / number2;
        operation = std::to_string(number1)+" / "+std::to_string(number2);
        break;
    case 4:
        result = number1 % number2;
        operation = std::to_string(number1)+" % "+std::to_string(number2);
        break;
    default:
        result = 0;
        break;
    }

    p.setId(ui->idLE->text().toInt());
    p.setNombre(ui->nameLE->text().toStdString());
    p.setOperacion(operation);
    p.setTiempoEstimado(ui->timeEstimatedSP->value());
    p.setResultadoOperacion(result);

    lot.push_back(p);
}

bool MainWindow::operationValid()
{
    int symbol = ui->symbolsCB->currentIndex();
    if (symbol == 3 or symbol == 4){
        if (ui->number2SP->value() == 0){
            QMessageBox message;
            message.setText("Error, no puede dividirse entre 0.");
            message.setIcon(QMessageBox::Critical);
            message.exec();
            return false;
        }
    }
    return true;
}

bool MainWindow::idExist(int id)
{
    int i = 0;
    for (auto it1 = lot.begin(); it1 != lot.end(); ++it1,++i){
        if (lot[i].getId() == id){
            QMessageBox message;
            message.setText("ID ya ingresado. Prueba con otro.");
            message.setIcon(QMessageBox::Critical);
            message.exec();
            return true;
        }
    }
    return false;
}

void MainWindow::splitProcessInLots()
{
    std::vector<Proceso> aux;

    for (int i = 0,j = 0; i < totalProcess;++j){
        aux.push_back(lot[j]);
        ++i;
        if (i%PROCESS_PER_LOT == 0 or i == totalProcess){
            lots.push_back(aux);
            aux.clear();
        }
    }
    lot.clear();
    aux.clear();
}

void MainWindow::startProcess()
{
    std::string lotsSeparation = "";
    pendientLots = lotsCounter;
    currentLot = 0;

    ui->pendientLotsLCD->display(pendientLots);
    while (lots.size()){
        pendientLots--;
        lotsSeparation = "LOTE " + std::to_string(currentLot+1);
        ui->finishedTB->insertRow(ui->finishedTB->rowCount());
        ui->finishedTB->setItem(currentLot*(PROCESS_PER_LOT+1),0,new QTableWidgetItem(QString(lotsSeparation.c_str())));
        while (lots[0].size()){
            showWorkingLot();
            runProcess();
        }
        ui->lotWorkingTB->setRowCount(0);
        lots.erase(lots.begin());
        currentLot++;
    }
}

void MainWindow::showWorkingLot()
{
    int i = 0;
    int rows = 0;
    int j = 1;
    Proceso p;

    ui->lotWorkingTB->setRowCount(0);
    while (j < (int)lots[i].size()){
        p = lots[i][j];
        ui->lotWorkingTB->insertRow(ui->lotWorkingTB->rowCount());
        ui->lotWorkingTB->setItem(rows,0,new QTableWidgetItem(QString(p.getNombre().c_str())));
        ui->lotWorkingTB->setItem(rows,1,new QTableWidgetItem(QString::number(p.getTiempoEstimado())));
        rows++;
        j++;
    }
}

void MainWindow::runProcess()
{
    tT = 0;
    tR = lots[0][0].getTiempoEstimado();
    Proceso p = lots[0][0];

    ui->processRuningTB->setItem(0,0,new QTableWidgetItem(QString::number(p.getId())));
    ui->processRuningTB->setItem(1,0,new QTableWidgetItem(QString(p.getNombre().c_str())));
    ui->processRuningTB->setItem(2,0,new QTableWidgetItem(QString(p.getOperacion().c_str())));
    ui->processRuningTB->setItem(3,0,new QTableWidgetItem(QString::number(p.getTiempoEstimado())));
    ui->processRuningTB->setItem(4,0,new QTableWidgetItem(QString::number(tT)));
    ui->processRuningTB->setItem(5,0,new QTableWidgetItem(QString::number(tR)));

    while (tT < p.getTiempoEstimado()){
        tR--;
        tT++;
        ui->processRuningTB->setItem(4,0,new QTableWidgetItem(QString::number(tT)));
        ui->processRuningTB->setItem(5,0,new QTableWidgetItem(QString::number(tR)));
        globalCounter++;
        ui->globalCountLCD->display(globalCounter);
        ui->pendientLotsLCD->display(pendientLots);
        std::thread sleep([](){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
        sleep.join();
        ui->stackedWidget->repaint();
    }
    int rowsFinished(ui->finishedTB->rowCount());
    ui->finishedTB->insertRow(rowsFinished);
    ui->finishedTB->setItem(rowsFinished,0,new QTableWidgetItem(QString::number(p.getId())));
    ui->finishedTB->setItem(rowsFinished,1,new QTableWidgetItem(QString(p.getOperacion().c_str())));
    ui->finishedTB->setItem(rowsFinished,2,new QTableWidgetItem(QString::number(p.getResultadoOperacion())));
    ui->finishedTB->setItem(rowsFinished,3,new QTableWidgetItem(QString::number(currentLot+1)));
    lots[0].erase(lots[0].begin());
    ui->processRuningTB->clearContents();

}

void MainWindow::delay()
{
    for (int i = 0; i < NUMBER_TO_DELAY; ++i);
}

void MainWindow::on_processNumberSP_valueChanged(int arg1)
{
    if (arg1 > 0){
        ui->processAcountPB->setEnabled(true);
    }
    else{
        ui->processAcountPB->setEnabled(false);
    }
}

void MainWindow::on_processAcountPB_clicked()
{
    ui->processNumberSP->setEnabled(false);
    ui->dataGB->setEnabled(true);
    totalProcess = ui->processNumberSP->value();
}

void MainWindow::enable_add_process_button()
{
    if (!ui->nameLE->text().isEmpty() and !ui->idLE->text().isEmpty() and
            ui->timeEstimatedSP->value() > 0){
        ui->addProcessBT->setEnabled(true);
    }
    else{
        ui->addProcessBT->setEnabled(false);
    }
}

void MainWindow::on_nameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enable_add_process_button();
}

void MainWindow::on_number1SP_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    enable_add_process_button();
}

void MainWindow::on_symbolsCB_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    enable_add_process_button();
}

void MainWindow::on_number2SP_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    enable_add_process_button();
}

void MainWindow::on_idLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enable_add_process_button();
}

void MainWindow::on_timeEstimatedSP_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    enable_add_process_button();
}

void MainWindow::on_addProcessBT_clicked()
{
    if (operationValid() and !idExist(ui->idLE->text().toInt())){
        capturedProcess++;
        processLot++;
        addProcess();
        if (capturedProcess < totalProcess){
            if (processLot == PROCESS_PER_LOT){
                processLot = 0;
                lotsCounter++;
            }
            ui->capturatedProcessesCounter->display(capturedProcess);
        }
        else{
            splitProcessInLots();
            ui->stackedWidget->setCurrentIndex(SHOW_LOTS);
            setFixedSize(1280,720);
        }
    }
    ui->nameLE->clear();
    ui->idLE->clear();
    ui->number1SP->setValue(0);
    ui->number2SP->setValue(0);
    ui->symbolsCB->setCurrentIndex(0);
    ui->timeEstimatedSP->setValue(1);
}

void MainWindow::on_startRunPB_clicked()
{
    startProcess();
    ui->startRunPB->setEnabled(false);
}
