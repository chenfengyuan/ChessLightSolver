#include "chess.h"
#include <vector>
#include <opencv2/opencv.hpp>
namespace Chess{
Chess::Board get_board(cv::Mat img){
    Chess::Board board{};
    for(int i = 0;i < 8;++i){
        for(int j = 0;j < 8;++j){
            auto x = 160 + i * 65;
            auto y = 90 + j * 65;
            std::vector<long long> values{};
            for(int di = -10 ;di < 10;++di){
                auto value = img.at<cv::Vec3b>(x + di,y);
                auto v = static_cast<int>(value[0]) * static_cast<int>(value[1]) * static_cast<int>(value[2]);
                values.push_back(v);
            }
            sort(values.begin(),values.end());
            if(*(values.end()-1)- *(values.begin()) > 700000)
                board.at(i * 8 + j) = false;
            else
                board.at(i * 8 + j) = true;
        }
    }
    return board;
}
}