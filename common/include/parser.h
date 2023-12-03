//
// Created by adamc on 03.12.2023.
//
#include <algorithm>
#include <charconv>
#include <iostream>

#ifndef AOC_PARSER_H
#define AOC_PARSER_H
namespace aoc {
    auto getInt(const std::string_view &str) {
        int value{-1};
        auto number_pos = std::find_if(str.begin(), str.end(), [](char c) {
            return std::isdigit(c);
        });
        auto ecc = std::from_chars(number_pos, str.data() + str.size(), value);
        if (ecc.ec != std::errc{}) {
            std::cout << "Parsing failure: " << str << std::endl;
        }
        return value;

    }
}
#endif //AOC_PARSER_H
