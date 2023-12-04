//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input_reader.h"
#include "parser.h"

#include <iostream>
#include <map>

namespace {
}


int main() {
    aoc::InputReader input{};
    const auto &input_lines = input.getLines();
    int total{0};
    std::map<const char *, std::vector<int>> gear_candidates{};
    for (int i = 0; i < input_lines.size(); ++i) {
    }

    std::cout << "Values:" << total << std::endl;
}