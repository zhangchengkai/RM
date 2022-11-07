#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Eigen/Dense"
#include "opencv2/core/eigen.hpp"

const std::vector<cv::Point3d> PW_BIG{// 灯条坐标，单位：m
                                             {-0.115, 0.0265, 0.},
                                             {-0.115, -0.0265, 0.},
                                             {0.115, -0.0265, 0.},
                                             {0.115, 0.0265, 0.}};
int main(){
    cv::Mat src;
    cv::Mat camera_matrix;
    cv::Mat distort_matrix;
    cv::FileStorage reader("../f_mat_and_c_mat.yml", cv::FileStorage::READ);
    reader["F"] >> camera_matrix;
    reader["C"] >> distort_matrix;


    src = cv::imread("../hero.jpg");
    std::vector<cv::Point2f> corners;
    corners.push_back(cv::Point2f(575.508,282.175));
    corners.push_back(cv::Point2f(573.93 ,331.819));
    corners.push_back(cv::Point2f(764.518 ,337.652));
    corners.push_back(cv::Point2f(765.729,286.741));

    cv::Mat rvec, tvec;
    cv::solvePnP(PW_BIG, corners, camera_matrix, distort_matrix, rvec, tvec);
    Eigen::Quaterniond q={ -0.0816168,0.994363,-0.0676645,-0.00122528};
    Eigen::Vector3d t_1;
    cv::cv2eigen(tvec,t_1);
    Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
    T.translate(t_1);
    Eigen::Vector3d t_imu = q*t_1;
    for(int i=0;i<3;i++)
        std::cout << "t_imu: "<< t_imu(i) << std::endl << std::endl;
    cv::waitKey(0);
    return 0;
}