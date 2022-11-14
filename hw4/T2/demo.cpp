#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Eigen/Dense"
#include "opencv2/core/eigen.hpp"
#include "VO.h"
using namespace cv;
using namespace Eigen;
using namespace std;

int main(int argc,char** argv)
{
    // camera matrix
    FileStorage params("../camera.yaml",FileStorage::READ);
    Mat K = params["K"].mat();    // camera matrix

    Mat img1 = imread("../stereo-data/0_orig.jpg");
    Mat img2 = imread("../stereo-data/1_orig.jpg");
    Mat depth1 =imread("../stereo-data/0_dpt.tiff",IMREAD_UNCHANGED);
    Mat depth2 =imread("../stereo-data/1_dpt.tiff",IMREAD_UNCHANGED);
    Mat rvec, tvec;
    find_PnP(img1, depth1, img2, depth2, K, rvec, tvec);
    Mat R;
    Rodrigues(rvec, R);
    process_Stitch_project(img1, depth1, img2, depth2, K, R, tvec, "result");
    return 0;
}