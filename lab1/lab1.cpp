/* Завдання до лабораторної роботи 1
   1.  Створіть клас, метод main() якого виводить на консоль слова речення «We have the c++ learning course!» із затримкою 1 секунда.

   2.  Створіть два потоки, один з яких виводить на консоль символ ‘ - ‘, а інший – символ ‘ | ’.Запустіть потоки в основній програмі так,
        щоб вони по черзі виводили свої символи в рядок.Виведіть на консоль 10 таких рядків.

    3.  Створіть клас Counter з методами increment() та decrement(), які збільшують та зменшують значення лічильника відповідно.
        Створіть два потоки, один з яких збільшує 1000 разів значення лічильника, а інший – зменшує 1000 разів значення лічильника.
        Використовуючи методи sleep() та / або join() добийтесь правильної роботи лічильника  при одночасній роботі з ним двох потоків.
*/

#include <iostream>
#include <windows.h>
#include <sstream>
#include <thread>
#include <mutex>
using namespace std;

class PrintSentence {
public:
    void main() {
        string sentence = "We have the c++ learning course!";
        istringstream stream(sentence);
        string word;

        while (stream >> word) {
            cout << word << " ";
            Sleep(1000);
        }
        cout << endl;
    }
};

void printDash() {
    for (int i = 0; i < 10; i++) {
        cout << " - ";
        Sleep(500);
    }
}

void printPipe() {
    for (int i = 0; i < 10; i++) {

        cout << " | ";
        Sleep(500);
    }
}

class Counter {
private:
    int counter = 0;
    mutex mtx;

public:
    void  increment() {
        for (int i = 0; i < 1000; i++) {
            lock_guard<mutex> lock(mtx);
            ++counter;
            //cout  << "+ " << counter << endl;
            Sleep(1);
        }

    }

    void  decrement() {
        for (int i = 0; i < 1000; i++) {
            lock_guard<mutex> lock(mtx);
            --counter;
            //cout << "-" << counter << endl;
            Sleep(1);
        }
    }

    int getCounter() {
        return counter;
    }
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "1) Виведення речення 'We have the c++ learning course!' з паузою в 1 секунду між словами." << endl;

    PrintSentence printSentence;
    printSentence.main();

    cout << "\n2) Запуск двох потоків: один виводить символ '-', а інший символ '|'. Виведення символів чергується в рядок, 10 разів." << endl;

    thread t1(printDash);
    thread t2(printPipe);
    t1.join();
    t2.join();

    cout << "\n\n3) Робота з лічильником: один потік збільшує значення лічильника на 1000, інший зменшує на 1000. Показуємо остаточний результат." << endl ;
    Counter counter;

    thread th1(&Counter::increment, &counter);
    thread th2(&Counter::decrement, &counter);

    th1.join();
    th2.join();

    cout << "\nФінальне значення лічильника: " << counter.getCounter() << endl;

    return 0;
}

