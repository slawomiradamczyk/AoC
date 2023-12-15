//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include <numeric>
#include "input_reader.h"
#include "parser.h"
#include <regex>
#include <complex>
#include <optional>


namespace {

     int ReverseEqualDiff(std::string_view lhs, std::string_view rhs){
         auto count{0};
            for(int i = 0; i < lhs.size(); ++i){
                if(lhs[i] != rhs[lhs.size()-1-i]){
                    ++count;
                }
            }
            return count;
    }


    int FindVerticalMirror(const std::vector<std::string_view> &lines){
        auto len = lines[0].size();
        for(unsigned long mirror_pos = 1; mirror_pos < len; ++mirror_pos){
            bool is_mirror = true;
            bool found_one = false;
            for(const auto &line : lines){
                auto length = std::min(mirror_pos, len-mirror_pos);
                auto diff = ReverseEqualDiff(line.substr(mirror_pos - length, length),
                                            line.substr(mirror_pos, length));
                if(diff == 1 && not found_one){
                    found_one = true;
                } else if(diff != 0 ){
                    is_mirror = false;
                    break;
                }
            }
            if(is_mirror && found_one){
                return mirror_pos;
            }
        }
        return 0;
    }

    /*
     * 1. Find vertical mirror
     */
    int FindHorizontalMirror(const std::vector<std::string_view> &lines){
        for(unsigned long mirror_pos = 1; mirror_pos < lines.size(); ++mirror_pos){
            bool is_mirror = true;
            bool found_one = false;
            for(int i = 0; i < lines[0].size(); ++i){
                auto length = std::min(mirror_pos, lines.size()-mirror_pos);
                for(auto j = 0; j < length; ++j){
                    if(lines[mirror_pos-1-j][i] != lines[mirror_pos+j][i]){
                        if(found_one){
                            is_mirror = false;
                            break;
                        } else {
                            found_one = true;
                        }
                    }
                }
                if(not is_mirror){
                    break;
                }
            }
            if(is_mirror && found_one){
                return mirror_pos;
            }
        }
        return 0;
    }

}


int main() {
    aoc::InputReader inputReader{};
    const auto& lines = inputReader.getLines();
    std::vector<std::string_view> vec;
    long mirror_pos = 0;
    for(const auto &line : lines){
        if(line.empty()){
            mirror_pos += FindVerticalMirror(vec);
            mirror_pos += 100*FindHorizontalMirror(vec);
            vec.clear();
        } else{
            vec.push_back(line);
        }
    }
    mirror_pos += FindVerticalMirror(vec);
    mirror_pos += 100*FindHorizontalMirror(vec);
    std::cout<<mirror_pos<<std::endl;
    return 0;
}

