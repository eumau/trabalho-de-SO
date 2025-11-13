//trem.cpp

//trem.cpp

#include "trem.h"
#include "mainwindow.h"
#include <QtCore>
#include <semaphore.h>

extern sem_t semaforos[9]; // 7 regiões críticas

#include <QDebug>

Trem::Trem(int ID, int x, int y, int vel){
    this->ID = ID;
    this->x = x;
    this->y = y;
    setVel(vel);
    // inicializa flags de regiões críticas
    for (int i = 0; i < 9; ++i) insideRegion[i] = false;
}


void Trem::run(){
    while(true){
        if (vel > 0) {
            switch(ID) {
                // ---------- CASE 1 (Trem 1) ----------
            case 1: // Trem 1 (Verde)

                // --- Lógicas de Saída ---
                if (x == 310 && y == 160 && insideRegion[0]) { sem_post(&semaforos[0]); insideRegion[0] = false; }
                if (x == 40 && y == 160 && insideRegion[2]) { sem_post(&semaforos[2]); insideRegion[2] = false; }

                // --- Lógicas de Parada/Movimento (Seus Pontos) ---

                // Ponto de Parada RC 0 (Seu ponto: 290, 40)
                if (x == 290 && y == 40 && !insideRegion[0]) {
                    sem_wait(&semaforos[0]);
                    insideRegion[0] = true;
                    x += 10;
                }
                // Ponto de Parada RC 2 (Seu ponto: 310, 140)
                else if (x == 310 && y == 140 && !insideRegion[2]) {
                    sem_wait(&semaforos[2]);
                    insideRegion[2] = true;
                    y += 10;
                }
                // Movimento Normal
                else {
                    if (y == 40 && x < 310)
                        x += 10;
                    else if (x == 310 && y < 160) // Dentro RC 0
                        y += 10;
                    else if (x > 40 && y == 160) // Dentro RC 2
                        x -= 10;
                    else
                        y -= 10;
                }
                break;


            // ---------- CASE 2 (Trem 2) ----------
            case 2: // Trem 2 (Vermelho)

                // --- Lógicas de Saída ---
                if (x == 310 && y == 40 && insideRegion[0]) { sem_post(&semaforos[0]); insideRegion[0] = false; }
                if (x == 580 && y == 160 && insideRegion[1]) { sem_post(&semaforos[1]); insideRegion[1] = false; }
                if (x == 310 && y == 160 && insideRegion[2]) { sem_post(&semaforos[2]); insideRegion[2] = false; }
                if (x == 440 && y == 160 && insideRegion[4]) { sem_post(&semaforos[4]); insideRegion[4] = false; }

                // --- Movimento (Seus Pontos) ---

                // Ponto de Parada RC 1 (Seu ponto: 560, 40)
                if (x == 560 && y == 40 && !insideRegion[1]) {
                    sem_wait(&semaforos[1]);
                    insideRegion[1] = true;
                    x += 10;
                }
                // Ponto de Parada RC 4 (Seu ponto: 580, 140)
                else if (x == 580 && y == 140 && !insideRegion[4]) {
                    sem_wait(&semaforos[4]);
                    insideRegion[4] = true;
                    y += 10;
                }
                // Ponto de Parada RC 2 (Seu ponto: 460, 160)
                else if (x == 460 && y == 160 && !insideRegion[2]) {
                    sem_wait(&semaforos[2]);
                    insideRegion[2] = true;
                    x -= 10;
                }
                // Ponto de Parada RC 0 (Seu ponto: 330, 160)
                else if (x == 330 && y == 160 && !insideRegion[0]) {
                    sem_wait(&semaforos[0]);
                    insideRegion[0] = true;
                    x -= 10;
                }
                // Movimento normal
                else {
                    if (y == 40 && x < 580)
                        x += 10;
                    else if (x == 580 && y < 160) // Dentro RC 1
                        y += 10;
                    else if (x > 310 && y == 160) // Dentro RC 4 e RC 2
                        x -= 10;
                    else // Dentro RC 0
                        y -= 10;
                }
                break;


                // ---------- CASE 3 (Trem 3) ----------
            case 3: //Trem 3 (Azul)
                // --- Lógicas de Saída ---
                if (x == 580 && y == 40 && insideRegion[1]) { sem_post(&semaforos[1]); insideRegion[1] = false; }
                if (x == 580 && y == 160 && insideRegion[4]) { sem_post(&semaforos[4]); insideRegion[4] = false; }
                if (x == 850 && y == 40 && insideRegion[5]) { sem_post(&semaforos[5]); insideRegion[5] = false; }

                // --- Lógica de Movimento (CORRIGIDA) ---

                // Ponto de Parada RC 4 (Seu ponto: 850, 140)
                if (x == 850 && y == 140 && !insideRegion[4]) {
                    sem_wait(&semaforos[4]);
                    insideRegion[4] = true;
                    y += 10;
                }
                // Ponto de Parada RC 1 (Seu ponto: 600, 160)
                else if (x == 600 && y == 160 && !insideRegion[1]) {
                    sem_wait(&semaforos[1]);
                    insideRegion[1] = true;
                    x -= 10;
                }
                // Ponto de Parada RC 5 (Seu ponto: 580, 50)
                else if (x == 580 && y == 50 && !insideRegion[5]) {
                    sem_wait(&semaforos[5]);
                    insideRegion[5] = true;
                    y -= 10;
                }
                // Movimento Normal
                else {
                    if (y == 40 && x < 850) // Dentro da RC 5
                        x += 10;

                    // Lógica de descida (x=850)
                    else if (x == 850 && y < 160) {
                        // CORREÇÃO: SÓ se move se NÃO estiver no ponto de parada (850, 140)
                        // Se estiver em (850, 140) e o 'if' principal falhou (pq o semáforo está ocupado),
                        // este 'if' vai falhar e o trem ficará parado, que é o correto.
                        if (y != 140) {
                            y += 10;
                        }
                    }

                    else if (x > 580 && y == 160) // Dentro RC 4
                        x -= 10;
                    else // Dentro RC 1
                        y -= 10;
                }
                break;


            // ---------- CASE 4 (Trem 4) ----------
            case 4: //Trem 4 (Laranja)

                // --- Lógicas de Saída ---
                if (x == 440 && y == 160 && insideRegion[2]) { sem_post(&semaforos[2]); insideRegion[2] = false; } // Libera RC 2
                if (x == 440 && y == 280 && insideRegion[3]) { sem_post(&semaforos[3]); insideRegion[3] = false; } // Libera RC 3

                // --- Lógicas de Parada/Movimento ---
                if (x == 40 && y == 170 && !insideRegion[2]) { // Parada RC 2
                    sem_wait(&semaforos[2]); // Pede RC 2 (Oeste)
                    insideRegion[2] = true;
                    y -= 10;
                }
                // Ponto de Parada RC 3 (Seu ponto: 420, 160)
                else if (x == 420 && y == 160 && !insideRegion[3]) {
                    sem_wait(&semaforos[3]); // Pede RC 3 (Vertical)
                    insideRegion[3] = true;
                    x += 10;
                }
                else { // Movimento Normal
                    if (y == 160 && x < 440) // Dentro da RC 2
                        x += 10;
                    else if (x == 440 && y < 280) // Dentro da RC 3
                        y += 10;
                    else if (x > 40 && y == 280)
                        x -= 10;
                    else // Sobe (x == 40 && y > 160)
                        y -= 10;
                }
                break;
                // ---------- CASE 5 (Trem 5) ----------
            case 5: //Trem 5 (Roxo)

                // --- Lógicas de Saída ---
                if (x == 440 && y == 160 && insideRegion[3]) { sem_post(&semaforos[3]); insideRegion[3] = false; }
                // A SAÍDA DA RC 4 FOI MOVIDA PARA A LÓGICA DE MOVIMENTO ABAIXO

                // --- Lógicas de Parada/Movimento ---

                // Ponto de Parada RC 3 (Seu ponto: 460, 280)
                if (x == 460 && y == 280 && !insideRegion[3]) {
                    sem_wait(&semaforos[3]);
                    insideRegion[3] = true;
                    x -= 10;
                }
                // Ponto de Parada RC 4 (Seu ponto: 440, 180)
                else if (x == 440 && y == 180 && !insideRegion[4]) {
                    sem_wait(&semaforos[4]);
                    insideRegion[4] = true;
                    y -= 10;
                }
                // Movimento Normal
                else {
                    if (y == 160 && x < 850) // Dentro da RC 4
                        x += 10;

                    // PONTO DE SAÍDA CORRIGIDO DA RC 4
                    else if (x == 850 && y < 280) {
                        // Se está em (850, 160) E vai para (850, 170)...
                        // este é o momento de soltar o semáforo.
                        if (y == 160 && insideRegion[4]) {
                            sem_post(&semaforos[4]); // Solta a RC 4
                            insideRegion[4] = false;
                        }
                        y += 10; // Continua descendo
                    }

                    else if (x > 440 && y == 280) // Indo para a RC 3
                        x -= 10;
                    else // (x == 440 && y > 160) // Dentro da RC 3
                        y -= 10;
                }

                break;

            case 6: // Trem 6 (preto) malha externa (ANTI-HORÁRIO - CORRIGIDO)
                if (y == 280 && x < 850) // 1. Anda para a DIREITA na base
                    x+=10;
                else if (x == 850 && y > 40) // 2. Sobe pela direita
                    y-=10;
                else if (y == 40 && x > 40) // 3. Anda para a ESQUERDA no topo
                    x-=10;
                else if (x == 40 && y < 280) // 4. Desce pela esquerda
                    y+=10;
                break;

            default:
                break;
            }


            emit updateGUI(ID, x, y, vel);
        }
        else {
            msleep(200);
        }

        msleep(vel > 0 ? (200 - vel) : 200);
    }
}

void Trem::setVel(int value){
    // Garante que a velocidade esteja entre 0 e 200
    if (value < 0)
        this->vel = 0;
    else if (value > 200)
        this->vel = 200;
    else
        this->vel = value;
}

int Trem::getX(){
    return this->x;
};

void Trem::setX(int x){
    this->x = x;
};

int Trem::getY(){
    return this->y;
};

void Trem::setY(int y){
    this->y = y;
};
