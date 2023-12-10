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
}

enum Direction {
    UP=0,
    LEFT,
    DOWN,
    RIGHT
};

enum Symbol : char {
    VERTIAL='|',
    HORIZONTAL='-',
    RUP = 'L',
    LUP = 'J',
    DOT='.',
    LDOWN = '7',
    RDOWN = 'F',
    START = 'S',
    OHNO = '0',
    INS = 'T'
};

using PipeMap = std::vector<std::string>;
using Position = std::pair<int, int>;

class PipeResolver{
public:
    explicit PipeResolver(PipeMap lines) : lines{std::move(lines)}, starting_position{GetStartingPosition()}{
        current_position = starting_position;
        next_move = GetStartingMove();
        new_lines = std::vector<std::string>(this->lines.size()*3, std::string(this->lines[0].size()*3, '.'));
    }

    long RunUntilBeginning(){
        long steps{1};
        PerformMove();
        while(current_position != starting_position){
            PerformMove();
            ++steps;
        }
        return steps;
    }

    void Print(){
        for(const auto & line : new_lines){
            std::cout<<line<<std::endl;
        }
    }


    void FillGaps(){
        for(int y_pos = 0; y_pos < new_lines.size(); ++y_pos){
            auto &line = new_lines[y_pos];
            auto x_pos = line.find('.');
            while(x_pos != std::string::npos){
                PerformSpill(x_pos, y_pos);
                x_pos = line.find('.');
            }
        }
    }

    long CountInside(){
        long total{0};
        for(int i = 1; i < new_lines.size(); i+=3){
           for(int j = 1; j < new_lines[i].size(); j+=3){
                if(new_lines[i][j]==INS){
                    ++total;
                }
           }
        }
        return total;
    }

private:

    std::optional<Position> SpillInDirection(Position position, Direction direction){
        std::optional<Position> ret_val;
        switch(direction){
            case UP:
                if(position.second > 0){
                   auto new_c = new_lines[position.second-1][position.first];
                   if(new_c == '0') {
                       is_inside = false;
                   } else if(new_c == DOT) {
                       ret_val = {position.first, position.second - 1};
                   }
                } else {
                    is_inside = false;
                }
                break;
            case DOWN:
                if(position.second < new_lines.size() - 1){
                    auto new_c = new_lines[position.second+1][position.first];
                    if(new_c == '0') {
                        is_inside = false;
                    } else if(new_c == DOT) {
                        ret_val = {position.first, position.second + 1};
                    }
                } else {
                    is_inside = false;
                }
                break;
            case LEFT:
                if(position.first > 0){
                    auto new_c = new_lines[position.second][position.first - 1];
                    if(new_c == '0') {
                        is_inside = false;
                    } else if(new_c == DOT) {
                        ret_val = {position.first - 1, position.second};
                    }
                } else {
                    is_inside = false;
                }
                break;
            case RIGHT:
                if(position.first < new_lines[0].size() - 1){
                    auto new_c = new_lines[position.second][position.first + 1];
                    if(new_c == '0') {
                        is_inside = false;
                    } else if(new_c == DOT) {
                        ret_val = {position.first + 1, position.second};
                    }
                } else {
                    is_inside = false;
                }
                break;
        }
        return ret_val;
    }

    void PerformSpill(int x_pos, int y_pos){
        std::stack<Position> spills_left{};
        spills_left.emplace(x_pos, y_pos);
        is_inside = true;
        std::vector<Position> spilled{};
        while(not spills_left.empty()){
            auto spill = spills_left.top();
            spills_left.pop();
            auto next_try = SpillInDirection(spill, UP);
            if(next_try){
                spills_left.push(std::move(*next_try));
            }
            next_try = SpillInDirection(spill, LEFT);
            if(next_try){
                spills_left.push(std::move(*next_try));
            }
            next_try = SpillInDirection(spill, RIGHT);
            if(next_try){
                spills_left.push(std::move(*next_try));
            }
            next_try = SpillInDirection(spill, DOWN);
            if(next_try){
                spills_left.push(std::move(*next_try));
            }
            new_lines[spill.second][spill.first] = '?';
            spilled.push_back(spill);
        }
        for(const auto & spill : spilled){
            if(is_inside){
                new_lines[spill.second][spill.first] = INS;
            } else {
                new_lines[spill.second][spill.first] = OHNO;
            }
        }
    }

    void PerformMove(){
        if(next_move == UP) {
            current_position.second-=1;
        } else if (next_move == DOWN) {
            current_position.second+=1;
        } else if(next_move == LEFT) {
            current_position.first-=1;
        } else {
            current_position.first+=1;
        }

        switch(lines[current_position.second][current_position.first]) {
            case RUP:
                if(next_move == DOWN) {
                    next_move = RIGHT;
                } else {
                    next_move =  UP;
                }
                break;
            case LUP:
                if(next_move == DOWN) {
                    next_move = LEFT;
                } else {
                    next_move =  UP;
                }
                break;
            case RDOWN:
                if(next_move == UP) {
                    next_move = RIGHT;
                } else {
                    next_move = DOWN;
                }
                break;
            case LDOWN:
                if(next_move == UP) {
                    next_move = LEFT;
                } else {
                    next_move = DOWN;
                }
                break;
            default:
                break;
        }
        FillBiggerMap();
    }

