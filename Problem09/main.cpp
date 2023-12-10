//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include <numeric>
#include "input_reader.h"
#include "parser.h"

namespace {


    long getPrev(const std::vector<long> &sequence){
        std::vector<long> current_working{sequence};
        std::vector<long> results{*sequence.begin()};
        bool isAllZeros{false};
        while(not isAllZeros){
            isAllZeros = true;
            std::vector<long> differences{};
            differences.reserve(current_working.size()-1);
            for(auto it = current_working.begin()+1; it != current_working.end(); ++it){
                auto diff = *it - *(it-1);
                if(diff != 0){
                    isAllZeros &= false;
                }
                differences.push_back(diff);
            }
            current_working = differences;
            results.push_back(*differences.begin());
        }

        long total_result = 0;
        for(auto it = results.rbegin() + 1; it != results.rend(); ++it) {
            total_result = *it - total_result;
        }

        return total_result;

    }
}


int main() {
    aoc::InputReader inputReader{};
    const auto &lines = inputReader.getLines();
    long total{0};
    int counter = 1;
    for(const auto &line : lines){
        auto sequence = aoc::getlongs(line);
        auto prev = getPrev(sequence);
        std::cout << counter++ << " : " << prev << std::endl;
        total += prev;
    }
    std::cout<<"Total: "<<total<<std::endl;
    return 0;
}