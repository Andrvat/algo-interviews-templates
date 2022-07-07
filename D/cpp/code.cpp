#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


/*
Условие задачи.

Вам дана матрица из n строк и m столбцов, заполненная натуральными числами.
По матрице можно перемещаться, из клетки можно уходить только в соседнюю по стороне клетку,
переходы по диагонали, а также выход за границу матрицы запрещены.
Ваша задача — найти наиболее длинный возрастающий путь в матрице.
Путь возрастающий, если значения в посещаемых клетках строго возрастают от начала пути к его концу.

Следует задать вопрос: какие ограничения на n и m? Особенно чем они ограничены сверху.
Так можно будет примерно определить требуемую асимптотику решения у эффективного алгоритма.
В этой задаче 1 ≤ n , m ≤ 10^3 ==> решение за n^2 вполне сгодится.

Первое предложенное решение (Вердикт: TL).
Представим исходную матрицу M в виде ориентированного графа. Каждый элемент матрицы -- это вершина,
будем давать вершине, соответствующей элементу (i, j) матрицы n x m, число i * m + j.
Ребро из вершины (i1 * m + j1) в (i2 * m + j2) будет <=> элементы (i1, j1) и (i2, j2)
являются соседями в исходной матрице по стороне и при этом M[i1][j1] < M[i2][j2].
Когда граф построен, можно искать поиском в глубину длину максимального пути в этом ор. графе.
Граф м. б. не связан, поэтому нужно проверять каждую вершину.
Наивный подход -- запустить dfs из каждой вершины, поддерживая глобальный максимум.
Dfs работает за O(|V| + |E|) = O(nm) в нашем случае. Вершин всего |V| = nm, поэтому
общее время работы оказалось бы O(n^2 m^2), что медленно.
Можно ускорить поиск ответа с использованием dfs, если для каждой вершины хранить в ней
дополнительную информацию df, а именно "какой максимальный путь до этой вершины был пройден,
прежде чем попали в нее саму?". Эту информацию будем обновлять при каждом обходе.
Как это ускорит -- если мы хотим пойти из одной вершины v в другую вершину u и при этом
df[u] >= df[v] + 1, то мы вершину u в обходе пропускаем.

Первое предложенное решение (Вердикт: OK).
Ускорим работу первого предложенного решения по времени.
Представим, что самый длинный возрастающий путь -- это змейка по матрице,
которая начинается с (0, 0), идет вправо до границы, потом шаг вниз, потом налево до границы и т. д.
Наш первый алгоритм посетит вершину (0, 0), увидит, что из нее нет ребер и завершит dfs с df[(0, 0)] = 0
Потом посетит вершину (0, 1), увидит, что можно сходить в (0, 0), у которой df меньше, поэтому он пойдет
заново в (0, 0) и обновит текущий максимум, который станет равным 1. И так далее. То есть каждый новый
запуск dfs из функции getLongestPathByDfs будет доходить до вершины (0, 0) и обновлять ее df, который
и будет конечным максимумом. Видно, что выходит сложность O(N^2), где N -- количество вершин.
Оптимизируем dfs. Если мы посетили вершину v, то для нее мы знаем самый оптимальный длинный путь,
стартовав из нее. Если есть дуга из u в v, то можно вычислить df[u] = df[v] + 1, ведь в df[v] и так
уже лежит самый оптимальный длинный путь. Таким образом, мы будем улучшать df не для ребенка,
а для предка через ребенка.
*/

typedef struct NodeInfo {
    std::vector<size_t> destinations_;
    size_t df_;
    bool visited_;

    NodeInfo() :
            df_(0),
            visited_(false),
            destinations_({}) {};

    friend bool operator<(const NodeInfo &l, const NodeInfo &r) {
        return l.df_ < r.df_;
    }

} NodeInfo;

void doDfs(NodeInfo &node, std::vector<NodeInfo> &nodes) {
    for (const auto dest: node.destinations_) {
        auto &destNode = nodes[dest];
        if (!destNode.visited_) {
            doDfs(destNode, nodes);
        }
        node.df_ = std::max(node.df_, destNode.df_ + 1);
    }
    node.visited_ = true;
}

size_t getLongestPathByDfs(std::vector<NodeInfo> &nodes) {
    for (auto &node: nodes) {
        if (!node.visited_) {
            doDfs(node, nodes);
        }
    }
    return (*std::max_element(nodes.begin(), nodes.end())).df_;
}

size_t getLongestIncreasingPath(const std::vector<std::vector<int32_t>> &matrix) {
    auto matrixSize = matrix.size();
    std::vector<NodeInfo> nodes(matrixSize * matrix[0].size());
    for (int32_t i = 0; i < matrixSize; ++i) {
        auto rowSize = matrix[i].size();
        for (int32_t j = 0; j < rowSize; ++j) {
            auto pos = i * rowSize + j;
            auto &node = nodes[pos];
            std::vector<std::pair<int32_t, int32_t>> neighbors = {
                    {i - 1, j},
                    {i,     j + 1},
                    {i + 1, j},
                    {i,     j - 1}
            };
            for (const auto &coords: neighbors) {
                if (0 <= coords.first && coords.first < matrixSize &&
                    0 <= coords.second && coords.second < rowSize) {
                    if (matrix[i][j] < matrix[coords.first][coords.second]) {
                        node.destinations_.push_back(coords.first * rowSize + coords.second);
                    }
                }
            }
        }
    }
    return getLongestPathByDfs(nodes) + 1;
}

vector<int32_t> readList(size_t n) {
    vector<int32_t> res(n);
    for (size_t i = 0; i < n; i++) {
        cin >> res[i];
    }
    return res;
}

vector<vector<int32_t>> readMatrix(size_t n, size_t m) {
    vector<vector<int32_t>> matrix(n, vector<int32_t>(m));
    for (size_t i = 0; i < n; i++) {
        matrix[i] = readList(m);
    }
    return matrix;
}

int main() {
    size_t n, m;
    cin >> n >> m;
    vector<vector<int32_t>> matrix = readMatrix(n, m);
    cout << getLongestIncreasingPath(matrix);
}
