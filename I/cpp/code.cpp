#include "solution.h"

#include <vector>

using namespace std;

//  Comment it before submitting
// struct Node {
//     int val;
//     Node* next;
//     Node(int val_, Node* next_) {
//         val = val_;
//         next = next_;
//     }
// };

/*
Условие задачи.

В этой задаче требуется сделать разворот части односвязного списка. 
Каждая вершина списка описывается структурой Node, 
каждый экземпляр хранит указатель на следующую вершину или null (nullptr, None, nil), 
если вершина последняя. 
По заданным индексам from и to разверните все вершины на отрезке 
с from до to включительно. 
Заметьте, что нумерация в индексах from и to с единицы.

Первое предложенное решение (Вердикт: OK).
Применяем метод двух указателей.
Сохраним все указатели в вектор так, чтобы было удобно обращаться к элементам
списка по индексам (оптимизация по времени).
Будем свопать ноды, на которые указывают соответственно левый и правый указатели.
Не забываем перекинуть некст предлевой ноды на правую ноду,
некст предправой ноды на левую ноду,
некст левой ноды на ноду, следующей за правой,
некст правой ноды на ноду, следующей за левой.
Последние операции можно делать за O(1), если знать индексы и иметь вектор указателей.

При всем при этом нужно быть аккуратными с индексами (например, не забыть обновить голову,
если left = 1), 
а также в случае, когда некст левого указателя -- это правый указатель. 

Сложность: O(n) (время и память).
*/


Node* Reverse(Node* head, int left, int right) {
    left--;
    right--;
    if (left >= right || left < 0 || right < 0) {
        return head;
    }
    std::vector<Node *> savedPointers;
    Node *it = head;
    while (it) {
        savedPointers.push_back(it);
        it = it->next;
    }
    Node *savedHead = head;
    while (left < right) {
        Node *leftPtr = savedPointers[left];
        Node *rightPtr = savedPointers[right];

        if (left >= 1) {
            savedPointers[left - 1]->next = rightPtr;
        } else {
            savedHead = rightPtr;
        }

        if (right - left > 1) {
            savedPointers[right - 1]->next = leftPtr;
        }

        leftPtr->next = rightPtr->next;

        if (right - left > 1) {
            rightPtr->next = savedPointers[left + 1];
        } else {
            rightPtr->next = leftPtr;
        }


        Node *tmp = savedPointers[left];
        savedPointers[left] = savedPointers[right];
        savedPointers[right] = tmp;

        left++;
        right--;
    }
    return savedHead;
}