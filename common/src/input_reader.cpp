//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//

#include <fstream>
#include <exception>
#include "input_reader.h"

namespace {
    constexpr auto kInputFilename = "input.h";
}

namespace aoc {
    InputReader::InputReader() {
        std::ifstream file{kInputFilename};

        if (file.fail()) {
            throw std::runtime_error("Failed to open file: ");
        }

        std::string line;

        while (std::getline(file, line)) {
            m_lines.push_back(line);
        }
    }

    const std::vector<std::string>& InputReader::getLines() const {
        return m_lines;
    }
} // aoc
