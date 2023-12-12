//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include <numeric>
#include "input_reader.h"
#include "parser.h"
#include <regex>
#include <complex>

namespace {
    std::vector<long> readContigues(const std::string &candidate){
        bool is_contigues = false;
        long length{0};
        std::vector<long> groups;
        for(auto c : candidate){
            if(c=='#'){
                length+=1;
                is_contigues=true;
            } else {
                if(is_contigues){
                    groups.push_back(length);
                    length = 0;
                    is_contigues = false;
                }
            }
        }
        if(is_contigues){
            groups.push_back(length);
        }
        return groups;
    }

    bool good(const std::string &candidate, const std::vector<long> cont){
         auto read_continuges = readContigues(candidate);
         return cont == read_continuges;
    }
}


int main() {
    aoc::InputReader inputReader{};
    const auto& lines = inputReader.getLines();
    long combinations{0};
    for(const auto &line :lines){
        auto sep = line.find(' ');
        std::string patter = line.substr(0, line.find(' '));
        std::string pattern{patter};
        std::string numbers = line.substr(sep+1, std::string::npos);
        std::replace(numbers.begin(), numbers.end(), ',', ' ');
        std::vector<int> positions;
        auto contingues = aoc::getlongs(numbers);
        std::vector<long> new_contigues{contingues};
        for(int i = 0; i < 4; ++i){
            pattern+="?"+patter;
            std::copy(contingues.begin(), contingues.end(), std::back_inserter(new_contigues));
        }

        for(int i = 0; i < pattern.size(); ++i){
            if(pattern[i]=='?'){
                positions.push_back(i);
            }
        }
        for(int i = 0; i < 1 << positions.size(); ++i){
            for(int j = 0; j < positions.size(); ++j){
                pattern[positions[j]] = (i & (1 << j)) != 0 == 1 ? '.' : '#';
            }
            if(good(pattern, contingues)){
                std::cout<<pattern<<std::endl;
                combinations+=1;
            }
        }
    }
    std::cout<<combinations<<std::endl;
    return 0;
}