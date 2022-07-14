#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/*
Explanation (eng): https://www.youtube.com/watch?v=bAeJ04b84QM&ab_channel=CodingDecoded

*/

struct TrieNode {
    struct TrieNode *zeroLeft;
    struct TrieNode *oneRight;

    TrieNode() :
            zeroLeft(nullptr), oneRight(nullptr) {
    }
};

int64_t getMaxXOR(const vector<int> &list) {
    auto listMaxNumber = *std::max_element(list.begin(), list.end());
    auto bitsNumber = static_cast<int32_t>(std::log2(listMaxNumber)) + 1;
    TrieNode root;
    std::vector<TrieNode> trieElements(list.size() * bitsNumber);
    auto currentNumber = 0;
    for (const auto x: list) {
        TrieNode *it = &root;
        for (int32_t shift = bitsNumber - 1; shift >= 0; --shift) {
            auto isBitExist = 1 & (x >> shift);
            if (isBitExist) {
                if (!it->oneRight) {
                    it->oneRight = &(trieElements[currentNumber]);
                    currentNumber++;
                }
                it = it->oneRight;
            } else {
                if (!it->zeroLeft) {
                    it->zeroLeft = &(trieElements[currentNumber]);
                    currentNumber++;
                }
                it = it->zeroLeft;
            }
        }
    }
    int64_t globalMax = -1;
    for (const auto x: list) {
        int64_t currentMax = 0;
        TrieNode *it = &root;
        for (int32_t shift = bitsNumber - 1; shift >= 0; --shift) {
            TrieNode *next;
            auto isBitExist = 1 & (x >> shift);
            if (isBitExist) {
                if (it->zeroLeft) {
                    next = it->zeroLeft;
                    currentMax += (1 << shift);
                } else {
                    next = it->oneRight;
                }
            } else {
                if (it->oneRight) {
                    next = it->oneRight;
                    currentMax += (1 << shift);
                } else {
                    next = it->zeroLeft;
                }
            }
            it = next;
        }
        globalMax = std::max(globalMax, currentMax);
    }
    return globalMax;
}

vector<int> readList() {
    int n;
    cin >> n;
    vector<int> res(n);
    for (int i = 0; i < n; i++) {
        cin >> res[i];
    }
    return res;
}

int main() {
    vector<int> list = readList();
    cout << getMaxXOR(list);
    return 0;
}

