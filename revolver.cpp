#include "revolver.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

void revolver::new_magazine() {
    srand(time(0));
    random_device rd;
    mt19937 g(rd());

    // Определяем количество пуль (1-5)
    uniform_int_distribution<> dist(1, 5);
    bullets = dist(g);

    // Создаем магазин из 6 ячеек, заполненных нулями
    magazine = vector<int>(6, 0);

    // Заполняем случайные ячейки пулями
    for (int i = 0; i < bullets; ++i) {
        magazine[i] = 1;
    }

    // Перемешиваем пули в магазине
    shuffle(magazine.begin(), magazine.end(), g);

    // Начинаем с первой ячейки
    currentCell = 0;
}

bool revolver::cell() {
    return magazine[currentCell] == 1;
}

int revolver::num_bullets() {
    return bullets;
}

int revolver::num_chambers() {
    return 6 - currentCell;
}


bool revolver::shoot() {
    if (cell()) {
        cout << "Bullet!" << endl;
        bullets--;
        currentCell = (currentCell + 1) % 6;
        if (bullets == 0) {
            std::cout << "No more bullets! Reloading...\n";
            bullets = std::rand() % 5 + 1;
            new_magazine();
        }
        return true;
    }
    cout << "No bullet!" << endl;
    currentCell = (currentCell + 1) % 6;
    return false;
}

void revolver::swap_bullet() {
    if (cell()) {
        magazine[currentCell] = 0;
    }
    else {
        magazine[currentCell] = 1;
    }
}


void revolver::printmagazine() {
    cout << "Magazine [";
    for(int i = 0; i < 6; ++i) {
        if(i == currentCell) {
            std::cout << "(" << magazine[i] << ")";
        } else {
            std::cout << " " << magazine[i] << " ";
        }
        if(i < 5) std::cout << ",";
    }
    cout << "] Bullets: " << bullets << "\n";
}

