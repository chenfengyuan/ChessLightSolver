#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImgUtils.h"

int main(int,char *argv[])
{
    {
        cv::Mat ref = cv::imread(argv[1]);
        cv::Mat tpl = cv::imread(argv[2]);
        for(auto & point : ImgUtils::get_positions(ref,tpl)){
            std::cout << point << std::endl;
        }
    }
    cv::Mat ref = cv::imread(argv[1]);
    cv::Mat tpl = cv::imread(argv[2]);
    if (ref.empty() || tpl.empty())
        return -1;

    cv::Mat gref, gtpl;
    cv::cvtColor(ref, gref, CV_BGR2GRAY);
    cv::cvtColor(tpl, gtpl, CV_BGR2GRAY);

    cv::Mat res(ref.rows-tpl.rows+1, ref.cols-tpl.cols+1, CV_32FC1);
    cv::matchTemplate(gref, gtpl, res, CV_TM_CCOEFF_NORMED);
    cv::threshold(res, res, 0.8, 1., CV_THRESH_TOZERO);

    while (true)
    {
        double minval, maxval, threshold = 0.8;
        cv::Point minloc, maxloc;
        cv::minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);

        std::cerr << maxloc.x << "," << maxloc.y << " : " << maxval << " " << res.at<float>(maxloc.y,maxloc.x) << std::endl;
        if (maxval >= threshold)
        {
            cv::rectangle(
                ref,
                maxloc,
                cv::Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows),
                CV_RGB(0,255,0), 2
            );
            cv::floodFill(res, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
        }
        else
            break;
    }

    cv::circle( ref,cv::Point(322,766),2,cv::Scalar(0,0,255),-1,8);
    cv::imshow("reference", ref);
    cv::waitKey();
    return 0;
}
