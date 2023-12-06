//
// Created by Slawomir Adamczyk on 01.12.23.
// Copyright (c) 2023 FineCode IT Slawomir Adamczyk. All rights reserved.
//
#include "input.h"
#include "input_reader.h"
#include "parser.h"
#include "string_split.h"

#include <iostream>
#include <set>
#include <numeric>
#include <optional>
#include <cmath>


namespace {

    class QuadraticEquation{
        using Solution = std::optional<std::pair<double, double>>;
    public:
        QuadraticEquation(long a, long b, long c) : a{a}, b{b}, c{c}{}

        Solution Solve(){
            Solution ret_val;
            auto delta = countDelta();
            if(delta >= 0) {
                auto first = (-b - sqrt(delta))/2*a;
                auto second = (-b + sqrt(delta))/2*a;
                if(first > second) {
                    std::swap(first, second);
                }
                ret_val.emplace(std::make_pair(first, second));
            }
            return ret_val;
        };

    private:
        double countDelta() const{
            return static_cast<double>(b)*b - 4*a*c;
        };

        long a;
        long b;
        long c;
    };

    long firstSmallest(double x) {
        auto res = std::floor(x);
        if(x == res){
            res-=1;
        }
        return static_cast<long>(res);
    }

    long firstLarger(double x) {
        auto res = std::ceil(x);
        if(x == res){
            res+=1;
        }
        return static_cast<long>(res);
    }
}



int main() {
    long total = 1;
    for(long i = 0; i < input::distance.size(); ++i){
        QuadraticEquation eq{-1, input::time[i], -input::distance[i]};
        auto solution = eq.Solve();
        if(solution){
            auto min = firstLarger(solution->first);
            auto max = firstSmallest(solution->second);
            auto min_clamped = std::max(1L, min);
            auto max_clamped = std::min(input::time[i]-1, max);
            total*=max_clamped-min_clamped+1;
        }
    }
    std::cout << total << std::endl;
}