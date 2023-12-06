//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"
#include "parser.h"

#include <iostream>
#include <sstream>
#include <charconv>
#include "string_split.h"

namespace {

    struct Draw {
        long red{0};
        long green{0};
        long blue{0};

        friend std::ostream &operator<<(std::ostream &os, const Draw &draw);

        [[nodiscard]] bool isDrawPossible() const;

        void add(std::string_view str);

    };

    struct MinNeeded : Draw {
        void operator +=(const Draw &rhs) {
            if(rhs.red > red){
                red = rhs.red;
            }
            if(rhs.green > green){
                green = rhs.green;
            }
            if(rhs.blue > blue){
                blue = rhs.blue;
            }
        }

        long power() const {
            return red*green*blue;
        }
    };

    constexpr Draw kMax{12, 13, 14};

    bool Draw::isDrawPossible() const {
        return red <= kMax.red && blue <= kMax.blue && green <= kMax.green;
    }

    void Draw::add(std::string_view str) {
        long value = aoc::getLong(str);
        if (str.find("red") != std::string::npos) {
            red += value;
        } else if (str.find("blue") != std::string::npos) {
            blue += value;
        } else if (str.find("green") != std::string::npos) {
            green += value;
        }
    }

    std::ostream &operator<<(std::ostream &os, const Draw &draw) {
        os << "red: " << draw.red << " green: " << draw.green << " blue: " << draw.blue;
        return os;
    }


    auto parseSingleDraw(std::string_view draw_str) {
        Draw draw{};
        auto value_color_strings = aoc::split(draw_str, ',');
        for(const auto &value_color_str : value_color_strings) {
            draw.add(value_color_str);
        }
        std::cout << "Draw str: " << draw_str << std::endl;
        std::cout << "Draw parsed: " << draw << std::endl;
        return draw;
    }

    auto parseDraws(const std::string &line) {
        std::vector<Draw> ret_val;
        auto search_start = line.find(':');
        std::string_view draws_str{std::next(line.begin(), search_start + 1), line.end()};
        auto draws = aoc::split(draws_str, ';');
        for(const auto &single_draw : draws){
            auto draw = parseSingleDraw(single_draw);
            ret_val.push_back(draw);
        }
        return ret_val;
    }

}

int main() {
    aoc::InputReader input{};
    long total{0};
    for (const auto &line: input.getLines()) {
        MinNeeded min_needed{};
        auto hands = parseDraws(line);
        for (const auto &hand: hands) {
            min_needed+=hand;
        }
        total += min_needed.power();
    }
    std::cout << total << std::endl;
}