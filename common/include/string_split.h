//
// Created by Slawomir Adamczyk on 04.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//

#ifndef AOC_STRING_SPLIT_H
#define AOC_STRING_SPLIT_H
#include <string_view>
namespace aoc{
    std::vector<std::string_view>  split(const std::string_view& str, char delimiter){
        std::vector<std::string_view> ret_val;
        std::cout << "splitting string: " << str << std::endl;
        size_t start_pos{0};
        while(start_pos != std::string_view::npos) {
            auto end_pos = str.find(delimiter, start_pos);
            std::string_view token;
            if(end_pos != std::string_view::npos) {
                token = {str.begin() + start_pos, str.begin() + end_pos};
                start_pos = end_pos+1;
            } else {
                token = {str.begin() + start_pos, str.end()};
                start_pos = end_pos;
            }
            std::cout << "token: "<<token<<std::endl;
            ret_val.push_back(token);
        }
        return ret_val;
    }
} //namespace aoc
#endif //AOC_STRING_SPLIT_H
