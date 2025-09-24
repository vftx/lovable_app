#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "parser.hpp"

namespace {

void expectEqual(double expected, const std::string &expression) {
    Parser parser(expression);
    double result = parser.parse();
    assert(std::fabs(result - expected) < 1e-9 && "Unexpected calculation result");
}

void expectThrows(const std::string &expression, const std::string &message) {
    try {
        Parser parser(expression);
        (void)parser.parse();
        assert(false && "Expected exception was not thrown");
    } catch (const std::runtime_error &ex) {
        assert(message == ex.what() && "Unexpected exception message");
    }
}

void testAddition() {
    expectEqual(7.0, "3 + 4");
}

void testSubtractionWithWhitespace() {
    expectEqual(2.0, " 5 - 3 ");
}

void testMultiplicationPrecedence() {
    expectEqual(14.0, "2 + 3 * 4");
}

void testParentheses() {
    expectEqual(20.0, "(2 + 3) * 4");
}

void testNestedParenthesesAndUnary() {
    expectEqual(-5.0, "-(2 + 3)");
    expectEqual(6.0, "+(2 * 3)");
    expectEqual(-1.0, "-(-(-1))");
}

void testDecimalNumbers() {
    expectEqual(4.0, "3.5 + 0.5");
}

void testTrailingCharacters() {
    expectThrows("2 + 3a", "Nieoczekiwane znaki w wyrażeniu.");
}

void testMissingClosingParenthesis() {
    expectThrows("(2 + 3", "Brakuje zamykającego nawiasu.");
}

void testInvalidNumberFormat() {
    expectThrows("2..3", "Niepoprawny format liczby.");
}

void testMissingNumber() {
    expectThrows("2 + * 3", "Oczekiwano liczby.");
}

void testDivisionByZero() {
    expectThrows("10 / 0", "Dzielenie przez zero jest niedozwolone.");
}

void testUnexpectedEnd() {
    expectThrows("5 +", "Nieoczekiwany koniec wyrażenia.");
}

void testWhitespaceOnlyUnary() {
    expectEqual(-3.0, " - 3");
}

void runAllTests() {
    testAddition();
    testSubtractionWithWhitespace();
    testMultiplicationPrecedence();
    testParentheses();
    testNestedParenthesesAndUnary();
    testDecimalNumbers();
    testTrailingCharacters();
    testMissingClosingParenthesis();
    testInvalidNumberFormat();
    testMissingNumber();
    testDivisionByZero();
    testUnexpectedEnd();
    testWhitespaceOnlyUnary();
}

} // namespace

int main() {
    runAllTests();
    std::cout << "Wszystkie testy zakończone sukcesem." << std::endl;
    return 0;
}
