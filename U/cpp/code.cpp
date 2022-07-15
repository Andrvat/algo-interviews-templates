#include <iostream>
#include <vector>
#include <stack>

/*

В этой задаче под правильной скобочной последовательностью (ПСП) понимается правильная скобочная последовательность из круглых и квадратных скобок, где ни одна пара квадратных скобок не может содержать пару круглых скобок.
По данному числу n выведите все ПСП из n скобок в лексикографическом порядке.
Упорядоченность символов следующая: ’(’ < ’[’ < ’)’ < ’]’

Предложенное решение (Вердикт: ОК):
Берем за основу https://www.geeksforgeeks.org/print-all-combinations-of-balanced-parentheses/ -- 
генерацию всех последовательностей длины n с одним типом скобок. Здесь отличается только тем, что нужно наложить
ограничения на добавления новых скобок определенного типа.
Так,
    1) мы не можем ставить откр. круглых скобок больше чем всего допустимо открывающихся минус сколько уже поставили открыв. квадратных скобок.
    2) мы не можем ставить откр. квадратных скобок больше чем всего допустимо открывающихся минус сколько уже поставили открыв. круглых скобок.
    3) мы не можем ставить закрыв. круглых скобок больше чем поставили открыв. круглых скобок
    4) мы не можем ставить закрыв. квадратных скобок больше чем поставили открыв. квадратных скобок
    Дополнительно
    5) если мы ставим открыв. круглую скобку, то последняя не должна быть открыв. квадратная, иначе допустима вложенность круглых в квадратные, что запрещено по усл.
    6) если мы ставим открыв. круглую скобку, то количество открытых квадрат. скобок должно равняться количеству закрытых квадрат. скобок, иначе ЗА текущей круглой скобкой (которую хотим поставить) количество закрыв. квадратных скобок будет больше чем количество открыв. квадратных скобок, что вообще не является ПСП
    7) если мы ставим закрыв. круглую скобку, то количество открытых квадрат. скобок должно равняться количеству закрытых квадрат. скобок, ииначе допустима вложенность круглых в квадратные, что запрещено по усл.
    8) если мы ставим закрыв. квадратную скобку, то последняя не должна быть закрыв. круглая, иначе допустима вложенность круглых в квадратные, что запрещено по усл.
*/

struct BracketsNumberInfo {
    int32_t total;
    int32_t openedRound = 0;
    int32_t openedSquare = 0;
    int32_t closedRound = 0;
    int32_t closedSquare = 0;

    explicit BracketsNumberInfo(int32_t total) : total(total) {}
};

void outputAnswer(const std::vector<std::string> &sequences) {
    for (const auto &sequence: sequences) {
        std::cout << sequence << std::endl;
    }
}

void findNextSequence(BracketsNumberInfo &info, std::string current, std::vector<std::string> &collector) {
    if (info.openedRound + info.openedSquare == info.total &&
        info.closedRound + info.closedSquare == info.total) {
        collector.push_back(current);
        return;
    }
    if (info.openedRound < info.total - info.openedSquare && current.back() != '[' && info.closedSquare == info.openedSquare) {
            info.openedRound++;
            findNextSequence(info, current + "(", collector);
            info.openedRound--;
    }
    if (info.openedSquare < info.total - info.openedRound) {
        info.openedSquare++;
        findNextSequence(info, current + "[", collector);
        info.openedSquare--;
    }
    if (info.closedRound < info.openedRound && info.closedSquare == info.openedSquare) {
        info.closedRound++;
        findNextSequence(info, current + ")", collector);
        info.closedRound--;
    }
    if (info.closedSquare < info.openedSquare && current.back() != ')') {
        info.closedSquare++;
        findNextSequence(info, current + "]", collector);
        info.closedSquare--;
    }
}

std::vector<std::string> generateSequences(int32_t n) {
    std::vector<std::string> sequences;
    if (n % 2 == 1) {
        return sequences;
    }
    BracketsNumberInfo info(n / 2);
    findNextSequence(info, "", sequences);
    return sequences;
}

void check(std::vector<std::string> &v) {
    for (const auto &s : v) {
        std::stack<char> stack;
        bool isOk = true;
        for (const auto c : s) {
            if (stack.empty()) {
                stack.push(c);
            } else {
                switch (c) {
                    case '(' : {
                        stack.push(c);
                        break;
                    }
                    case '[': {
                        stack.push(c);
                        break;
                    }
                    case ')': {
                        if (stack.top() == '(') {
                            stack.pop();
                        } else {
                            std::cerr << s << std::endl;
                            isOk = false;
                        }
                        break;
                    }
                    case ']': {
                        if (stack.top() == '[') {
                            stack.pop();
                        } else {
                            std::cerr << s << std::endl;
                            isOk = false;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            if (!isOk) {
                break;
            }
        }
    }
}

int main() {
    int n;
    std::cin >> n;
    auto v = generateSequences(n);
    outputAnswer(v);
    check(v);
    return EXIT_SUCCESS;
}