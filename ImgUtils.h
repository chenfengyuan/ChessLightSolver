#ifndef IMGUTILS_H
#define IMGUTILS_H
#include "opencv2/core/core.hpp"
namespace ImgUtils{
size_t get_occur_times(cv::Mat origin, cv::Mat sub,double confidence=0.9);
std::vector<cv::Point> get_positions(cv::Mat origin, cv::Mat tpl);
cv::Mat get_binary_image(cv::Mat origin,double thresh);
}
#endif // IMGUTILS_H
