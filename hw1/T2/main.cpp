#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>
#include<queue>
const int N=3;

double calc_threshold(const std::vector<std::vector<cv::Point>> &contours) {
    std::priority_queue<double> areas;
    for (const auto &item: contours) areas.push(cv::arcLength(item,1));
    int cnt = 0;
    double threshold = 1e7;
    while (!areas.empty() && (++cnt) <= N) {
        threshold = std::min(threshold, areas.top());
        areas.pop();
    }
    return threshold;
}

bool judge_counter(const std::vector<cv::Point> &contour, double threshold) {
    return cv::arcLength(contour,1)>= threshold;
}

int main(int argc, char ** argv){
    cv::Mat src = cv::imread("../ass.png");
    assert(src.channels() == 3); // 检测是否为三通道彩⾊图⽚
    
    cv::Mat channels[3];
    cv::split(src, channels); // 三通道分离
    cv::Mat red_sub_blue = channels[2] - channels[0]; // 红蓝通道相减
    cv::Mat normal_mat;
    cv::normalize(red_sub_blue, normal_mat, 0., 255., cv::NORM_MINMAX); // 归 ⼀化到[0, 255]
    cv::Mat result;
    cv::threshold(normal_mat, result, 100, 255, 80); // OTSU⾃ 适应阈值
    cv::Mat A=getStructuringElement(cv::MORPH_ELLIPSE, cv::Point(3,3));
    cv::Mat chan;
    morphologyEx(result,chan,cv::MORPH_OPEN,A,cv::Point(-1,-1),3);
    assert(result.channels() == 1);
    cv::Mat canny_result;
    cv::Canny(chan, canny_result, 50, 100, 3);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierachy;
    cv::findContours(canny_result, contours, hierachy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    double threshold = calc_threshold(contours);

    std::vector<std::vector<cv::Point>> res;
    for (auto it = contours.begin(); it != contours.end(); it++) {
        if (judge_counter(*it, threshold)) {
            res.push_back(*it);
        }
    }

    cv::Mat drawer = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
    cv::drawContours(drawer, res, -1, {255, 255, 255}, 3);

    int x1 = 1e4, x2 = -1, y1 = 1e4, y2 = -1; // x1=min x,x2= max x,y1=min y,y2=max y
    for (auto sub: res) {
        for (auto point: sub) {
            x1 = std::min(x1, point.x);
            x2 = std::max(x2, point.x);
            y1 = std::min(y1, point.y);
            y2 = std::max(y2, point.y);
        }
    }
    cv::Point p1(x1, y1);
    cv::Point p2(x2, y1);
    cv::Point p3(x1, y2);
    cv::Point p4(x2, y2);
    cv::rectangle(src, p1, p4, cv::Scalar(255, 255, 0));
    cv::rectangle(drawer, p1, p4, cv::Scalar(255, 255, 0));

    cv::imshow("src", src);
    cv::waitKey(0);
    cv::imwrite("../res.png",src);
    return 0; 
}
