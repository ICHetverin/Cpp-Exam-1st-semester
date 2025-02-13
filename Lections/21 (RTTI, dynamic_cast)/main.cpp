#include <iostream>

/*
    * RTTI and dynamic_cast

    Каст в рантайме
    В случае статик каста - понятно либо UB, либо CE, либо без ошибки
    И все это еще на этапе компиляции программы

    Потому что, что реально делает динамик каст?
    Ведь у объекта с виртаульными функциями/наследованием появляется эта табличка с указателями
    динамик каст идет в эту табличку и ищет нужный класс для приведения
    и в зависимости от этого сдвигает пойнтер или нет
    * и это называется механизмом RTTI
    * RTTI - RunTime Type Information

    * Можно еще явно попросить компилятор показать какой тип лежит под данной ссылкой реально

    * 1) Всегда можно делать динамик каст к пойнт звездочке
    (ну естественно от полимфорного типа)

    * 2) Можно делать от наследника к родителю (всегда, даже если не полимфорный)
    - Тут работает как статик каст
    - Но если от родителя к наследднику, то RE

    * 3) Каст вбок (от Мамы к Папе)

    ! Полимфорным должен быть тот, от кого делается каст, а не к кому

    * А чтобы сделать тип полиморфным достаточно указать его деструктор виртуальным

    * Виртуальная функция - это такая, решение вызова которой определяется в рантайме
*/

struct Base {
    int x = 1;
    virtual void f() {}
    virtual ~Base() = default;
};

struct Derived : Base {
    int y = 0;
    void f() override {}
};

int main() {
    {
        Derived d;
        Base& b = d;
        // Вот такое не очень хорошо (если не юзать исключения),
        // потому что если там под Base не лежит Derived, то будет ошибка
        Derived& dc = dynamic_cast<Derived&>(b);
        std::cout << dc.y << std::endl; // 0
        // Вот это можно, и зная что под Base точно лежит Derived
        // можно сделать такйо dynamic_cast
        // Отличие от static_cast в том, что если там был не Derived
        // даст UB, а dynamic_cast - RE (bad cast)
    }
    {   // лучше делать так, тогда при приведениее типа,
        // если под Base не лежит Derived, то будет просто nullptr
        Derived d;
        Base& b = d;
        Derived* dp = dynamic_cast<Derived*>(&b);
        std::cout << dp->y << std::endl;
        // работает только для функций с виртуальными методами
        // потому что только в рантайме понятно
        //! если делать динамик каст с классом без виртуальных функций
        //! то это ошибка еще CE
    }
    {
        Derived d;
        Base& b = d;

        typeid(b); // std::type_info
        //TODO монглированное имя - узнать что это
        std::cout << typeid(b).name() << std::endl; // число - длина имени
        // ну ссылка же на Base, но по факту под ним лежит Derived
        //* можно прям в компиляторе посмотреть что за тип означает это имя
        //* c++filt -t [typeid().name()]

        int x;
        std::cout << typeid(x).name() << std::endl; //i - можно как раз проверить через filt
    }
    {   //! Вот это пипец вообще че за черная магия
        // Если додуматься до такого самому - то ты бог
        Derived d;
        Base& b = d;
        if (Derived* pd = dynamic_cast<Derived*>(&b)) { // иф с инициализацией
            std::cout << pd->y << std::endl;
        }

    }
}