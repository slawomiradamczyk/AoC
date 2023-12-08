//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include <map>
#include <sstream>
#include <any>
#include <numeric>
#include "input_reader.h"
#include "parser.h"

enum class Direction : bool {
    left = false,
    right = true
};

namespace {
    class Sequence{
    public:
        explicit Sequence(const std::string &left){
            steps.reserve(left.size());
              for(auto c : left){
                    if(c == 'L'){
                        steps.push_back(Direction::left);
                    } else {
                        steps.push_back(Direction::right);
                    }
              }
              pos = steps.begin();
        }

        Direction GetAndMove(){
           Direction ret_val = *pos;
            ++pos;
            if(pos == steps.end()){
                pos = steps.begin();
            }
            return ret_val;
        }

        void Reset(){
            pos = steps.begin();
        }

    private:
        std::vector<Direction> steps{};
        std::vector<Direction>::const_iterator pos{};
    };

    class DessertMap {
        using MapType = std::map<std::string, std::pair<std::any, std::any>>;
    public:
        explicit DessertMap(const std::vector<std::string> &lines){
            for(auto it = lines.begin()+2; it != lines.end(); ++it){
                std::istringstream iss{*it};
                std::string position, left, right;
                iss >> position >> left >> right;
                auto left_pos = dessert_map.insert({left, {}}).first;
                auto right_pos = dessert_map.insert({right, {}}).first;
                auto position_update = std::make_pair(left_pos, right_pos);
                auto current_pos = dessert_map.insert({position, position_update});
                if (not current_pos.second) {
                    dessert_map[position] = position_update;
                }
                if(position[2] == 'A'){
                    starting_nodes.push_back(current_pos.first);
                }
            }
        }

        long RunSequence(Sequence sequence){
            long ret_val{1};
            for(auto starting_node  : starting_nodes) {
                auto current_point = starting_node;
                long step{0};
                while(current_point->first[2] != 'Z') {
                    if(sequence.GetAndMove() == Direction::left){
                        current_point = std::any_cast<MapType::iterator>(current_point->second.first);
                    } else{
                        current_point = std::any_cast<MapType::iterator>(current_point->second.second);
                    }
                    ++step;
                }
                sequence.Reset();
                std::cout<<step<<std::endl;
                ret_val = std::lcm(step, ret_val);
            }
            return ret_val;
        }

        MapType dessert_map{};
        std::vector<MapType::iterator> starting_nodes{};
    };
}

int main() {
    aoc::InputReader inputReader{};
    const auto &lines = inputReader.getLines();
    Sequence seq(lines[0]);
    DessertMap dessert_map{lines};
    std::cout << dessert_map.RunSequence(std::move(seq));
    return 0;
}