#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/*
Условие задачи.

Вам дан массив натуральных чисел a[i].
Найдите число таких пар элементов (a[i], a[j]),
где ∣a[i] − a[j]∣ % 200 == 0 и i < j.

Первое предоженное решение (Вердикт: ?).
Если разность двух чисел должно без остатка делиться на 200,
то это значит, что разность делений с остатком этих чисел на 200
должна давать 0. Поэтому подходят два числа в пару, если у них
одинаковый остаток от деления на 200.
Нам нужно число таких пар, значит нужно посчитать количество чисел,
у которых остаток на 200 одинаков, а после посчитать, сколько уникальных
пар можно составить из них.
Если всего чисел с одинаковым остатком на 200 k штук, то пар можно
составить (1 + (k - 1)) / 2 * (k - 1) -- сумма арифметической прогрессии
от 1 до (k - 1).

*/

constexpr int32_t DELIM = 200;

size_t getPairsNumber(const std::vector<int32_t> &numbers) {
    std::unordered_map<int32_t, int32_t> remainderNumbers;
    for (const auto number : numbers) {
        int32_t remainder = number % DELIM;
        remainderNumbers[remainder] += 1;
    }
    size_t totalPairs = 0;
    for (const auto &[remainder, totalNumber] : remainderNumbers) {
        totalPairs += totalNumber * (totalNumber - 1) / 2;
    }
    return totalPairs;
}

size_t readNumber() {
    size_t x;
    cin >> x;
    return x;
}

vector<int32_t> readList(size_t n) {
    vector<int32_t> res(n);
    for (size_t i = 0; i < n; i++) {
        cin >> res[i];
    }
    return res;
}

int main() {
    size_t n = readNumber();
    vector<int32_t> numbers = readList(n);
    cout << getPairsNumber(numbers);
}
