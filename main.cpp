#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImgUtils.h"
#include <vector>
#include <algorithm>
#include <cstdlib>

int main(int,char *argv[])
{
    cv::Mat ref = cv::imread(argv[1]);
    std::cout << ref.rows << " " << ref.cols << std::endl;
    std::cout << ref.at<cv::Vec3b>(262,216) << std::endl;
    std::cout << static_cast<int>(ref.at<cv::Vec3b>(262,216)[0]) << std::endl;
    for(int i = 0;i < 8;++i){
        for(int j = 0;j < 8;++j){
            auto x = 160 + i * 65;
            auto y = 90 + j * 65;
            std::vector<long long> values{};
            for(int di = -10 ;di < 10;++di){
                auto value = ref.at<cv::Vec3b>(x + di,y);
                auto v = static_cast<int>(value[0]) * static_cast<int>(value[1]) * static_cast<int>(value[2]);
                values.push_back(v);
            }
            sort(values.begin(),values.end());
            if(*(values.end()-1)- *(values.begin()) > 700000)
                std::cout << '.';
            else
                std::cout << '*';
        }
        std::cout << "\n";
    }
    cv::Mat tpl = cv::imread(argv[2]);
    for(auto & point : ImgUtils::get_positions(ref,tpl)){
        std::cout << point << std::endl;
    }
}
