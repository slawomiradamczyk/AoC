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


namespace {
    struct Group {
        Group(long len):length{len}{};
        long length;
        long min{};
        long max{};
    };

    bool can_be_broken(char c){
        return c == '#' || c == '?';
    }

    bool can_be_osasis(char c){
    return c == '.' || c == '?';
    }

    class Solver{
    public:
        Solver(const std::vector<Group> &groups, const std::string &pattern) : groups_{groups}, pattern_{pattern}, reverse_pattern_{pattern.rbegin(), pattern.rend()}  {
            GetMinMax();
        }

        long CountCombinations(){
            long total_multi=1;
            auto overlap_start = groups_.begin();
            for(auto it = overlap_start+1; it != groups_.end(); ++it){
                if(it->min > overlap_start->max){
                    std::string working_patter{pattern_};
                    for(int i = 0; i < working_patter.size(); ++i){
                        if(i < overlap_start->min || i >= it->min-1){
                            working_patter[i]='.';
                        }
                    }
                    total_multi*= CountSingleGroup(working_patter,overlap_start, it, overlap_start->min);
                    overlap_start = it;
                }
            }
            std::string working_patter{pattern_};
            for(int i = 0; i < working_patter.size(); ++i){
                if(i < overlap_start->min ){
                    working_patter[i]='.';
                }
            }
            total_multi*= CountSingleGroup(working_patter, overlap_start, groups_.end(), overlap_start->min);
            return total_multi;
        }

    private:

        std::optional<long> PlaceContig(std::string &pattern, long contig, long pos){
            auto point = std::find_if(pattern.begin()+pos, pattern.end(), can_be_broken);

            while(point != pattern.end()){
                if((point==pattern.begin() || can_be_osasis(*(point-1))) &&
                  ((point+contig==pattern.end()) || can_be_osasis(*(point+contig)))
                ){
                   if(std::find_if_not(point, point+contig, can_be_broken) == point + contig){
                       std::fill(point, point + contig, 'M');
                       return(std::distance(pattern.begin(), point));
                   }
                }
                point = std::find_if(point+1, pattern.end(), can_be_broken);
            }
            return std::nullopt;
        }

        void ClearPoint(std::string &pattern, const std::string &orig, long contig, long pos){
            for(long i = pos; i < pos + contig; ++i){
                pattern[i] = orig[i];
            }
        }


        void GetMinMax(){
            std::string working_pattern{pattern_};
            long starting_pos = 0;
            for(auto &group : groups_) {
                auto pos = *PlaceContig(working_pattern, group.length, starting_pos);
                group.min = pos;
                starting_pos = pos + group.length + 1;
            }
            std::reverse(groups_.begin(), groups_.end());
            auto reverse_patter{reverse_pattern_};
            starting_pos = 0;
            for(auto &group : groups_) {
                auto pos = *PlaceContig(reverse_patter, group.length, starting_pos);
                group.max = pos;
                starting_pos = pos + group.length + 1;
            }
            for(auto &group : groups_){
                group.max = pattern_.length() - group.max - group.length;
            }
            std::reverse(groups_.begin(), groups_.end());
        }

        long CountSingleGroup(const std::string& pattern, std::vector<Group>::iterator begin, std::vector<Group>::iterator  end, long min_start){
            if(begin == end){
                auto is_clean = pattern.find('#') == std::string::npos;
                return is_clean;
            }
            long multi_count = 0;
            auto working_pattern{pattern};
            auto starting_pos = std::max(begin->min, min_start);
            while(starting_pos <= begin->max && starting_pos >= min_start){
                auto pos = PlaceContig(working_pattern, begin->length, starting_pos);
                if(pos.has_value()){
                    auto val = CountSingleGroup(working_pattern, begin+1, end, *pos+begin->length+1);
                    multi_count += val;
                    working_pattern=pattern;
                    ++starting_pos;
                } else {
                    break;
                }
            }
            return multi_count;
        }

        const std::string &pattern_;
        std::vector<Group> groups_;
        const std::string reverse_pattern_;
    };


}


int main() {
    aoc::InputReader inputReader{};
    const auto& lines = inputReader.getLines();
    long combinations{0};
    for(const auto &line :lines){
        auto sep = line.find(' ');
        std::string patter = line.substr(0, line.find(' '));
        std::string pattern{patter};
        std::string numbers = line.substr(sep+1, std::string::npos);
        std::replace(numbers.begin(), numbers.end(), ',', ' ');
        auto contingues = aoc::getlongs(numbers);
        std::vector<long> new_contigues{contingues};
//        for(int i = 0; i < 4; ++i){
//            pattern+="?"+patter;
//            std::copy(contingues.begin(), contingues.end(), std::back_inserter(new_contigues));
//        }
        std::vector<Group> groups{};
        std::copy( new_contigues.begin(), new_contigues.end(), std::back_inserter(groups));

        Solver solver{groups, pattern};
        combinations += solver.CountCombinations();


    }
    std::cout<<combinations<<std::endl;
    return 0;
}

