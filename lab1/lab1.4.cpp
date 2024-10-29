/* Завдання до лабораторної роботи 1
    4.  Реалізуйте стрічковий алгоритм або алгоритм Фокса множення матриць.Результат множення записуйте в об’єкт класу Result.
        Виконайте експерименти з різною кількістю потоків та різною розмірністю матриць, які перемножуються, реєструючи час виконання.
        Побудуйте графіки відповідних залежностей.
*/

#include <iostream>
#include <windows.h>
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>

#include <vector>

using namespace std;

class Matrix {
public:
    const int rows;
    const int cols;
    int** matrix = nullptr;
    Matrix():rows(1), cols(1){
        matrix = new int* [1];
        for (int i = 0; i < 1; ++i) {
            matrix[i] = new int[1];
        }


        for (int i = 0; i < 1; ++i) {
            for (int j = 0; j < 1; ++j) {
                matrix[i][j] = 0;
            }
        }
    }
    Matrix(int size_rows, int size_cols) : rows(size_rows), cols(size_cols){
        matrix = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            matrix[i] = new int[cols];
        }


        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] = 0;
            }
        }
    }
    ~Matrix() {
        for (int i = 0; i < rows; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void set(vector<vector<int>> m) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] = m[i][j];
            }
        }
    }

    int& get(int x, int y){
        return matrix[x][y];
    }

    void print() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << matrix[i][j]<<" | ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class Result {
public:
    Matrix *data;
    double Time;
    Result() {
        data = new Matrix();
        Time = 0.0f;
    }
};

Result& foxAlgorithm(Matrix &A, Matrix &B) {
    int size = A.cols;

    Matrix *C = new Matrix{ size, size };

    vector<thread> threads;
    mutex mtx;

    cout << "Запуск алгоритму Фокса для множення матриць розмірності " << size << "x" << size << endl;

    auto start = chrono::high_resolution_clock::now();


    auto threadFunction = [&](int i, int j) {
        for (int k = 0; k < A.rows; ++k) {
            lock_guard<mutex> guard(mtx);
            C->get(i, j) += A.get(k, j) * B.get(i, k);
        }
         cout << "Обчислено значення для комірки (" << i << ", " << j << ")\n";
    };
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            threads.push_back(thread(threadFunction, i, j));
        }
    }

    cout << "Кількість потоків: " << threads.size() << endl;

    for (auto& thread : threads) {
        thread.join();
    }
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsedTime = end - start;
    
    Result result = Result();
    result.data = C;
    result.Time = elapsedTime.count();

    cout << "Алгоритм завершено. Час виконання: " << result.Time << " секунд" << endl;

    return result;
}

int main()
{
    Matrix A{ 3,5 };
    Matrix B{ 5,3 };

    A.set({
        { 1,  2,  3,  4,  5 },
        { 6,  7,  8,  9,  10 },
        { 11, 12, 13, 14, 15 }
        });

    B.set({
        { 1,  2,  3},
        { 4,  5,  6},
        { 7,  8,  9},
        { 10, 11, 12},
        { 13, 14, 15}
        });
        
    Result r = foxAlgorithm( A, B);
    r.data->print();
    cout << "Час, витрачений на множення матриць: " << r.Time << " секунд" << endl;

    return 0;
}

