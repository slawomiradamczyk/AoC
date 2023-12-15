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
#include <queue>
#include <list>


namespace {
        long CalculateHash(std::string_view s){
            long hash = 0;
            for(auto c : s){
                hash += c;
                hash *= 17;
                hash %= 256;
            }
            return hash;
        }

        class Box{
        public:
            Box() = default;
            void Add(std::string_view elem, long lens_value){
                auto it = elements.find(elem);
                if(it != elements.end()){
                   *(it->second) = lens_value;
                } else {
                    auto it = lens_values.emplace(lens_values.end(), lens_value);
                    elements.emplace(elem, it);
                }
            }
            void Remove(std::string_view elem){
                if(elements.find(elem) != elements.end()){
                    auto it = elements[elem];
                    lens_values.erase(it);
                    elements.erase(elem);
                }
            }
            long CalculateValue(int box_number){
                int position{1};
                long total{0};
                for(auto lens_value : lens_values){
                    total+=lens_value*position*box_number;
                    ++position;
                }
                return total;
            }
        private:
            std::list<long> lens_values;
            std::map<std::string_view, decltype(lens_values.begin())> elements;
        };
    }



    int main() {
        aoc::InputReader inputReader{};
        const auto &lines = inputReader.getLines();
        auto splitted_line = aoc::split(lines[0], ',');
        long total = 0;
        std::vector<Box> boxes{256};
        for(auto split : splitted_line){
            auto operation = split.find_first_of("=-");
            auto label = split.substr(0, operation);
            auto hash = CalculateHash(label);
            if(split[operation] == '='){
                auto value = split.substr(operation+1);
                boxes[hash].Add(label, aoc::getLong(value));
            } else {
                boxes[hash].Remove(label);
            }
        }

        for(int i = 0; i < boxes.size(); ++i){
            total+= boxes[i].CalculateValue(i+1);
        }
        std::cout << total << std::endl;
        return 0;
    }

