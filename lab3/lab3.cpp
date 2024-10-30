/*Завдання до лабораторної роботи 3
    «Дослідження паралельних алгоритмів сортування колекцій»

    1.  Розробити класи у відповідності до варіанту завдання.

    2.  Створити компаратори класу за різними можливими способами сортування екземплярів класу.

    3.  Створити колекцію екземплярів класу та виконати сортування елементів колекції з використанням алгоритмів звичайного сортування та сортування з  паралельними обчисленнями,
        реалізованих у стандартних бібліотеках Java. Використовуйте як сортування за одним критерієм, так і за кількома одночасно. Наприклад, відсортувати готелі міста за категорією
        та за назвою міста розташування.

    4.  Збільшуючи обсяг масиву, що сортується, вимірювати час виконання програми.
        Побудувати графік залежності часу виконання програми від розміру масиву для методу звичайного сортування та сортування з паралельними обчисленнями. Порівняти результати та зробити висновки.

Варіант 5 - Авто з полями Номер авто, Тип, Об’єм двигуна, Вартість.
*/

#include <iostream>
#include <windows.h>
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>

#include <random>
#include <vector>
#include <functional>

using namespace std;

enum CarType {
    A, B, C, D, E, F, S,
};

class Car {
public:
    int carNumber;
    CarType type;
    double engineSize;
    double cost;

    void print() {
        cout << "Автомобіль (" << carNumber << ", тип: " << type << ", об'єм двигуна: " << engineSize << ", вартість: " << cost << ")" << endl;
    }
};

void sort_carNumber(vector<Car>& list, int start_id, int end_id) {
    if (start_id < end_id) {
        for (int i = start_id; i < end_id; ++i) {
            if (list[start_id].carNumber > list[i].carNumber) {
                swap(list[start_id], list[i]);
            }
        }
        sort_carNumber(list, start_id + 1, end_id);
    }
}

void start_carNumber(vector<Car>& list, int start_id, int end_id) {
    cout << "Сортування за номером авто виконано в окремому потоці" << endl;
    sort_carNumber(list, start_id, end_id);
    cout << "Сортування за номером авто завершено" << endl;
}

void sort_type(vector<Car>& list, int start_id, int end_id) {
    if (start_id < end_id) {
        for (int i = start_id; i < end_id; ++i) {
            if (list[start_id].type > list[i].type) {
                swap(list[start_id], list[i]);
            }
        }
        sort_carNumber(list, start_id + 1, end_id);
    }

}

void start_type(vector<Car>& list, int start_id, int end_id) {
    cout << "Сортування за типом авто виконано в окремому потоці" << endl;
    sort_type(list, start_id, end_id);
    cout << "Сортування за типом авто завершено" << endl;
}

void sort_engineSize(vector<Car>& list, int start_id, int end_id) {
    if (start_id < end_id) {
        for (int i = start_id; i < end_id; ++i) {
            if (list[start_id].engineSize > list[i].engineSize) {
                swap(list[start_id], list[i]);
            }
        }
        sort_carNumber(list, start_id + 1, end_id);
    }

}

void start_engineSize(vector<Car>& list, int start_id, int end_id) {
    cout << "Сортування за об'ємом двигуна завершено" << endl;
    sort_engineSize(list, start_id, end_id);
    cout << "Сортування за об'ємом двигуна виконано в окремому потоці" << endl;
}

void sort_cost(vector<Car>& list, int start_id, int end_id) {
    if (start_id < end_id) {
        for (int i = start_id; i < end_id; ++i) {
            if (list[start_id].cost > list[i].cost) {
                swap(list[start_id], list[i]);
            }
        }
        sort_carNumber(list, start_id + 1, end_id);
    }
}

void start_cost(vector<Car>& list, int start_id, int end_id) {
    cout << "Сортування за вартістю авто завершено" << endl;
    sort_cost(list, start_id, end_id);
    cout << "Сортування за вартістю авто виконано в окремому потоці" << endl;
}

vector<Car> createList(int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 1000);
    uniform_int_distribution<> distrib2(0, 100);
    uniform_int_distribution<> distrib_Type(0, 6);


    vector<Car> listCar;
    vector<CarType> listType{ CarType::A, CarType::B, CarType::C, CarType::D, CarType::E, CarType::F, CarType::S };
    for (int i = 0; i < size; ++i) {
        int random_carNumber = distrib(gen);
        int random_Type = distrib_Type(gen);
        double random_engineSize = distrib(gen);
        double random_engineSize_point = distrib2(gen);
        double random_cost = distrib(gen);
        double random_cost_point = distrib2(gen);

        Car A{ random_carNumber, listType[random_Type], random_engineSize + random_engineSize_point / 100, random_cost + random_cost_point / 100 };
        listCar.push_back(A);
    }
    return listCar;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int size = 100;
    //======================================================================================================================
    cout << "=== Початок виконання сортування в потоках ===" << endl;

    vector<Car> listCar1 = createList(size);
    vector<Car> listCar2 = createList(size);
    vector<Car> listCar3 = createList(size);
    vector<Car> listCar4 = createList(size);

    auto start = chrono::high_resolution_clock::now();

    thread th1(&start_carNumber, ref(listCar1), 0, size);
    thread th2(&start_type, ref(listCar2), 0, size);
    thread th3(&start_engineSize, ref(listCar3), 0, size);
    thread th4(&start_cost, ref(listCar4), 0, size);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsedTime = end - start;

    cout << "Час виконання з потоками: " << elapsedTime.count() << " секунд" << endl;

    //======================================================================================================================
    cout << "\n=== Початок виконання сортування без потоків ===" << endl;

    listCar1 = createList(size);
    listCar2 = createList(size);
    listCar3 = createList(size);
    listCar4 = createList(size);

    start = chrono::high_resolution_clock::now();
    start_carNumber(listCar1, 0, size);
    start_type(listCar2, 0, size);
    start_engineSize(listCar3, 0, size);
    start_cost(listCar4, 0, size);
    end = chrono::high_resolution_clock::now();

    elapsedTime = end - start;

    cout << "Час виконання без потоків: " << elapsedTime.count() << " секунд" << endl;

    return 1;
}

