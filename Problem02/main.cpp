//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"

#include <iostream>
#include <sstream>
#include <charconv>

namespace {
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

    struct Draw {
        int red{0};
        int green{0};
        int blue{0};

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

        int power() const {
            return red*green*blue;
        }
    };

    constexpr Draw kMax{12, 13, 14};

    bool Draw::isDrawPossible() const {
        return red <= kMax.red && blue <= kMax.blue && green <= kMax.green;
    }

    void Draw::add(std::string_view str) {
        int value = getInt(str);
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
        size_t search_start = 0;
        Draw draw{};
        while (search_start != std::string_view::npos) {
            auto search_end = draw_str.find(',', search_start + 1);
            std::string_view value_color_str;
            if (search_end != std::string::npos) {
                value_color_str = {draw_str.data() + search_start + 1, draw_str.data() + search_end};
            } else {
                value_color_str = {draw_str.data() + search_start + 1, draw_str.end()};
            }
            draw.add(value_color_str);
            search_start = search_end;
        }
        std::cout << "Draw str: " << draw_str << std::endl;
        std::cout << "Draw parsed: " << draw << std::endl;
        return draw;
    }

    auto parseDraws(const std::string &line) {
        std::vector<Draw> ret_val;
        auto search_start = line.find(':');
        while (search_start != std::string::npos) {
            auto search_end_pos = line.find(';', search_start + 1);
            std::string_view single_draw;
            if (search_end_pos != std::string::npos) {
                single_draw = {line.data() + search_start + 1, line.data() + search_end_pos};
            } else {
                single_draw = {line.data() + search_start + 1};
            }
            auto draw = parseSingleDraw(single_draw);
            ret_val.push_back(draw);
            search_start = search_end_pos;
        };
        return ret_val;
    }


    auto getGameNumber(const std::string &line) {
        return getInt(line);
    }
}



int main() {
    aoc::InputReader input{};
    int total{0};
    for (const auto &line: input.getLines()) {
        int game_number = getGameNumber(line);
        MinNeeded min_needed{};
        std::cout << "Game number " << game_number << std::endl;
        auto hands = parseDraws(line);
        for (const auto &hand: hands) {
            min_needed+=hand;
        }
        total += min_needed.power();
    }
    std::cout << total << std::endl;
}