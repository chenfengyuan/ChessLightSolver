#include "ImgUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
namespace ImgUtils{
std::vector<cv::Point> get_positions(cv::Mat origin, cv::Mat tpl){
    cv::Mat gref, gtpl;
    std::vector<cv::Point> points;
    cv::cvtColor(origin, gref, CV_BGR2GRAY);
    cv::cvtColor(tpl, gtpl, CV_BGR2GRAY);

    cv::Mat res(origin.rows-tpl.rows+1, origin.cols-tpl.cols+1, CV_32FC1);
    cv::matchTemplate(gref, gtpl, res, CV_TM_CCOEFF_NORMED);
    cv::threshold(res, res, 0.8, 1., CV_THRESH_TOZERO);

    while (true)
    {
        double minval, maxval, threshold = 0.8;
        cv::Point minloc, maxloc;
        cv::minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);

        std::cerr << maxloc.x << "," << maxloc.y << " : " << maxval << " " << res.at<float>(maxloc.y,maxloc.x) << std::endl;
        points.push_back(maxloc);
        if (maxval >= threshold)
        {
            cv::rectangle(
                origin,
                maxloc,
                cv::Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows),
                CV_RGB(0,255,0), 2
            );
            cv::floodFill(res, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
        }
        else
            break;
    }

    cv::circle( origin,cv::Point(322,766),2,cv::Scalar(0,0,255),-1,8);
    return points;
}
cv::Mat get_binary_image(cv::Mat origin,double thresh){
    cv::Mat rv;
    cv::cvtColor(origin,rv,CV_BGR2GRAY);
    cv::threshold(rv,rv,thresh, 255, CV_THRESH_BINARY);
    return rv;
}
}
