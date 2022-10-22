#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
int n;
int main(){
    Eigen::Matrix<double, 3, 4> cam_f;
    cam_f << 400., 0.  , 190., 0.,
             0.  , 400., 160., 0.,
             0.  , 0.  , 1.  , 0.;
	cv::Mat image(680,1380,CV_8UC3);
    freopen("../points.txt","r",stdin);
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        double x,y,z;
        scanf("%lf%lf%lf",&x,&y,&z);
        Eigen::Vector4d w4;
        w4 << x,y,z,1;
		Eigen::Quaterniond q={-0.5,0.5,0.5,-0.5};
        Eigen::Vector3d cam_w = {2,2,2};
        Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
        Eigen::Matrix3d rot_c_to_w = q.matrix();
        converter.block(0, 0, 3, 3) = rot_c_to_w.transpose().cast<double>();
        converter.block(0, 3, 3, 1) =-rot_c_to_w.transpose().cast<double>() * cam_w;
        converter(3, 3) = 1.;
        Eigen::Vector4d c4 = converter * w4;
        Eigen::Vector3d u3 = cam_f * c4;
        // 归⼀化像素坐标
        u3 /= u3(2, 0);
		cv::Point2d point_img(u3(0,0),u3(1,0));
		cv::circle(image,point_img,0.1,cv::Scalar(255,255,255));
    }
	cv::imshow("final",image);
    cv::imwrite("../res.png", image);
	cv::waitKey(50);
    return 0;
}