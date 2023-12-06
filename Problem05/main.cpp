//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"
#include "parser.h"
#include "string_split.h"

#include <iostream>
#include <set>
#include <numeric>

namespace {
    std::vector<long> getlongs(const std::string_view &view) {
        std::vector<long> ret_val;
        auto long_str = aoc::split(view, ' ');
        for (const auto &long_s: long_str) {
            try {
                ret_val.push_back(aoc::getLong(long_s));
            } catch (const std::exception &e) {
                std::cout << "Failed to convert: " << long_s << std::endl;
            }
        }
        return ret_val;
    }

struct Mapping {
    explicit Mapping(const std::vector<long> &mapping_line) : destination{mapping_line[1]}, source{mapping_line[0]}, length{mapping_line[2]}{
    };

    [[nodiscard]] bool isInSourceRange(long value) const {
        return value >= source && value < source + length;
    }

    [[nodiscard]] long map(long value) const {
        if(isInSourceRange(value)) {
            return value - source + destination;
        }
        return value;
    }

    long destination{};
    long source{};
    long length{};
};

class MapResolver {
public:
    void AddMapping(Mapping mapping) {
        mappings.push_back(mapping);
    }
    [[nodiscard]] long Resolve(long value) const {
        for(const auto &mapping : mappings){
            if(mapping.isInSourceRange(value)){
                return mapping.map(value);
            }
        }
        return value;
    }
private:
    std::vector<Mapping> mappings{};
};

class SeedToLocationSolver {
public:
    void AddSolver(MapResolver solver){
        maps.insert(maps.begin(), std::move(solver));
    }
    long EndToEndRemap(long value){
        for(const auto &map : maps){
            value = map.Resolve(value);
        }
        return value;
    }
    std::vector<MapResolver> maps;
};

SeedToLocationSolver parseFromInputLines(const std::vector<std::string> &input_lines){
    SeedToLocationSolver solver;
    MapResolver current_resolver;
    for(auto it = input_lines.begin() + 3; it != input_lines.end(); ++it) {
        if(it->empty()){ continue; }
        if(it->find(':') != std::string::npos){
            solver.AddSolver(std::move(current_resolver));
            current_resolver = {};
        } else {
            current_resolver.AddMapping(Mapping(getlongs(*it)));
        }
    }
    solver.AddSolver(std::move(current_resolver));
    return solver;
}

std::vector<std::pair<long, long>> parseSeeds(const std::string &seed_line){
    std::vector<std::pair<long, long>> ret_val;
    auto seeds = getlongs(aoc::split(seed_line, ':')[1]);
    for(auto it = seeds.begin(); it != seeds.end(); it+=2){
        ret_val.emplace_back(*it, *(it+1));
    }
    return ret_val;
}

}

int main() {
    aoc::InputReader input{};
    const auto &input_lines = input.getLines();
    auto seed_defs = parseSeeds(input_lines[0]);
    auto solver = parseFromInputLines(input_lines);
    for(long i = 0; i < std::numeric_limits<long>::max(); ++i){
        auto seed = solver.EndToEndRemap(i);
        for(auto seed_def : seed_defs){
            if(seed >= seed_def.first && seed < seed_def.first+seed_def.second){
                std::cout << "Values:" << i << std::endl;
                return 0;
            }
        }
    }
}