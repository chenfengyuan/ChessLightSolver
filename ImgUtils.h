#ifndef IMGUTILS_H
#define IMGUTILS_H
#include "opencv2/core/core.hpp"
namespace ImgUtils{
std::vector<cv::Point> get_positions(cv::Mat origin, cv::Mat tpl);
}
#endif // IMGUTILS_H
