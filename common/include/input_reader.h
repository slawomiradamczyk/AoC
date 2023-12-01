//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//

#ifndef AOC_INPUT_READER_H
#define AOC_INPUT_READER_H

#include <vector>
#include <string>

namespace aoc {

    class InputReader {
    public:
        explicit InputReader();

        [[nodiscard]] const std::vector<std::string> &getLines() const;

    private:
        std::vector<std::string> m_lines;
    };

} // aoc

#endif //AOC_INPUT_READER_H
