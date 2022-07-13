#include <iostream>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

/*
В этой задаче вам предстоит ответить на вопрос, является данная на вход строка корректным IPv4 или IPv6-адресом. 
Решите задачу без использования регулярных выражений. 

Валидный IPv4-адрес имеет следующий формат: s 1 . s 2 . s 3 . s 4 , 
где s i — целое число от 0 до 2 5 5 . 
Числа s i не должны иметь лидирующих нулей. 

Валидный IPv6-адрес имеет следующий формат: s 1 : s 2 : s 3 : s 4 : s 5 : s 6 : s 7 : s 8 , 
где s i - представление числа в 16-ричной системе, состоящее не более чем из четырёх символов. 
В записи каждого числа могут присутствовать лидирующие нули. 
Разрешено использовать символы 0-9, a-f, A-F. s i не может быть пустой последовательностью символов.


*/

const string IPV4 = "IPv4";
const string IPV6 = "IPv6";
const string ERROR = "Error";

bool isIPv4NumberCorrect(const std::string &optionalNumber) {
    int32_t multiplier = 1;
    int32_t number = 0;
    for (auto it = optionalNumber.rbegin(); it != optionalNumber.rend(); ++it) {
        number += ((*it) - '0') * multiplier;
        multiplier *= 10;
    }
    if (0 <= number && number <= 9 && optionalNumber.size() > 1 ||
        10 <= number && number <= 99 && optionalNumber.size() > 2 ||
        number > 255 || optionalNumber.empty()) {
        return false;
    }
    return true;
}

std::unordered_set<char> getIPv4AvailableChars() {
    std::unordered_set<char> available;
    for (auto c = '0'; c <= '9'; ++c) {
        available.insert(c);
    }
    return available;
}

std::unordered_set<char> getIPv6AvailableChars() {
    std::unordered_set<char> available;
    for (auto c = '0'; c <= '9'; ++c) {
        available.insert(c);
    }
    for (auto c = 'a'; c <= 'f'; ++c) {
        available.insert(c);
    }
    for (auto c = 'A'; c <= 'F'; ++c) {
        available.insert(c);
    }
    return available;
}

bool isSubstrContainsBanned(const std::string &s, const std::unordered_set<char> &available) {
    for (const auto c: s) {
        if (available.find(c) == available.end()) {
            return true;
        }
    }
    return false;
}

/**
 * return IPV4,IPV6, or ERROR
 */
std::string checkIpAddress(const std::string &ip) {
    // Your code here
    int64_t dotCounter = std::count(ip.begin(), ip.end(), '.');
    int64_t colonCounter = std::count(ip.begin(), ip.end(), ':');
    if (dotCounter == 3 && colonCounter == 0) {
        std::unordered_set<char> available = getIPv4AvailableChars();
        std::string::size_type pos = 0;
        auto n = ip.find('.', pos);
        while (n != std::string::npos) {
            std::string substr = ip.substr(pos, n - pos);
            if (isSubstrContainsBanned(substr, available)) {
                return ERROR;
            }
            if (!isIPv4NumberCorrect(substr)) {
                return ERROR;
            }
            pos = n + 1;
            n = ip.find('.', pos);
            if (ip.size() - pos > 0 && n == std::string::npos) {
                substr = ip.substr(pos, ip.size() - pos);
                if (isSubstrContainsBanned(substr, available)) {
                    return ERROR;
                }
                if (!isIPv4NumberCorrect(substr)) {
                    return ERROR;
                }
            }
            if (ip.size() == pos && ip.at(pos - 1) == '.') {
                return ERROR;
            }
        }
        return IPV4;
    } else if (dotCounter == 0 && colonCounter == 7) {
        std::unordered_set<char> available = getIPv6AvailableChars();
        std::string::size_type pos = 0;
        auto n = ip.find(':', pos);
        while (n != std::string::npos) {
            std::string substr = ip.substr(pos, n - pos);
            if (isSubstrContainsBanned(substr, available)) {
                return ERROR;
            }
            if (substr.empty() || substr.size() > 4) {
                return ERROR;
            }
            pos = n + 1;
            n = ip.find(':', pos);
            if (ip.size() - pos > 0 && n == std::string::npos) {
                substr = ip.substr(pos, ip.size() - pos);
                if (isSubstrContainsBanned(substr, available)) {
                    return ERROR;
                }
                if (substr.empty() || substr.size() > 4) {
                    return ERROR;
                }
            }
            if (ip.size() == pos && ip.at(pos - 1) == ':') {
                return ERROR;
            }
        }
        return IPV6;
    }
    return ERROR;
}

int main() {
    string ipAddress;
    std::cin >> ipAddress;
    std::cout << checkIpAddress(ipAddress);
    return 0;
}