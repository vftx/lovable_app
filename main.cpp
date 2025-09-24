#include <iostream>
#include <string>

#include "parser.hpp"

int main() {
    std::cout << "Prosty kalkulator CLI" << std::endl;
    std::cout << "Podaj całe wyrażenie arytmetyczne (obsługiwane operatory: +, -, *, / oraz nawiasy):" << std::endl;
    std::cout << "> ";

    std::string input;
    if (!std::getline(std::cin, input)) {
        std::cerr << "Nie udało się wczytać wyrażenia." << std::endl;
        return 1;
    }

    try {
        Parser parser(input);
        double result = parser.parse();
        std::cout << "Wynik: " << result << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Błąd: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
