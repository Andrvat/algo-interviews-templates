#include <iostream>
#include <vector>
#include <queue>
#include <stack>

/*
Условие задачи.

Вам даны две последовательности отрезков. 
Каждый отрезок задаётся координатой начала и конца — [ s t a r t i , e n d i ] . 
Отрезки каждой последовательности отсортированы слева направо и не имеют общих точек. 
Найдите пересечение двух последовательностей отрезков. 
То есть третью последовательность отрезков, такую, что: 
    Каждый отрезок содержится в некотором отрезке и первой, и второй последовательности; 
    Никакой отрезок нельзя увеличить; 
    Отрезки этой последовательности не имеют общих точек; 
    Отрезки в последовательности также отсортированы в порядке возрастания.

Первое предложенное решение (Вердикт: ОК).
Каждой границе каждого отрезка из любой последовательности, будь та первая или вторая,
сопоставим позицию на прямой этой границы, является ли эта граница открывающей отрезок или закрывающей,
а также из первой она последовательности либо из второй.
Будем добавлять в очередь с приоритетом все эти границы, да так, чтобы граница с меньшим значением
на прямой была ближе к началу очереди. Так мы отсортируем по возрастанию позиции все-все границы.
Важно отметить, для того, чтобы не пропустить при формировании ответа отрезки нулевой длины, 
необходимо в компараторе (operator<) правильно вернуть значение, когда границы совпадают по своим значениям на оси.
А именно раньше в очереди будет идти та граница, у которой тип -- открывающая.

Далее будет два стека, один для границ первой последовательности, второй -- для второй последовательности.
Со стеком будем работать как в классической задаче про открывающиеся/закрывающиеся скобки.
Будем класть границу в соответствующий стек, если она открывающегося типа.
Если же граница закрывающегося типа, то тут нужно быть осторожным.
Текущая граница с закрывающимся типом может и не дать пересечение, если ее позиция левее чем граница открывающегося типа с другого стека 
(а равно и с другой последовательности).
Если же последнее условие выполняется, то может быть два случая:
    1) граница открывающегося типа с другого стека идет раньше на отрезке чем граница открывающегося типа с его стека, в таком случае получается,
    что отрезок целиком лежит в каком-то отрезке другой последовательности и поэтому в ответ идет внутренний отрезок.
    2) граница открывающегося типа с другого стека идет позже на отрезке чем граница открывающегося типа с его стека, в таком случае в ответ
    идет отрезок, начинающийся на открывающейся границе из одного стека, и заканчивающийся на закрывающейся границе из другого стека.
В любом случае закрывающая граница не добавляется в стек, а лишь удаляет соответствующую открывающуюся границу из соответствующего стека. 
*/

struct Segment {
    int left;
    int right;

    Segment(int left, int right) : left(left), right(right) {
    }
};

std::vector<Segment> readSegments() {
    int n;
    std::cin >> n;
    std::vector<Segment> segments ;
    for (int i = 0; i < n; i++) {
        int left, right;
        std::cin >> left >> right;
        segments.emplace_back(left, right);
    }
    return segments;
}

typedef struct Bracket {
    int pos;
    bool isOpen;
    bool isFromFirst;

    Bracket(int p, bool open, bool first) :
            pos(p), isOpen(open), isFromFirst(first) { }

    friend bool operator<(const Bracket &l, const Bracket &r) {
        if (l.pos == r.pos) {
            return r.isOpen;
        }
        return l.pos > r.pos;
    }
} Bracket;



std::vector<Segment> getIntersection(
        const std::vector<Segment>& firstSequence,
        const std::vector<Segment>& secondSequence) {
    std::priority_queue<Bracket> bracketsQueue;
    for (const auto &segment : firstSequence) {
        bracketsQueue.push(Bracket(segment.left, true, true));
        bracketsQueue.push(Bracket(segment.right, false, true));
    }
    for (const auto &segment : secondSequence) {
        bracketsQueue.push(Bracket(segment.left, true, false));
        bracketsQueue.push(Bracket(segment.right, false, false));
    }
    std::stack<Bracket> firstStack;
    std::stack<Bracket> secondStack;
    std::vector<Segment> intersections;
    while (!bracketsQueue.empty()) {
        auto bracket = bracketsQueue.top();
        bracketsQueue.pop();
        if (bracket.isFromFirst) {
            if (bracket.isOpen) {
                firstStack.push(bracket);
            } else {
                if (!secondStack.empty()) {
                    if (bracket.pos >= secondStack.top().pos) {
                        intersections.emplace_back(std::max(secondStack.top().pos, firstStack.top().pos), bracket.pos);
                    }
                }
                firstStack.pop();
            }
        } else {
            if (bracket.isOpen) {
                secondStack.push(bracket);
            } else {
                if (!firstStack.empty()) {
                    if (bracket.pos >= firstStack.top().pos) {
                        intersections.emplace_back(std::max(firstStack.top().pos, secondStack.top().pos), bracket.pos);
                    }
                }
                secondStack.pop();
            }
        }
    }
    return intersections;
}


void outputAnswer(const std::vector<Segment>& intersection) {
    for (const Segment& segment : intersection) {
        std::cout << segment.left << " " << segment.right << std::endl;
    }
}

int main() {
    std::vector<Segment> firstSequence = readSegments();
    std::vector<Segment> secondSequence = readSegments();

    std::vector<Segment> intersection = getIntersection(firstSequence, secondSequence);
    outputAnswer(intersection);
    return 0;
}