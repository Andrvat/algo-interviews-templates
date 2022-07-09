#include <vector>
#include <iostream>
#include<unordered_set>
#include <numeric>
#include <algorithm>

/*
Условие задачи.

Дано укоренённое дерево на N вершинах и число X.
В каждой вершине записано число — её вес.
Назовём восходящим путь a_i, p(a_i), p(p(a_i)), . . . , где p(a) — родитель вершины a.
Проще говоря, восходящий путь — это путь, который начинается с некоторой вершины
и двигается в сторону корня (не обязательно доходя до него).
Путь может состоять из одной вершины.
Весом пути назовём суммарный вес вершин на этом пути.
Найдите количество восходящих путей с весом X.

Первое предлженное решение (Вердикт: TL).
Положимся на то, что вершины во входном векторе описаны по порядку,
то есть по уровням слево на право как будто бы мы интуитивно рисовали это дерево.
Главная идея -- будем сопоставлять каждой вершине дополнительную информацию, а именно:
1. накопленную сумму при обходе в глубину сверху вниз (от корня до листа).
2. для следующего шага итерации информацию о весе самого дальнего предка, начиная
от уровня обхода на данной итерации.
Обход будет выполняться по уровням -- сначала корень, потом все его дети и т. д.
Уровней в дереве примерно O(logN), на каждом обходе посещаем каждую вершину, начиная
от начального уровня итерации, по разу, значит O(N) вершин посетим. Поэтому общие
затраты по времени займут в среднем O(NlogN). В случае вырожденного дерева затраты будут O(N^2).

Как только мы переходим на уровень ниже (пусть вершина u была на уровне, который мы обработали),
мы должны запомнить, какой вес имела вершина u. После обновить веса для всех вершин поддерева,
в котором вершина u была корнем, а именно вычесть вес вершины u.
По ходу обновлений весов просматривать получающиеся веса и увеличивать счечтик, если вес оказался равен X.

Второе предлженное решение (Вердикт: Не был отправлен).
Нет смысла спускаться дальше по ветки от уровня i к уровню i + 1, если уже на i-м уровне наблюдать
на листья, для которых корень находится на i-м уровне, и проследить, что в листьях сумма уже меньше Х.
Для этого можно по каналу связи прокидывать дальше информацию не только о весе родителя, но и о его индексе (номере вершины).
Как только дойдем до листа и увидим, что сумма меньше X, то нужно подниматься обратно по ветке и сообщать всем
вершинам по пути, что в них больше не нужно заходить.

Третье предлженное решение (Вердикт: ?).
Будем использовать preorder обход в глубину. Идея следующая:
1. Сказать обходу -- я вершина, учитывай меня при вычислении путей. Добавляем текущую вершину
в вектор пути, начиная с корня.
2. Рекурсивно вызываем функцию от всех детей, передавая вектор пути по ссылке (так, чтобы
изменение вектора пути в одной вершине отразились на изменении вектора пути во всех остальных)
3. При обработке вершины u пробегаемся по всем вершинам, которые есть в векторе путей, от КОНЦА к НАЧАЛУ.
Вектор путей будет содержать все вершины, начиная от корня и заканчивая как будто вершина u -- лист.
Если идти от конца к началу, то мы переберем все возможные пути, которые заканчиваются в u, а начинаются
с какого-то более раннего предка.
4. Удаляем вершину из вектора пути. Так мы будем считать ее обработанной. Обход в глубину в нее больше
никогда не зайдет, при этом мы уже смогли перебрать все пути, заканчивающиеся в удаляемой вершине.
Поэтому она больше и не нунжа.


11
22
-1 1
0 2
0 1
1 8
1 6
1 7
3 10
5 11
5 7
6 5
6 2
*/

typedef struct Vertex {
    int32_t weight_;
    int32_t parent_;

    Vertex() = default;

    Vertex(int32_t w, int32_t p) :
            weight_(w), parent_(p) { }
} Vertex;

typedef struct AdvancedVertex {
    const struct Vertex *me_{};
    std::vector<struct AdvancedVertex *> childs_;
} AdvancedVertex;


void traverseTree(AdvancedVertex *vertex, const int32_t x,
                  size_t &upgoingPathsNumber, std::vector<int32_t> &dynamicPath) {
    dynamicPath.push_back(vertex->me_->weight_);
    for (auto child : vertex->childs_) {
        traverseTree(child, x, upgoingPathsNumber, dynamicPath);
    }
    int32_t accumulator = 0;
    for (auto it = dynamicPath.rbegin(); it != dynamicPath.rend(); ++it) {
        accumulator += *it;
        if (accumulator == x) {
            upgoingPathsNumber++;
        }
    }
    dynamicPath.pop_back();
}

size_t getUpgoingPathsNumber(const std::vector<Vertex> &tree, const int32_t x) {
    if (tree.empty()) {
        return 0;
    }
    if (tree.size() == 1) {
        return (tree[0].weight_ == x) ? 1 : 0;
    }
    std::vector<AdvancedVertex> vertices(tree.size());
    size_t rootIndex;
    for (size_t i = 0; i < tree.size(); ++i) {
        auto &vertex = vertices[i];
        vertex.me_ = &tree[i];
        if (vertex.me_->parent_ != -1) {
            vertices[vertex.me_->parent_].childs_.push_back(&vertex);
        } else {
            rootIndex = i;
        }
    }
    size_t upgoingPathsNumber = 0;
    std::vector<int32_t> dynamicPath;
    dynamicPath.reserve(tree.size());
    traverseTree(&vertices[rootIndex], x, upgoingPathsNumber, dynamicPath);
    return upgoingPathsNumber;
}

std::vector<Vertex> readTree(int32_t n) {
    std::vector<Vertex> tree;
    for (int32_t i = 0; i < n; i++) {
        int32_t parent, weight;
        std::cin >> parent >> weight;
        tree.emplace_back(weight, parent);
    }
    return tree;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int32_t n;
    std::cin >> n;
    int32_t x;
    std::cin >> x;
    std::vector<Vertex> tree = readTree(n);
    std::cout << getUpgoingPathsNumber(tree, x);
}