    Position GetStartingPosition(){
        for(int y=0; y<lines.size(); ++y){
            for(int x=0; x<lines[y].size(); ++x){
                if(lines[y][x] == Symbol::START){
                    return std::make_pair(x, y);
                }
            }
        }
        throw std::runtime_error{"Wrong input"};
    };

    void FillBiggerMap(){
        auto y_offset = 3 * current_position.second;
        auto x_offset = 3 * current_position.first;
        switch(lines[current_position.second][current_position.first]){
            case HORIZONTAL:
                new_lines[y_offset + 1][x_offset]=HORIZONTAL;
                new_lines[y_offset + 1][x_offset+1]=HORIZONTAL;
                new_lines[y_offset + 1][x_offset+2]=HORIZONTAL;
                break;
            case VERTIAL:
                new_lines[y_offset][x_offset+1]=VERTIAL;
                new_lines[y_offset+1][x_offset+1]=VERTIAL;
                new_lines[y_offset+2][x_offset+1]=VERTIAL;
                break;
            case RDOWN:
                new_lines[y_offset+1][x_offset+1]=RDOWN;
                new_lines[y_offset+1][x_offset+2]=HORIZONTAL;
                new_lines[y_offset+2][x_offset+1]=VERTIAL;
                break;
            case RUP:
                new_lines[y_offset+1][x_offset+1]=RUP;
                new_lines[y_offset][x_offset+1]=VERTIAL;
                new_lines[y_offset+1][x_offset+2]=HORIZONTAL;
                break;
            case LDOWN:
                new_lines[y_offset+1][x_offset+1]=LDOWN;
                new_lines[y_offset+2][x_offset+1]=VERTIAL;
                new_lines[y_offset+1][x_offset]=HORIZONTAL;
                break;
            case LUP:
                new_lines[y_offset+1][x_offset+1]=LUP;
                new_lines[y_offset][x_offset+1]=VERTIAL;
                new_lines[y_offset+1][x_offset]=HORIZONTAL;
                break;

        }

    }

    bool IsDirectionPossible(Direction direction){
        switch(direction){
            case UP:
                if(current_position.second > 0){
                    auto next_symbol = lines[current_position.second-1][current_position.first];
                    const std::vector<Symbol> allowed_symbols{VERTIAL, LDOWN, RDOWN};
                    if(std::find(allowed_symbols.begin(), allowed_symbols.end(), next_symbol) != allowed_symbols.end()){
                        return true;
                    }
                }
                break;
            case DOWN:
                if(current_position.second < lines.size()-1){
                    auto next_symbol = lines[current_position.second+1][current_position.first];
                    const std::vector<Symbol> allowed_symbols{LUP, RUP, VERTIAL};
                    if(std::find(allowed_symbols.begin(), allowed_symbols.end(), next_symbol) != allowed_symbols.end()){
                        return true;
                    }

                }
                break;
            case LEFT:
                if(current_position.first > 0){
                    auto next_symbol = lines[current_position.second][current_position.first-1];
                    const std::vector<Symbol> allowed_symbols{HORIZONTAL, RUP, RDOWN};
                    if(std::find(allowed_symbols.begin(), allowed_symbols.end(), next_symbol) != allowed_symbols.end()){
                        return true;
                    }

                }
                break;
            case RIGHT:
                if(current_position.first < lines[current_position.second].size()-1){
                    auto next_symbol = lines[current_position.second][current_position.first+1];
                    const std::vector<Symbol> allowed_symbols{HORIZONTAL, LUP, LDOWN};
                    if(std::find(allowed_symbols.begin(), allowed_symbols.end(), next_symbol) != allowed_symbols.end()){
                        return true;
                    }

                }
                break;
        }
        return false;
    }

    Direction GetStartingMove(){
        std::vector<Direction> directions_possible{};
        for(auto direction : {LEFT, RIGHT, UP, DOWN}){
            if(IsDirectionPossible(direction)){
                directions_possible.push_back(direction);
            }
        }
        if(directions_possible.empty()){
            throw std::runtime_error{"Wrong input"};

        }
        Symbol start_symbol = DOT;
        if(directions_possible[0]==LEFT){
            if(directions_possible[1]==RIGHT){
                start_symbol = HORIZONTAL;
            } else if (directions_possible[1] == DOWN){
                start_symbol = LDOWN;
            } else if (directions_possible[1] == UP){
                start_symbol = LUP;
            }
        } else if (directions_possible[0] == RIGHT) {
            if(directions_possible[1] == DOWN){
               start_symbol = RDOWN;
            } else if(directions_possible[1] == UP){
                start_symbol = RUP;
            }
        } else {
            start_symbol = VERTIAL;
        }
        lines[starting_position.second][starting_position.first] = start_symbol;
        return directions_possible[0];
    }

    PipeMap lines;
    PipeMap new_lines;
    const Position starting_position;
    Direction next_move;
    Position current_position;
    bool is_inside{false};
};


int main() {
    aoc::InputReader inputReader{};
    auto lines = inputReader.getLines();
    PipeResolver resolver{std::move(lines)};
    resolver.RunUntilBeginning();
    resolver.Print();
    resolver.FillGaps();
    resolver.Print();
    std::cout<<"Inside: "<<resolver.CountInside()<<std::endl;
    return 0;
}