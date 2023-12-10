//
// Created by adamc on 03.12.2023.
//
#include <algorithm>
#include <charconv>
#include <iostream>
#include "string_split.h"

#ifndef AOC_PARSER_H
#define AOC_PARSER_H
namespace aoc {
    auto getLong(const std::string_view &str) {
        long value{-1};
        auto number_pos = std::find_if(str.begin(), str.end(), [](char c) {
            return std::isdigit(c) || c == '-';
        });
        auto ecc = std::from_chars(number_pos, str.data() + str.size(), value);
        if (ecc.ec != std::errc{}) {
            throw std::range_error{"Bad long"};
        }
        return value;
    }

    std::vector<long> getlongs(const std::string_view &view) {
        std::vector<long> ret_val;
        auto long_str = aoc::split(view, ' ');
        for (const auto &long_s: long_str) {
            try {
                ret_val.push_back(aoc::getLong(long_s));
            } catch (const std::exception &e) {
                std::cout << "Failed to convert: " << long_s << std::endl;
            }
        }
        return ret_val;
    }
}
#endif //AOC_PARSER_H
