#ifndef CALCULATOR_PARSER_HPP
#define CALCULATOR_PARSER_HPP

#include <cctype>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

class Parser {
public:
    explicit Parser(const std::string &expression) : expr(expression), pos(0) {}

    double parse() {
        double value = parseExpression();
        skipSpaces();
        if (pos != expr.size()) {
            throw std::runtime_error("Nieoczekiwane znaki w wyrażeniu.");
        }
        return value;
    }

private:
    const std::string expr;
    std::size_t pos;

    void skipSpaces() {
        while (pos < expr.size() && std::isspace(static_cast<unsigned char>(expr[pos]))) {
            ++pos;
        }
    }

    double parseExpression() {
        double value = parseTerm();
        while (true) {
            skipSpaces();
            if (pos >= expr.size()) {
                break;
            }
            char op = expr[pos];
            if (op == '+' || op == '-') {
                ++pos;
                double rhs = parseTerm();
                if (op == '+') {
                    value += rhs;
                } else {
                    value -= rhs;
                }
            } else {
                break;
            }
        }
        return value;
    }

    double parseTerm() {
        double value = parseFactor();
        while (true) {
            skipSpaces();
            if (pos >= expr.size()) {
                break;
            }
            char op = expr[pos];
            if (op == '*' || op == '/') {
                ++pos;
                double rhs = parseFactor();
                if (op == '*') {
                    value *= rhs;
                } else {
                    if (std::abs(rhs) < std::numeric_limits<double>::epsilon()) {
                        throw std::runtime_error("Dzielenie przez zero jest niedozwolone.");
                    }
                    value /= rhs;
                }
            } else {
                break;
            }
        }
        return value;
    }

    double parseFactor() {
        skipSpaces();
        if (pos >= expr.size()) {
            throw std::runtime_error("Nieoczekiwany koniec wyrażenia.");
        }

        char current = expr[pos];
        if (current == '+') {
            ++pos;
            return parseFactor();
        }
        if (current == '-') {
            ++pos;
            return -parseFactor();
        }
        if (current == '(') {
            ++pos;
            double value = parseExpression();
            skipSpaces();
            if (pos >= expr.size() || expr[pos] != ')') {
                throw std::runtime_error("Brakuje zamykającego nawiasu.");
            }
            ++pos;
            return value;
        }
        return parseNumber();
    }

    double parseNumber() {
        skipSpaces();
        std::size_t start = pos;
        bool hasDigit = false;
        bool hasDot = false;

        while (pos < expr.size()) {
            char c = expr[pos];
            if (std::isdigit(static_cast<unsigned char>(c))) {
                hasDigit = true;
                ++pos;
            } else if (c == '.') {
                if (hasDot) {
                    throw std::runtime_error("Niepoprawny format liczby.");
                }
                hasDot = true;
                ++pos;
            } else {
                break;
            }
        }

        if (!hasDigit) {
            throw std::runtime_error("Oczekiwano liczby.");
        }

        return std::stod(expr.substr(start, pos - start));
    }
};

#endif // CALCULATOR_PARSER_HPP
