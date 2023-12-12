//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include <numeric>
#include <optional>
#include <stack>
#include "input_reader.h"
#include "parser.h"

namespace {
   struct Coordinates{
       int row;
       int column;
   };

   std::pair<std::vector<int>, std::vector<Coordinates>> ParseRows(const std::vector<std::string> &lines){
       std::vector<int> empty_rows{};
       std::vector<Coordinates> coordinates;
        for(int i = 0;  i < lines.size(); ++i){
            auto pos = lines[i].find('#');
            if(pos == std::string::npos){
                empty_rows.push_back(i);
            } else{
                while(pos != std::string::npos){
                    coordinates.push_back({i, static_cast<int>(pos)});
                    pos = lines[i].find('#', pos+1);
                }
            }
        }
        return std::make_pair(empty_rows, coordinates);
   }

   std::vector<int> GetEmptyColumns(const std::vector<std::string> &lines){
        std::vector<int> empty_cols{};
        std::vector<Coordinates> coordinates;
        for(int i = 0;  i < lines.size(); ++i){
            bool galaxy{false};
            for(int j = 0; j < lines[0].size(); ++j){
                if(lines[j][i] == '#'){
                    galaxy = true;
                    break;
                }
            }
            if(not galaxy){
                empty_cols.push_back(i);
            }
        }
        return empty_cols;
    }

    class DistanceCalculator{
   public:
       DistanceCalculator(std::vector<int> empty_rows, std::vector<int> empty_cols) : empty_rows{std::move(empty_rows)}, empty_cols{std::move(empty_cols)}{}

       long calc(Coordinates left, Coordinates right){
           if(left.column > right.column) {
                std::swap(left.column, right.column);
           }
           long base_distance = right.column - left.column + right.row - left.row;
           for(auto i : empty_cols){
               if(i >= left.column && i < right.column){
                   base_distance+=999999;
               }
           }
           for(auto i : empty_rows){
               if(i >= left.row && i < right.row){
                   base_distance+=999999;
               }
           }
           return base_distance;
       }

   private:
        std::vector<int> empty_cols;
        std::vector<int> empty_rows;
    };


}


int main() {
    aoc::InputReader inputReader{};
    const auto& lines = inputReader.getLines();
    auto [empty_rows, galaxies] = ParseRows(lines);
    auto empty_columns = GetEmptyColumns(lines);
    DistanceCalculator calculator{empty_rows, empty_columns};
    long total{0};
    for(int i = 0; i < galaxies.size(); ++i){
        for(int j = i; j < galaxies.size(); ++j){
             total+=calculator.calc(galaxies[i], galaxies[j]);
        }
    }
    std::cout<<total<<std::endl;
    return 0;
}