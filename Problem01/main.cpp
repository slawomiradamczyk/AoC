//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"

#include <algorithm>
#include <iostream>
#include <functional>
#include <optional>

namespace {
    const std::vector<std::string> kAllowedDigitNames{
            "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    auto find_first_spelled(const std::string_view &view) {
        std::optional<int> ret_val;
        auto last_found = view.end();
        for(int i = 0; i < kAllowedDigitNames.size(); ++i) {
            auto found_pos = view.find(kAllowedDigitNames.at(i));
            if(found_pos != std::string_view::npos){
                auto new_found = std::next(view.begin(), found_pos);
                if(new_found < last_found) {
                    ret_val = i+1;
                    last_found = new_found;
                }
            }
        }
        return ret_val;
    }

    auto find_last_spelled(const std::string_view &view) {
        std::optional<int> ret_val;
        auto last_found = view.begin() - 1;
        for(int i = 0; i < kAllowedDigitNames.size(); ++i) {
            auto found_pos = view.rfind(kAllowedDigitNames.at(i));
            if(found_pos != std::string_view::npos){
                auto new_found = std::next(view.begin(), found_pos);
                if(new_found > last_found) {
                    ret_val = i+1;
                    last_found = new_found;
                }
            }
        }
        return ret_val;
    }
}


int main(){
    aoc::InputReader input{};
    int total{0};
    for(const auto &line : input.getLines()) {
        auto first_digit_char = std::find_if(line.begin(), line.end(), [](char c){return std::isdigit(c);});
        auto last_digit_char = std::find_if(line.rbegin(), line.rend(), [](char c){return std::isdigit(c);});

        auto first_digit_spelled = find_first_spelled({line.begin(), first_digit_char});
        auto last_digit_spelled = find_last_spelled({last_digit_char.base(), line.end()});

        auto first_digit = first_digit_spelled ? *first_digit_spelled : *first_digit_char - '0';
        auto last_digit = last_digit_spelled ? *last_digit_spelled : *last_digit_char - '0';

        int calibration_value =(first_digit) * 10 + last_digit;
        std::cout<<line<<" "<<calibration_value<<std::endl;
        total+=calibration_value;
    }
    std::cout<<total<<std::endl;
}