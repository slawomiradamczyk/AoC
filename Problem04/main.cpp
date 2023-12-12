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
    std::set<long> getlongs(const std::string_view &view) {
        std::set<long> ret_val;
        auto long_str = aoc::split(view, ' ');
        for (const auto &long_s: long_str) {
            try {
                ret_val.insert(aoc::getLong(long_s));
            } catch (const std::exception &e) {
                std::cout << "Failed to convert: " << long_s << std::endl;
            }
        }
        return ret_val;
    }
}


int main() {
    aoc::InputReader input{};
    const auto &input_lines = input.getLines();
    std::vector<long> card_count(input_lines.size(), 1);
    long total{0};
    for (long i = 0; i<input_lines.size();++i) {
        const auto & line = input_lines[i];
        auto colon_pos = line.find(':');
        std::string_view values{line.begin() + colon_pos + 1, line.end()};
        auto values_splitted = aoc::split(values, '|');
        auto winning_values = getlongs(values_splitted[0]);
        auto my_values = getlongs(values_splitted[1]);
        std::set<long> diff;
        std::set_intersection(winning_values.begin(), winning_values.end(),
                            my_values.begin(), my_values.end(),
                            std::inserter(diff, diff.begin()));
        if(not diff.empty()) {
            for(long j = 1; j <= diff.size(); ++j){
                card_count[i+j]+=card_count[i];
            }
        }
    }

    std::cout << "Values:" << std::accumulate(card_count.begin(), card_count.end(), 0L) << std::endl;
}