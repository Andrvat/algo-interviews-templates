#include "solution.h"

#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <queue>

using namespace std;

//Comment it before submitting
// struct Node {
//     int val;
//     std::vector<Node*> neighbours;
//     Node(int val_) {
//         val = val_;
//         neighbours = {};
//     }
// };

struct QueueNode {
    struct Node *node;
    struct Node *parent;

    QueueNode(struct Node *n, struct Node *p) :
            node(n), parent(p) { }
};

Node* cloneGraph(Node* node) {
    std::unordered_map<Node *, Node *> cloneMapping;
    std::queue<QueueNode> toCloneNodes;
    toCloneNodes.push(QueueNode(node, nullptr));
    Node *head = nullptr;
    while (!toCloneNodes.empty()) {
        auto toClone = toCloneNodes.front();
        toCloneNodes.pop();
        if (cloneMapping.find(toClone.node) == cloneMapping.end()) {
            Node *cloned = new Node(toClone.node->val);
            cloneMapping[toClone.node] = cloned;
            for (auto &x : toClone.node->neighbours) {
                toCloneNodes.push(QueueNode(x, cloned));
            }
            if (!head) {
                head = cloned;
            }
            if (toClone.parent) {
                toClone.parent->neighbours.push_back(cloned);
            }
        } else {
            toClone.parent->neighbours.push_back(cloneMapping.at(toClone.node));
        }
    }
    return head;
}
