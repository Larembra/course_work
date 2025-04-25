#ifndef REVOLVER_H
#define REVOLVER_H

//#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>



class revolver {
private:
    std::vector<int> magazine;  // Магазин на 6 ячеек
    int currentCell;       // Текущая ячейка (0-5)
    int bullets;
public:
    void new_magazine(); //Если пули кончились, а игра продолжается, револьвер перезаряжается

    // Проверка текущей ячейки
    bool cell();

    int num_bullets();

    int num_chambers();

    bool shoot();

    void swap_bullet();
    // Вывод состояния магазина (для отладки)
    void printmagazine();
};



#endif //REVOLVER_H
