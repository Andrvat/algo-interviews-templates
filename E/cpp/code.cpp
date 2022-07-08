#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
Условие задачи.

Вам дано число, записанное римскими цифрами. Получите это же число в обычной записи (арабскими цифрами).
Римская запись чисел может включать следующие символы:

’I’ — 1
’V’ — 5
’X’ — 10
’L’ — 50
’C’ — 100
’D’ — 500
’M’ — 1000
Цифры ’I’, ’X’, ’C’ и ’M’ нельзя использовать более трех раз подряд. Цифры ’V’, ’L’ и ’D’ нельзя использовать более одного раза во всей записи числа.
Обыкновенно цифры записывают по убыванию слева направо. Например, число 350 будет записано как ’CCCL’.
Однако есть исключения:

Чтобы получить ’4’ или ’9’, надо поставить ’I’ перед ’V’ или ’X’ соответственно
Чтобы получить ’40’ или ’90’, надо поставить ’X’ перед ’L’ или ’C’.
Чтобы получить ’400’ или ’900’, надо поставить ’C’ перед ’D’ или ’M’.
*/

int32_t getArabicRepresentation(const std::string& romanNumber) {
    if (romanNumber.empty()) {
        return -1;
    }
    std::unordered_set<char> romanChars = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};
    std::unordered_set<char> banChars = {'I', 'X', 'C', 'M'};
    const char SEPARATOR = '$';
    std::unordered_map<char, int32_t> arabicMatches = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000}
    };
    std::unordered_map<char, int8_t> frequencyChecker = {
        {'V', 0},
        {'L', 0},
        {'D', 0}
    };
    std::stack<char> romanStack;
    std::stack<int32_t> arabicStack;
    char c = *romanNumber.rbegin();
    if (romanChars.find(c) == romanChars.end()) {
        return -1;
    }
    romanStack.push(c);
    arabicStack.push(arabicMatches[c]);
    if (frequencyChecker.find(c) != frequencyChecker.end()) {
        frequencyChecker[c] += 1;
    }
    for (auto it = romanNumber.rbegin() + 1; it != romanNumber.rend(); ++it) {
        c = *it;
        if (romanChars.find(c) == romanChars.end()) {
            return -1;
        }
        if (frequencyChecker.find(c) != frequencyChecker.end()) {
            frequencyChecker[c] += 1;
        }
        for (const auto &[romanNum, frequency] : frequencyChecker) {
            if (frequency > 1) {
                return -1;
            }
        }
        if (romanStack.top() == 'V' && c == 'I') {
            romanStack.pop();
            romanStack.push(SEPARATOR);
            arabicStack.pop();
            arabicStack.push(4);
        } else if (romanStack.top() == 'X' && c == 'I') {
            romanStack.pop();
            romanStack.push(SEPARATOR);
            arabicStack.pop();
            arabicStack.push(9);
        } else if (romanStack.top() == 'L' && c == 'X') {
            romanStack.pop();
            romanStack.push(SEPARATOR);
            arabicStack.pop();
            arabicStack.push(40);
        } else if (romanStack.top() == 'C' && c == 'X') {
            romanStack.pop();
            romanStack.push(SEPARATOR);
            arabicStack.pop();
            arabicStack.push(90);
        } else if (romanStack.top() == 'D' && c == 'C') {
            romanStack.pop();
            romanStack.push(SEPARATOR);
            arabicStack.pop();
            arabicStack.push(400);
        } else if (romanStack.top() == 'M' && c == 'C') {
            romanStack.pop();
            romanStack.push(SEPARATOR);
            arabicStack.pop();
            arabicStack.push(900);
        } else {
            romanStack.push(c);
            arabicStack.push(arabicMatches[c]);
        }
        if (romanStack.size() > 3) {
            std::vector<char> checkings(4);
            for (auto &x : checkings) {
                x = romanStack.top();
                romanStack.pop();
            }
            if (banChars.find(checkings[0]) != banChars.end() 
                && checkings[0] == checkings[1] 
                && checkings[1] == checkings[2] 
                && checkings[2] == checkings[3]) {
                return -1;
            } else {
                for (auto tmp = checkings.rbegin(); tmp != checkings.rend(); ++tmp) {
                    romanStack.push(*tmp);
                }
            }
        }
    }
    int32_t arabicRepresentation = arabicStack.top();
    arabicStack.pop();
    int32_t last = arabicRepresentation;
    while (!arabicStack.empty()) {
        auto top = arabicStack.top();
        if (top > last) {
            return -1;
        }
        arabicRepresentation += top;
        last = top;
        arabicStack.pop();
    }
    return arabicRepresentation;
}

int main() {
    std::string romanNumber;
    std::cin >> romanNumber;
    std::cout << getArabicRepresentation(romanNumber);
}
