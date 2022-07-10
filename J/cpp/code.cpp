#include <vector>
#include <iostream>

/*
Условие задачи.

Определите все расстановки из n ферзей на шахматной доске n × n , 
где ферзи не могут бить друг друга. 
В качестве ответа на задачу выведите в первой строке число расстановок, 
а далее все расстановки в следующем формате: 
одна расстановка описывается n числами. 
i -е число описывает i -ую строку доски, а именно оно равно номеру клетки, 
в которой стоит ферзь на текущей строке. 
Строки нумеруются сверху вниз от 1 до n . 
Клетки внутри строки нумеруются от 1 до n слева направо.

Первое предложенное решение (Вердикт: OK).
Рекурсия обеспечивает движение по строкам, цикл for внутри рекурсии -- по столбцам.
В каждом вызове рекурсивной функции мы на текущей строке row пытаемся найти такую клетку,
то есть такой столбец, чтобы можно было поставить ферзя в строке row и это не противоречило
уже расставленным на строках от 0 до row - 1 вкл. другим ферзям.
Для этого проверяем столбец, куда хотим поставить, строку и диагонали. Если везде в этих местах
нет ферзей, поставленных ранее, то размещаем ферзя в выбранный столбец на row строке
и вызываем рекурсивно функцию, только в параметр уже передаем строку row + 1.
Выход из рекурсии -- когда пытаемся найти место на строке row=n, то есть когда вышли за
шахматную доску. Чтобы сэкономить по времени, с каждой расстановкой ферзя на row строке
запоминаем столбец, где ферзь был поставлен. Если по рекурсии откатываемся назад, то
убираем ферзя из ранее выбранного столбца и удаляем запоминаемый столбец.
Расстановка нам подходит <=> на последней строке мы нашли место, куда поставить ферзя.
На всех предыдущих строках ферзи уже расставлены, так как иначе до последней строки
мы бы просто не добрались, не позволила бы рекурсия.
*/

bool isPossibleToPlaceQueen(const std::vector<bool> &chessboard, const int32_t n,
                                const int32_t row, const int32_t col) {
    for (int32_t i = 0; i < n; ++i) {
        if (chessboard[row * n + i]) {
            return false;
        }
        if (chessboard[i * n + col]) {
            return false;
        }
    }
    int32_t rowCopy = row - 1;
    int32_t colCopy = col - 1;
    while (rowCopy >= 0 && colCopy >= 0) {
        if (chessboard[rowCopy * n + colCopy]) {
            return false;
        }
        rowCopy--;
        colCopy--;
    }
    rowCopy = row + 1;
    colCopy = col + 1;
    while (rowCopy < n && colCopy < n) {
        if (chessboard[rowCopy * n + colCopy]) {
            return false;
        }
        rowCopy++;
        colCopy++;
    }
    rowCopy = row + 1;
    colCopy = col - 1;
    while (rowCopy < n && colCopy >= 0) {
        if (chessboard[rowCopy * n + colCopy]) {
            return false;
        }
        rowCopy++;
        colCopy--;     
    }
    rowCopy = row - 1;
    colCopy = col + 1;
    while (rowCopy >= 0 && colCopy < n) {
        if (chessboard[rowCopy * n + colCopy]) {
            return false;
        }
        rowCopy--;
        colCopy++;     
    }
    return true;
}

void findCombinations(std::vector<std::vector<int32_t>> &peacefulCombinations,
                        std::vector<bool> &chessboard, std::vector<int32_t> &currentPlacement,
                        const int32_t n, const int32_t row) {
    if (row >= n) {
        return;
    }
    for (int32_t i = 0; i < n; ++i) {
        if (isPossibleToPlaceQueen(chessboard, n, row, i)) {
            chessboard[row * n + i] = true;
            currentPlacement.push_back(i + 1);
            if (row == n - 1) {
                peacefulCombinations.push_back(currentPlacement);
            }
            findCombinations(peacefulCombinations, chessboard, currentPlacement, n, row + 1);
            chessboard[row * n + i] = false;
            currentPlacement.pop_back();
        }
    }
}

std::vector<std::vector<int32_t>> getAllPeacefulCombinations(const int32_t n) {
    std::vector<std::vector<int32_t>> peacefulCombinations;
    std::vector<bool> chessboard(n * n, false);
    std::vector<int32_t> currentPlacement;
    findCombinations(peacefulCombinations, chessboard, currentPlacement, n, 0);
    return peacefulCombinations;
}

void printArray(const std::vector<int32_t>& combination) {
    for (int32_t elem : combination) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void outputAnswer(const std::vector<std::vector<int32_t>>& possibleCombinations) {
    std::cout << possibleCombinations.size() << std::endl;
    for (const std::vector<int32_t>& combination : possibleCombinations) {
        printArray(combination);
    }
}

int main() {
    int32_t n;
    std::cin >> n;
    std::vector<std::vector<int32_t>> possibleCombinations = getAllPeacefulCombinations(n);
    outputAnswer(possibleCombinations);
}




