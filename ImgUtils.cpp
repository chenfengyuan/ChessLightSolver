#include "ImgUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
namespace ImgUtils{
std::vector<bool> get_board(cv::Mat img){
    std::vector<bool> board;
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
                board.push_back(false);
            else
                board.push_back(true);
        }
    }
    return board;
}

std::vector<cv::Point> get_positions(cv::Mat origin, cv::Mat tpl){
    if(tpl.dims == 0)
        return std::vector<cv::Point>();
    cv::Mat gref, gtpl;
    std::vector<cv::Point> points;
    cv::cvtColor(origin, gref, CV_BGR2GRAY);
    cv::cvtColor(tpl, gtpl, CV_BGR2GRAY);

    cv::Mat res(origin.cols-tpl.cols+1,origin.rows-tpl.rows+1, CV_32FC1);
    cv::matchTemplate(gref, gtpl, res, CV_TM_CCOEFF_NORMED);
    cv::threshold(res, res, 0.8, 1., CV_THRESH_TOZERO);

    for(int i = 0;i<res.rows;++i){
        for(int j = 0;j<res.cols;++j){
            if(res.at<float>(i,j) > 0.8){
                points.push_back(cv::Point(j,i));
            }
        }
    }
    decltype(points.begin()->x) width = 20;
    std::sort(points.begin(),points.end(),[width](cv::Point a,cv::Point b){
        if(a.x + width < b.x)
            return true;
        else if(a.y + width < b.y)
            return true;
        else
            return false;
    });

    auto last = std::unique(points.begin(),points.end(),[width](cv::Point a,cv::Point b){
            if(std::labs(a.x - b.x)< width && std::labs(a.y - b.y) < width)
            return true;
            else
            return false;
});

    points.resize(last - points.begin());
    for(auto & point : points){
        point = cv::Point(point.y,point.x);
    }

    return points;
}
}
