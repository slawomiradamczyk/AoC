//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"
#include "parser.h"
#include "string_split.h"

#include <iostream>
#include <map>
#include <set>
#include <cmath>
#include <numeric>

namespace {
    std::set<int> getInts(const std::string_view &view) {
        std::set<int> ret_val;
        auto int_str = aoc::split(view, ' ');
        for (const auto &int_s: int_str) {
            try {
                ret_val.insert(aoc::getInt(int_s));
            } catch (const std::exception &e) {
                std::cout << "Failed to convert: " << int_s << std::endl;
            }
        }
        return ret_val;
    }
}


int main() {
    aoc::InputReader input{};
    const auto &input_lines = input.getLines();
    std::vector<int> card_count(input_lines.size(), 1);
    int total{0};
    for (int i = 0; i<input_lines.size();++i) {
        const auto & line = input_lines[i];
        auto colon_pos = line.find(':');
        std::string_view values{line.begin() + colon_pos + 1, line.end()};
        auto values_splitted = aoc::split(values, '|');
        auto winning_values = getInts(values_splitted[0]);
        auto my_values = getInts(values_splitted[1]);
        std::set<int> diff;
        std::set_intersection(winning_values.begin(), winning_values.end(),
                            my_values.begin(), my_values.end(),
                            std::inserter(diff, diff.begin()));
        if(not diff.empty()) {
            for(int j = 1; j <= diff.size(); ++j){
                card_count[i+j]+=card_count[i];
            }
        }
    }

    std::cout << "Values:" << std::accumulate(card_count.begin(), card_count.end(), 0) << std::endl;
}