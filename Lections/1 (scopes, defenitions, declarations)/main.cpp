#include <iostream>
/*
    *Объявление, определение
    Как происходит при линковке - ведь просто подставляется содержимое
    соответствующего файла, а значит например у некоторых структур должны быть
    указаны переопределение или ровно такое же определение.

    Примеры:
    1) Namespace его можно доопределять (ведь при линковке это пространство
    собирается из кучи, лежащих в разных файлах)

    2) Для функций может быть много объявлений, но определение всегда одно. Также
    перегрузка функций - это когда например на одно имя функции имеется несколько
    разных сигнатур

    3) Class - у него во всех файлах может быть только одно определение, ОДИНАКОВОЕ

    *Области видимости
    Значит, у каждой переменной есть своя область видимости, в которой она
    существует. При компиляции приоритет отдается локальным переменным, и только потом
    глобальным

    *Перегрузка функций
    Уже немного затрагивали, но вернемся
    Возникают проблемы, конечно, при неявных приведениях типов
    Например:
    1) int f(double a, double b) {
        return a + b;
    }
    (можем спокойно брать int, float -> double при подстановке, результат же будет округлен
    т е возьмем только целую часть и все)

    2)  int f(int a, float b) { return a + b; }
        int f(float a, int b) { retur a + b; }
    опять же ответ будет округлен (т е возьмем только целую часть и все), НО
    тут возникнет ошибка6 если мы дадим два float или два int на вход - комплиятору
    одинаково "плохо", что то, что то
*/

// 1)
namespace N {
    int x = 12000;
}

namespace N {
    int y = 1000;
}

// 2)
void f();
int f(int a, int b);
//! int f(); - так делать нельзя, менять возвращаемый тип, но не менять входные
int f(int a, int b);
//* definition
int f (int a, int b) {
    return a + b;
}

//* Scopes
int x = 5;
int x1 = 15;

//* Functions
int f1(double a, double b) { return a + b; }
// int f1(double a, int b) { return a + b; }
// int f1(int a, double b) { return a + b; }

int main () {
    // using N::x, N::y; // можем делать так, через запятую с 17 версии
    // std::cout << f(x, y);
    std::cout << x; // 5 - global
    int x = 12;
    std::cout << x; // 12 - local
    {
        int x = 15;
        std::cout << x; // 15 - local local* (можем делать новые о.в. в локалке)
    }

    // уже есть x = 12 в этой области видимости, если попробовать вынести из пространства имен
    // - будет ошибка (declarations conflict - непонятно что брать)
    //! using N::x;
    //! std::cout << x;

    std::cout << '\n' << f1(1, 2); // 3
    std::cout << '\n' << f1(1.2, 2.5); // 3
    //! error: call to 'f1' is ambiguous
    // это если закомментить первую функцию

    {
        // можно кстати брать что-то из глобала так:
        std::cout << '\n' << ::x1; // 15
    }
}