#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
Объяснение идеи (23 тест TL, асимтотика O(N^2), нужна н лог н), см. по ссылке
https://vk.com/im?act=browse_images&id=1325903
Если ссылка не рабочая, то ищи две фотографии с листочками-стикерами в Favorites.
*/

struct Building {
    int needCapital;
    int addedCapital;

    Buildinаg(int c, int p) {
        needCapital = c;
        addedCapital = p;
    }

    friend bool operator<(const Building &l, const Building &r) {
        if (l.needCapital == r.needCapital) {
            return l.addedCapital < r.addedCapital;
        }
        return l.needCapital < r.needCapital;
    }
};


long long getMaxFinalCapital(const vector<Building> &buildings, int startCapital, int maxNumberOfBuildings) {
    std::vector<int64_t> previousRow(maxNumberOfBuildings + 1, startCapital);
    std::vector<int64_t> currentRow(maxNumberOfBuildings + 1);
    std::vector<Building> sortedBuildings = buildings;
    std::sort(sortedBuildings.begin(), sortedBuildings.end());
    for (int32_t availableFirst = 1; availableFirst <= sortedBuildings.size(); ++availableFirst) {
        currentRow[0] = startCapital;
        for (int32_t takenBuildings = 1; takenBuildings <= maxNumberOfBuildings; ++takenBuildings) {
            if (takenBuildings > availableFirst) {
                break;
            }
            int64_t optionalSum = -1;
            if (previousRow[takenBuildings - 1] >= sortedBuildings[availableFirst - 1].needCapital) {
                optionalSum = sortedBuildings[availableFirst - 1].addedCapital + previousRow[takenBuildings - 1];
            }
            int64_t optionalValue = -1;
            if (takenBuildings < availableFirst) {
                optionalValue = previousRow[takenBuildings];
            }
            currentRow[takenBuildings] = std::max(std::max(optionalValue,
                                                           currentRow[takenBuildings - 1]),
                                                  optionalSum);
        }
        previousRow = currentRow;
    }
    return currentRow[maxNumberOfBuildings];
}


vector<Building> readBuildings(int n) {
    vector<Building> buildings;
    for (int i = 0; i < n; i++) {
        int c, p;
        cin >> c >> p;
        buildings.push_back(Building(c, p));
    }
    return buildings;
}

int main() {
    int n;
    cin >> n;
    int k;
    cin >> k;
    vector<Building> buildings = readBuildings(n);
    int M;
    cin >> M;
    cout << getMaxFinalCapital(buildings, M, k) << endl;
    return 0;
}




