#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImgUtils.h"

int main(int,char *argv[])
{
    cv::Mat ref = cv::imread(argv[1]);
    cv::Mat tpl = cv::imread(argv[2]);
    for(auto & point : ImgUtils::get_positions(ref,tpl)){
        std::cout << point << std::endl;
    }
}
