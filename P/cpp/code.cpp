#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/*
Условие задачи.
Дано укоренённое бинарное дерево на N вершинах. 
Скажем, что вершина v находится на границе дерева, если она подходит под любое из условий: 
    -- v является листом; 
    -- пусть v находится на расстоянии h от корня. Тогда v — самая левая или самая правая вершина 
        среди всех вершин на расстоянии h от корня. 
Найдите все вершины, находящиеся на границе дерева.

Первое предложенное решение (Вердикт: ОК).
Будем использовать поуровневый обход бинарного дерева в ширину.
Чтобы разделять уровни, будем в очередь вершин класть на границе уровней
вершину-алиас. Если смотреть на один определенный уровень, то вершины на границе это:
    1) самая левая (первая на уровне) вершина
    2) самая правая (последняя на уровне) вершина
    3) любой лист
Поэтому мы с помощью вершины-алиас понимаем размер следующего уровня, и поэтому
несложно вычисляем вершины, попадающие под пункты 1) и 2).
Вершина-алиас кладется в очередь <=> в while обрабатывается первая вершина на текущем уровне.

*/

struct Vertex {
    int left;
    int right;

    Vertex(int left, int right) : left(left), right(right) {
    }
};

struct NumericVertex {
    const Vertex *instance;
    int no;

    NumericVertex(const Vertex *p, int n) : instance(p), no(n) {
    }
};


vector<int> getTreeBorder(const vector<Vertex> &tree, int root) {
    std::vector<int> borderVertices;
    constexpr int32_t ALIAS_VERTEX_NO = -2;
    constexpr int32_t NO_CHILD = -1;
    Vertex interLevelVertex(ALIAS_VERTEX_NO, ALIAS_VERTEX_NO);
    NumericVertex aliasVertex(&interLevelVertex, ALIAS_VERTEX_NO);
    std::queue<NumericVertex> toTraverseVertices;
    toTraverseVertices.push(NumericVertex(&tree[root], root));
    int32_t currentLevelSize = 1;
    int32_t levelVertexCounter = 0;
    while (!toTraverseVertices.empty()) {
        auto currentVertex = toTraverseVertices.front();
        toTraverseVertices.pop();
        if (currentVertex.no == ALIAS_VERTEX_NO) {
            currentLevelSize = toTraverseVertices.size();
            levelVertexCounter = 0;
            continue;
        }
        if (toTraverseVertices.size() + 1 == currentLevelSize && levelVertexCounter == 0) {
            toTraverseVertices.push(aliasVertex);
        }
        bool isCurrentYetBorder = false;
        if (levelVertexCounter == 0 || levelVertexCounter == currentLevelSize - 1) {
            borderVertices.push_back(currentVertex.no);
            isCurrentYetBorder = true;
        }
        levelVertexCounter++;
        if (currentVertex.instance->left == NO_CHILD &&
            currentVertex.instance->right == NO_CHILD &&
            !isCurrentYetBorder) {
            borderVertices.push_back(currentVertex.no);
        }
        auto leftNo = currentVertex.instance->left;
        if (leftNo != NO_CHILD) {
            toTraverseVertices.push(NumericVertex(&tree[leftNo], leftNo));
        }
        auto rightNo = currentVertex.instance->right;
        if (rightNo != NO_CHILD) {
            toTraverseVertices.push(NumericVertex(&tree[rightNo], rightNo));
        }
    }
    return borderVertices;
}

void outputAnswer(const vector<int> &treeBorder) {
    for (int elem: treeBorder) {
        cout << elem << " ";
    }
    cout << endl;
}

vector<Vertex> readTree(int n) {
    vector<Vertex> tree;
    for (int i = 0; i < n; i++) {
        int left, right;
        cin >> left >> right;
        tree.push_back(Vertex(left, right));
    }
    return tree;
}

int main() {
    int n;
    cin >> n;
    int root;
    cin >> root;
    vector<Vertex> tree = readTree(n);
    outputAnswer(getTreeBorder(tree, root));
    return 0;
}

