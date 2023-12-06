//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"
#include "parser.h"

#include <iostream>
#include <map>

namespace {
}


int main() {
    aoc::InputReader input{};
    long total_uno{0};
    const auto &input_lines = input.getLines();
    auto is_gear_symbol = [](char c) {
        return c == '*';
    };
    std::map<const char *, std::vector<long>> gear_candidates{};
    for (long i = 0; i < input_lines.size(); ++i) {
        const auto &prev_line = input_lines[std::max(0, i - 1)];
        const auto &current_line = input_lines[i];
        const auto &next_line = input_lines[std::min(i + 1, static_cast<long>(input_lines.size() - 1))];
        auto number_start = std::find_if(current_line.begin(), current_line.end(),
                                         [](char c) { return std::isdigit(c); });
        while (number_start != current_line.end()) {
            auto number_end = std::find_if(number_start, current_line.end(), [](char c) {return not std::isdigit((c));});

            auto min_index = std::max(std::distance(current_line.begin(), number_start) - 1, 0L);
            auto max_index = std::min(std::distance(current_line.begin(), number_end),
                                  static_cast<long>(current_line.size()) - 1);
            auto prev_line_symbol = std::find_if(prev_line.begin() + min_index, prev_line.begin() + max_index + 1, is_gear_symbol);
            if(&prev_line != &current_line && prev_line.begin() + max_index + 1 != prev_line_symbol ) {
                gear_candidates[
                        prev_line_symbol.base()].push_back(aoc::getLong({number_start, number_end}));
            }
            auto next_line_symbol = std::find_if(next_line.begin() + min_index, next_line.begin() + max_index + 1, is_gear_symbol);
            if(&next_line != &current_line &&
               next_line.begin() + max_index + 1 != next_line_symbol) {
                gear_candidates[
                        next_line_symbol.base()].push_back(aoc::getLong({number_start, number_end}));

            }
            if ( is_gear_symbol(current_line[min_index])) {
                gear_candidates[
                        current_line.c_str()+min_index].push_back(aoc::getLong({number_start, number_end}));

            }
            if ( is_gear_symbol(current_line[max_index])) {
                gear_candidates[
                        current_line.c_str()+max_index].push_back(aoc::getLong({number_start, number_end}));

            }

            number_start = std::find_if(number_end, current_line.end(), [](char c) { return std::isdigit(c); });
        }
    }

    long total{0};
    for(const auto &candidate : gear_candidates) {
        if(candidate.second.size() == 2) {
            total += candidate.second[0] * candidate.second[1];
        }
    }
    std::cout << "Values:" << total << std::endl;
}