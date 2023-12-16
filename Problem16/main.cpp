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
#include <set>


namespace {
    struct Coordinates {
        int x;
        int y;

        bool operator<(const Coordinates &rhs) const {
            if (x == rhs.x) {
                return y < rhs.y;
            } else {
                return x < rhs.x;
            }
        }
    };

    enum EnterDirection : int {
        North = 1,
        South = 2,
        West = 4,
        East = 8
    };

    struct PointWithDirection {
        Coordinates coordi;
        EnterDirection direction;
    };


    class Map {
    public:
        explicit Map(const std::vector<std::string> &lines) : lines(lines) {}

        void Run(PointWithDirection starting_point) {
            std::vector<PointWithDirection> points{starting_point};
            while (!points.empty()) {
                std::vector<PointWithDirection> nextPoints = NextMove(points.back());
                points.pop_back();
                std::copy(nextPoints.begin(), nextPoints.end(), std::back_inserter(points));
            }
        }

        void Print() {
            for (const auto &line : lines) {
                std::cout << line << std::endl;
            }
        }

        auto Count(){
            auto total = 0L;
            for(const auto &line : lines){
                total += std::count_if(line.begin(), line.end(), [](auto c){
                    return c == '#' || c == 'V' || c == 'H';
                });

            }
            return total + visited.size();
        }


    private:
        void SetPoint(Coordinates coords, char c) {
            lines[coords.y][coords.x] = c;
        }

        std::vector<PointWithDirection> NextMove(PointWithDirection point) {
            std::vector<PointWithDirection> result;
            if (point.coordi.x < 0 || point.coordi.x >= lines[0].size() || point.coordi.y < 0 ||
                point.coordi.y >= lines.size()) {
                return result;
            }
            auto c = lines[point.coordi.y][point.coordi.x];
            switch (c) {
                case '#':
                    break;
                case '.':
                    switch (point.direction) {
                        case North:
                            SetPoint(point.coordi, 'V');
                            point.coordi.y--;
                            break;
                        case South:
                            SetPoint(point.coordi, 'V');
                            point.coordi.y++;
                            break;
                        case West:
                            SetPoint(point.coordi, 'H');
                            point.coordi.x--;
                            break;
                        case East:
                            SetPoint(point.coordi, 'H');
                            point.coordi.x++;
                            break;
                    }
                    result.push_back(point);
                    break;
                case 'H':
                    switch (point.direction) {
                        case West:
                        case East:
                            break;
                        case North:
                            SetPoint(point.coordi, '#');
                            point.coordi.y--;
                            result.push_back(point);
                            break;
                        case South:
                            SetPoint(point.coordi, '#');
                            point.coordi.y++;
                            result.push_back(point);
                            break;
                    }
                    break;
                case 'V':
                    switch (point.direction) {
                        case North:
                        case South:
                            break;
                        case West:
                            SetPoint(point.coordi, '#');
                            point.coordi.x--;
                            result.push_back(point);
                            break;
                        case East:
                            SetPoint(point.coordi, '#');
                            point.coordi.x++;
                            result.push_back(point);
                            break;
                    }
                    break;
                case '-':
                    visited.insert(point.coordi);
                    switch (point.direction) {
                        case North:
                        case South: {
                            PointWithDirection p1{point.coordi, West};
                            PointWithDirection p2{point.coordi, East};
                            p1.coordi.x--;
                            p2.coordi.x++;
                            result.push_back(p1);
                            result.push_back(p2);
                            break;
                        }
                        case West:
                            point.coordi.x--;
                            result.push_back(point);
                            break;
                        case East:
                            point.coordi.x++;
                            result.push_back(point);
                            break;
                    }
                    break;
                case '|':
                    visited.insert(point.coordi);
                    switch (point.direction) {
                        case West:
                        case East: {
                            PointWithDirection p1{point.coordi, North};
                            PointWithDirection p2{point.coordi, South};
                            p1.coordi.y--;
                            p2.coordi.y++;
                            result.push_back(p1);
                            result.push_back(p2);
                            break;
                        }
                        case North:
                            point.coordi.y--;
                            result.push_back(point);
                            break;
                        case South:
                            point.coordi.y++;
                            result.push_back(point);
                            break;
                    }
                    break;
                case '/':
                    visited.insert(point.coordi);
                    switch (point.direction) {
                        case North:
                            point.direction = East;
                            point.coordi.x++;
                            result.push_back(point);
                            break;
                        case South:
                            point.direction = West;
                            point.coordi.x--;
                            result.push_back(point);
                            break;
                        case West:
                            point.direction = South;
                            point.coordi.y++;
                            result.push_back(point);
                            break;
                        case East:
                            point.direction = North;
                            point.coordi.y--;
                            result.push_back(point);
                            break;
                    }
                    break;
                case '\\':
                    visited.insert(point.coordi);
                    switch (point.direction) {
                        case North:
                            point.direction = West;
                            point.coordi.x--;
                            result.push_back(point);
                            break;
                        case South:
                            point.direction = East;
                            point.coordi.x++;
                            result.push_back(point);
                            break;
                        case West:
                            point.direction = North;
                            point.coordi.y--;
                            result.push_back(point);
                            break;
                        case East:
                            point.direction = South;
                            point.coordi.y++;
                            result.push_back(point);
                            break;
                    }
                    break;

            }
            return result;
        }

        std::set<Coordinates> visited;
        std::vector<std::string> lines;
    };

    long SingleRun(const auto &lines, PointWithDirection starting_point){
        Map map{lines};
        map.Run(starting_point);
        return map.Count();
    }

}


int main() {
    aoc::InputReader inputReader{};
    const auto &lines = inputReader.getLines();
    long max = std::numeric_limits<long>::min();
    //top row
    for(int x = 0; x < lines[0].size(); ++x){
        PointWithDirection point{{x, 0}, South};
        max = std::max(max, SingleRun(lines, point));
    }
    //bottom row
    for(int x = 0; x < lines[0].size(); ++x){
        PointWithDirection point{{x, static_cast<int>(lines.size()-1)}, North};
        max = std::max(max, SingleRun(lines, point));
    }
    //east row
    for(int y = 0; y < lines.size(); ++y){
        PointWithDirection point{{0, y}, East};
        max = std::max(max, SingleRun(lines, point));
    }
    //west row
    for(int y = 0; y < lines.size(); ++y){
        PointWithDirection point{{static_cast<int>(lines[0].size()-1), y}, West};
        max = std::max(max, SingleRun(lines, point));
    }
    std::cout<<max<<std::endl;
    return 0;
}

