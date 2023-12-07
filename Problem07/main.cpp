//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"
#include "parser.h"

#include <iostream>
#include <map>
#include <set>
#include <sstream>

namespace {
    enum Figure {
        High = 0,
        Pair,
        TwoPair,
        Three,
        Full,
        Four,
        Five
    };

    class Hand {
    public:
        explicit Hand(std::string hand_str) : hand_str{std::move(hand_str)}{
            std::map<char, int> counters;
            int jokers = 0;
            for(char c: this->hand_str){
                if(c == 'J') {
                    jokers+=1;
                } else {
                    counters[c]+=1;
                };
            }
            if(counters.empty()){
                figure = Five;
            } else {
                auto it = std::max_element(counters.begin(), counters.end(), [](const auto &lhs, const auto &rhs) {
                    return lhs.second < rhs.second;
                });
                it->second+=jokers;
                if(counters.size() == 1){
                    figure = Five;
                }
                else if(counters.size() == 5) {
                    figure = High;
                } else if(counters.size() == 4) {
                    figure = Pair;
                } else if(counters.size() == 3) {
                    if (counters.begin()->second == 3 || std::next(counters.begin(),1)->second  == 3 || std::next(counters.begin(),2)->second  == 3){
                        figure = Three;
                    } else {
                        figure = TwoPair;
                    }
                } else {
                    if(counters.begin()->second  == 4 || counters.begin()->second == 1){
                        figure = Four;
                    } else {
                        figure = Full;
                    }
                }

            }

            orig_hand = this->hand_str;
            for(char &c : this->hand_str) {
                switch(c){
                    case 'A':{
                       c = 'Z'; break;
                    }
                    case 'K':{
                       c = 'Y'; break;
                    }
                    case 'Q':{
                        c = 'X'; break;
                    }
                    case 'J':{
                        c = '1'; break;
                    }
                    case 'T':{
                        c = 'V'; break;
                    }
                    default:
                        break;
                }
            }
        }

        bool operator < (const Hand & hand) const{
            if(figure == hand.figure){
                return hand_str < hand.hand_str;
            } else {
                return figure < hand.figure;
            }
        }
    private:
        Figure figure;
        std::string hand_str;
        std::string orig_hand;
    };

    struct Play {
        Hand hand;
        int bid;

        bool operator < (const Play &play) const {
            return hand < play.hand;
        }
    };

}

int main() {
    aoc::InputReader inputReader{};
    long total = 0;
    const auto &lines = inputReader.getLines();
    std::set<Play> plays;
    for (const auto &line: lines) {
        std::istringstream iss{line};
        std::string hand_str;
        int bid;
        iss >> hand_str >> bid;
        plays.insert({Hand(std::move(hand_str)), bid});
    }
    for(auto it = plays.begin(); it != plays.end(); ++it ){
        auto rank = std::distance(plays.begin(), it)+1;
        total+=rank * it->bid;
    }
    std::cout << total << std::endl;
}