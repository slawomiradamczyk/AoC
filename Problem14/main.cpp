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


namespace {
    void EmptyQueue(std::queue<int> &q) {
        while (not q.empty()) {
            q.pop();
        }
    }

    class Tilter {
    public:
        Tilter(const std::vector<std::string> &lines) : lines_{lines} {}


        void Tilt() {
            TiltNorth();
            TiltEast();
            TiltSouth();
            TiltWest();
        }

        void Print() {
            for (const auto &line: lines_) {
                std::cout << line << std::endl;
            }
        }

        long Load() {
            long ret_val = 0;
            for (int i = 0; i < lines_.size(); ++i) {
                auto number_of_blocks = std::count(lines_[i].begin(), lines_[i].end(), 'O');
                ret_val += number_of_blocks * (lines_.size() - i);
            }
            return ret_val;
        }

        bool operator == (const Tilter &rhs) const{
            return this->lines_ == rhs.lines_;
        }

    private:
        void TiltNorth() {
            for (int i = 0; i < lines_[0].size(); ++i) {
                std::queue<int> last_good;
                for (int j = 0; j < lines_.size(); ++j) {
                    ProcessFieldNY(i, last_good, j);
                }
            }
        }

        void ProcessFieldNY(int i, std::queue<int> &last_good, int j) {
            if (lines_[j][i] == '.') {
                last_good.push(j);
            } else if (lines_[j][i] == '#') {
                EmptyQueue(last_good);
            } else if (lines_[j][i] == 'O') {
                if (not last_good.empty()) {
                    lines_[last_good.front()][i] = 'O';
                    lines_[j][i] = '.';
                    last_good.push(j);
                    last_good.pop();
                }
            }
        }

        void TiltSouth() {
            for (int i = 0; i < lines_[0].size(); ++i) {
                std::queue<int> last_good;
                for (int j = lines_.size() - 1; j >= 0; --j) {
                    ProcessFieldNY(i, last_good, j);
                }
            }
        }

        void TiltEast() {
            for (int i = 0; i < lines_.size(); ++i) {
                    std::queue<int> last_good;
                for (int j = 0; j < lines_[0].size(); ++j) {
                    ProcessFieldNX(i, last_good, j);
                }
            }
        }

        void TiltWest() {
            for (int i = 0; i < lines_.size(); ++i) {
                    std::queue<int> last_good;
                for (int j = lines_[0].size() - 1; j >= 0; --j) {
                    ProcessFieldNX(i, last_good, j);
                }
            }
        }

        void ProcessFieldNX(int i, std::queue<int> &last_good, int j) {
            if (lines_[i][j] == '.') {
                last_good.push(j);
            } else if (lines_[i][j] == '#') {
                EmptyQueue(last_good);
            } else if (lines_[i][j] == 'O') {
                if (not last_good.empty()) {
                    lines_[i][last_good.front()] = 'O';
                    lines_[i][j] = '.';
                    last_good.push(j);
                    last_good.pop();
                }
            }
        }

        std::vector<std::string> lines_;
        };


    }



    int main() {
        aoc::InputReader inputReader{};
        const auto &lines = inputReader.getLines();
        Tilter tilter{lines};
        Tilter tilter_hare{lines};
        long unsigned i = 0;
        for(; i < 1000000000; ++i){
            tilter.Tilt();
            tilter_hare.Tilt();
            tilter_hare.Tilt();
            if(tilter == tilter_hare){
                std::cout << "Found cycle at " << i << std::endl;
                break;
            }
        }
        long cycle_length = 1;
        tilter.Tilt();
        while(not (tilter==tilter_hare)){
            tilter.Tilt();
            ++cycle_length;
        }
        std::cout << "Cycle length is " << cycle_length << std::endl;
        auto number_of_iterations = (1000000000UL - i - 1) % cycle_length;
        std::cout << "Number of iterations is " << number_of_iterations << std::endl;
        while(number_of_iterations--){
            tilter.Tilt();
        }

        std::cout << tilter.Load() << std::endl;
        return 0;
    }

