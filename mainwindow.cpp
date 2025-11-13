//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <semaphore.h>
#include <QString>
// Semáforos para as 9 regiões críticas
sem_t semaforos [9];

sem_t S_danger_zone;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < 9; ++i)
        sem_init(&semaforos[i], 0, 1);

    // Evita deadlock inicial:
    // por exemplo, dá permissão inicial apenas ao Trem 1 na RC0
    //sem_wait(&semaforos[0]);  // Trem 1 começa "dentro" da RC0


    //Cria o trem com seu (ID, posição X, posição Y, velocidade)
    trem1 = new Trem(1,40,40, ui->horizontalSlider_1->value());
    trem2 = new Trem(2,310,40, ui->horizontalSlider_2->value());
    trem3 = new Trem(3,580,40, ui->horizontalSlider_3->value());
    trem4 = new Trem(4,170,160, ui->horizontalSlider_4->value());
    trem5 = new Trem(5,440,160, ui->horizontalSlider_5->value());

    // Trem 6 (preto), malha externa. Posição inicial (20, 300)
    trem6 = new Trem(6, 40, 280, ui->horizontalSlider_6->value());


    /*
     * Conecta o sinal UPDATEGUI à função UPDATEINTERFACE.
     * Ou seja, sempre que o sinal UPDATEGUI foi chamado, será executada a função UPDATEINTERFACE.
     */
    connect(trem1,SIGNAL(updateGUI(int,int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem2,SIGNAL(updateGUI(int,int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem3,SIGNAL(updateGUI(int,int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem4,SIGNAL(updateGUI(int,int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem5,SIGNAL(updateGUI(int,int,int,int)),SLOT(updateInterface(int,int,int)));

    // Conexão para o Trem 6
    connect(trem6,SIGNAL(updateGUI(int,int,int,int)),SLOT(updateInterface(int,int,int)));

    // Inicia os trens automaticamente
    init_trem();
}

//Função que será executada quando o sinal UPDATEGUI for emitido
void MainWindow::updateInterface(int id, int x, int y){

    QString coords = QString("(%1, %2)").arg(x).arg(y);

    switch(id){
    case 1: //Atualiza a posição do objeto da tela (quadrado) que representa o trem 1
        ui->label_trem1->setGeometry(x,y,21,17);
        ui->label_coords_1->setText(coords); // <-- ATUALIZA O TEXTO
        break;
    case 2: //Atualiza a posição do objeto da tela (quadrado) que representa o trem 2
        ui->label_trem2->setGeometry(x,y,21,17);
        ui->label_coords_2->setText(coords); // <-- ATUALIZA O TEXTO
        break;
    case 3: //Atualiza a posição do objeto da tela (quadrado) que representa o trem 3
        ui->label_trem3->setGeometry(x,y,21,17);
        ui->label_coords_3->setText(coords); // <-- ATUALIZA O TEXTO
        break;
    case 4: //Atualiza a posição do objeto da tela (quadrado) que representa o trem 4
        ui->label_trem4->setGeometry(x,y,21,17);
        ui->label_coords_4->setText(coords); // <-- ATUALIZA O TEXTO
        break;
    case 5: //Atualiza a posição do objeto da tela (quadrado) que representa o trem 5
        ui->label_trem5->setGeometry(x,y,21,17);
        ui->label_coords_5->setText(coords); // <-- ATUALIZA O TEXTO
        break;
    case 6: //Atualiza a posição do objeto da tela (quadrado) que representa o trem 6
        ui->label_trem6->setGeometry(x,y,21,17);
        ui->label_coords_6->setText(coords); // <-- ATUALIZA O TEXTO
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Inicia a execução das threads dos trens
 */
void MainWindow::init_trem()
{
    trem1->start();
    trem2->start();
    trem3->start();
    trem4->start();
    trem5->start();
    trem6->start();
}

// Slots de controle de velocidade/sliders
void MainWindow::on_horizontalSlider_1_valueChanged(int value){
    trem1->setVel(value);
};

void MainWindow::on_horizontalSlider_2_valueChanged(int value){
    trem2->setVel(value);
};

void MainWindow::on_horizontalSlider_3_valueChanged(int value){
    trem3->setVel(value);
};

void MainWindow::on_horizontalSlider_4_valueChanged(int value){
    trem4->setVel(value);
};

void MainWindow::on_horizontalSlider_5_valueChanged(int value){
    trem5->setVel(value);
};

void MainWindow::on_horizontalSlider_6_valueChanged(int value){
    trem6->setVel(value);
};

