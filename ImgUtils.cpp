#include "ImgUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
namespace ImgUtils{
std::vector<cv::Point> get_positions(cv::Mat origin, cv::Mat tpl){
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

    return points;
}
}
