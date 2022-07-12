#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;


struct HistoricalArray {
    static constexpr int DEFAULT_INITIAL_VALUE = 0;

    __attribute__((unused)) int size;

    int currentEra;

    std::vector<std::vector<int32_t>> historicalValues;

    std::vector<std::unordered_map<size_t, size_t>> eraIndexes;

    explicit HistoricalArray(int n) : size(n) {
        historicalValues.resize(n, {});
        eraIndexes.resize(n, {});
        currentEra = 0;
    }

    void beginNewEra(int eraId) {
        size_t currentValue = 0;
        for (auto &indexes: eraIndexes) {
            if (indexes.find(currentEra) == indexes.end()) {
                if (!historicalValues[currentValue].empty()) {
                    indexes[currentEra] = historicalValues[currentValue].size() - 1;
                }
            }
            currentValue++;
        }
        currentEra = eraId;
    }

    void set(int index, int value) {
        if (eraIndexes[index].find(currentEra) != eraIndexes[index].end()) {
            historicalValues[index][eraIndexes[index][currentEra]] = value;
        } else {
            auto valueFrequency = historicalValues[index].size();
            eraIndexes[index][currentEra] = valueFrequency;
            historicalValues[index].push_back(value);
        }
    }

    int get(int index, int eraId) {
        if (eraIndexes[index].find(eraId) == eraIndexes[index].end()) {
            return DEFAULT_INITIAL_VALUE;
        }
        return historicalValues[index][eraIndexes[index][eraId]];
    }
};


int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    HistoricalArray arr(n);
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        string queryType;
        cin >> queryType;
        if (queryType == "set") {
            int index, value;
            cin >> index >> value;
            arr.set(index, value);
        } else if (queryType == "begin_new_era") {
            int eraId;
            cin >> eraId;
            arr.beginNewEra(eraId);
        } else if (queryType == "get") {
            int index, eraId;
            cin >> index >> eraId;
            cout << arr.get(index, eraId) << endl;
        }
    }
    return 0;
}
