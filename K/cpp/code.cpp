#include <vector>
#include <iostream>

/*
Условие задачи.
Wildcard pattern matching.
В этой задаче вам надо определить, подходит ли строка S под конкретный шаблон P. 
Шаблон задаётся в следующем формате: 
    Символ «?» соответствует одному вхождению любого символа; 
    Символ «*» соответствует произвольному числу любых символов, в том числе нулю символов; 
    Остальные символы шаблона должны совпадать с символами строки;

Объяснение: https://bit.ly/3AMuxdw
Решение задачи построено на двумерной динамике. DP[i][j] означает, совпадает ли 
часть текста от 0 до i вкл. с шаблоном от 0 до j вкл.
В таком случае ответ на задачу будет лежать в DP[n - 1][m - 1], где
n -- длина текста, m -- длина шаблона (сверху вниз расположен текст, слева на право -- шаблон).
Есть несколько случаев при заполнении DP[i][j]:
    1) S[i] = P[j], где P[j] -- определенный символ. Тогда если есть совпадение символов, то нужно посмотреть
    на совпадение части текста позади S[i]. А это значит, что нужно проверить совпадение части текста
    от 0 до i-1 и шаблона от 0 до j-1, а это в точности есть значение DP[i - 1][j - 1].
    Если же нет совпадения символов, то ставим false.
    2) P[j] = '?'. Тогда нужно посмотреть аналогично пункту 1 совпадение части текста
    от 0 до i-1 и шаблона от 0 до j-1, поскольку неважно, какой символ S[i] сейчас, но важно, было
    ли совпадение ранее.
    3) P[j] = '*'. Можно рассматривать несколько случаев:
        а) будем считать, что '*' означает 0 символов. Тогда нужно посмотреть, совпадает ли часть текста
        от 0 до i и шаблона от 0 до j-1 (то есть мы текст оставляем на месте, а в шаблоне смещаемся на один
        символ назад, как будто этой '*' просто не было). Это в точности значение DP[j][i-1].
        б) будем считать, что текущий символ строки S[i] попадает под '*'. Тогда нужно посмотреть, совпадает
        ли часть текста от 0 до i - 1 с шаблоном от 0 до j. Это в точности значение DP[i-1][j]. Почему так?
        Дело в том, что в шаблоне нам нужно оставить '*', поскольку предыдущие символы (S[i-1], ...) в теории
        могут тоже "попадать" в '*'. Убирать же '*' нельзя (то есть нельзя переходить к DP[...][j-1]), поскольку
        случай, когда '*' ничего как бы не значал, уже был рассмотрен в пункте 3.а

Начальная инициализация требует заполнения 0-й строки и 0-го столбца. По строкам -- текст, по столбцам -- шаблон.
    1) 0-я строка и j-й столбец означает, совпадает ли пустой текст с шаблоном от 0 до j.
        а) пустой текст и пустой шаблон совпадают (DP[0][0] = true)
        б) пустой текст и символ или пустой текст и '?' не совпадают, DP[0][j] = false.
        в) пустой текст и '*' могут совпасть, могут нет, зависит от того, было ли до '*' в шаблоне совпадение,
        поэтому в таком случае DP[0][j] = DP[0][j-1].
    2) i-я строка и 0-й столбец означает, совпадает ли текст от 0 до i с пустым шаблоном. Тут везде false кроме случая в пунтке 1.а


*/


bool stringMatchesTemplate(const std::string& stringToCheck, 
                                const std::string& templateString) {
    std::vector<std::vector<bool>> dp(stringToCheck.size() + 1, 
                                        std::vector<bool>(templateString.size() + 1));
    dp[0][0] = true;
    for (int j = 1; j <= templateString.size(); ++j) {
        dp[0][j] = (templateString[j - 1] == '*') ? dp[0][j - 1] : false;
    }
    for (int i = 1; i <= stringToCheck.size(); ++i) {
        dp[i][0] = false;
    }
    for (int i = 1; i <= stringToCheck.size(); ++i) {
        for (int j = 1; j <= templateString.size(); ++j) {
            if (templateString[j - 1] == '*') {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            } else if (templateString[j - 1] == '?') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (stringToCheck[i - 1] == templateString[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = false;
            }
        }
    }
    return dp[stringToCheck.size()][templateString.size()];
}

int main() {
    std::string templateString;
    std::cin >> templateString;
    std::string stringToCheck;
    std::cin >> stringToCheck;

    if (stringMatchesTemplate(stringToCheck, templateString)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
}




