#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <thread>
#include <windows.h>
#include <QTimer>
#include <QMessageBox>

#include "proceso.h"

#define CAPTURE_DATA 0
#define SHOW_LOTS 1
#define PROCESS_PER_LOT 5
#define NUMBER_TO_DELAY 500000000


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addProcess();
    bool operationValid();
    bool idExist(int id);
    void splitProcessInLots();
    void startProcess();
    void showWorkingLot();
    void runProcess();
    void delay();

private slots:

    void on_processNumberSP_valueChanged(int arg1);

    void on_processAcountPB_clicked();

    void enable_add_process_button();

    void on_nameLE_textChanged(const QString &arg1);

    void on_number1SP_valueChanged(int arg1);

    void on_symbolsCB_currentIndexChanged(int index);

    void on_number2SP_valueChanged(int arg1);

    void on_idLE_textChanged(const QString &arg1);

    void on_timeEstimatedSP_valueChanged(int arg1);

    void on_addProcessBT_clicked();

    void on_startRunPB_clicked();

private:
    Ui::MainWindow *ui;

    int capturedProcess = 0;
    int totalProcess;
    int processLot = 0;
    int lotsCounter = 1;
    int globalCounter = 0;
    int pendientLots;
    int currentLot;
    int currentProcess;
    int tT;
    int tR;

    std::vector<std::vector<Proceso>> lots;
    std::vector<Proceso> lot;
};
#endif // MAINWINDOW_H
