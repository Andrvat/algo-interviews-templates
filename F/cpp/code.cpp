#include <iostream>
#include <string>
#include <vector>
#include <cmath>

/*
Условие задачи.

Назовем строку хорошей, если в ней нет двух соседних букв, которые различаются только регистром. 
Например, строка «abba» хорошая, а строка «aBba» нет.
Со строкой можно делать преобразование: если два соседних символа обозначают одну и ту же букву, 
но записаны в разных регистрах, то их можно удалить. 
При этом строка «схлопнется», то есть пробелов при удалении не образуется.
Цепочкой таких преобразований можно превратить любую строку в хорошую.
По заданной строке найдите хорошую строку, в которую ее можно превратить.

Первое предложенное решение (Вердикт: OK).
Будем использовать метод двух указателей.
Указатели стартуют вместе, первый на 0й элемент, второй -- на 1й.
Они двигается на один символ вперед вместе, пока не встретят Xx (некоторую букву в разном регистре).
Как только они находят этот Xx, _ _ _ X x _ _ _ _ _, то левый указатель смещается влево и указывает
на символ до Х. Симетрично поступает правый указатель -- он смещается на символ за x.
После этого два указателя пытаются идти в разные стороны и схлопывать буквы в разном регистре.
Как только у них это не получается, левый указатель встает на место правого, а правый сдвигается на единицу вперед.
Пример.
- vxOoOoVvx
    ||

- vx__OoVvx
   |  |

- vx__OoVvx
      ||

- vx____Vvx
   |    |

- vx____Vvx
        ||

- vx______x
   |      |

Алгоритм заканчивает работу, когда правый указатель доходит до конца строки и попробовал схлопнуть две буквы вместе с левым указателям.
TODO: как делать быстрый за О(1) прыжок через схлопнутые элементы?
Решение: хранить дополнительный массив прыжков, для каждого символа исходной строки
свой прыжок (сколько единиц нужно перепрыгуть налево), изменяющийся динамически (сначала у всех 1).
Как только схлопываем два символа и как только сдвинули указатели, для символа, на который указывает
передвинутый правый указатель, записывать значение прыжка = разнице правого указателя и левого.
Важно!!! Передвигаем левый указатель на столько позиций, сколько записано у символа, на который
этот самый левый указатель показывает ПЕРЕД удалением.
*/

std::string getGoodStringFrom(const std::string &inputString) {
    if (inputString.size() <= 1) {
        return inputString;
    }
    std::string goodString = inputString;
    std::vector<bool> markers(inputString.size(), true);
    std::vector<int32_t> jumps(inputString.size(), 1);
    auto left = goodString.begin();
    auto right = goodString.begin() + 1;
    const int32_t LETTERS_DIFF = std::abs('A' - 'a');
    while (right < goodString.end()) {
        char leftChar = *left;
        char rightChar = *right;
        if (std::abs(leftChar - rightChar) == LETTERS_DIFF) {
            int32_t leftIndex = left - goodString.begin();
            int32_t rightIndex = right - goodString.begin();
            markers[leftIndex] = false;
            markers[rightIndex] = false;
            left -= jumps[leftIndex];
            right++;
            if (right != goodString.end()) {
                jumps[rightIndex + 1] = right - left;
            }
            if (left == goodString.begin() - 1) {
                left = right;
                right++;
            }
        } else {
            left = right;
            right++;
        }
    }
    std::string result;
    for (int32_t i = 0; i < markers.size(); ++i) {
        if (markers[i]) {
            result += goodString[i];
        }
    }
    return result;
}

int main() {
    std::string probablyBadString;
    std::cin >> probablyBadString;
    std::cout << getGoodStringFrom(probablyBadString);
} 