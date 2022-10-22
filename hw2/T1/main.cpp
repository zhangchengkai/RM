#include<cstdio>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<assert.h>
int main(){
    cv::Mat src = cv::imread("../ass.png"),ans;
	cv::Point2f pic[4]={cv::Point2f(740,410),cv::Point2f(957,413),cv::Point2f(747,490),cv::Point2f(968,489)};
	cv::Point2f rel[4]={cv::Point2f(0,0),cv::Point2f(440,0),cv::Point2f(0,140),cv::Point2f(440,140)};

    cv::Mat res=cv::getPerspectiveTransform(pic, rel);
    cv::warpPerspective(src, ans, res, cv::Size(440,140));

    cv::imshow("ans", ans);
    cv::imwrite("../ans.png", ans);
    cv::waitKey(0);
	return 0;
}
