#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImgUtils.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "chess.h"

int main(int,char *argv[])
{
    cv::Mat ref = cv::imread(argv[1]);
    std::cout << ref.rows << " " << ref.cols << std::endl;
    std::cout << ref.at<cv::Vec3b>(262,216) << std::endl;
    std::cout << static_cast<int>(ref.at<cv::Vec3b>(262,216)[0]) << std::endl;
    auto board =  Chess::get_board(ref);
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            if(board[i*8+j])
                std::cout << '*';
            else
                std::cout << ".";
        }
        std::cout << "\n";
    }
    cv::Mat tpl = cv::imread(argv[2]);
    for(auto & point : ImgUtils::get_positions(ref,tpl)){
        std::cout << point << std::endl;
    }
}
