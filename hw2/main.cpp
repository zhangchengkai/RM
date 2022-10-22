#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
int n;
struct node{
    double x,y,z;
}p[100001];
int main(){
    cv::VideoWriter writer("../dragon.avi",cv::VideoWriter::fourcc('M','J','P','G'),20,cv::Size2d(1680,1080),true);

    Eigen::Matrix<double, 3, 4> cam_f;//Camera internal parameters
    cam_f << 400., 0.  , 190., 0.,
             0.  , 400., 160., 0.,
             0.  , 0.  , 1.  , 0.;
    freopen("../points.txt","r",stdin);
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        double x,y,z;
        scanf("%lf%lf%lf",&x,&y,&z);
        p[i]=(node){x,y,z};
    }
    cv::Mat iic(1080,1680,CV_8UC3);
    cv::Mat image(1080,1680,CV_8UC3);
    for(int i=100;i>=0;i--){
        double res=(0.01*i)*(0.01*i);
        Eigen::Vector3d cam_w = {2+0.01*i,2,2-0.01*i};//Camera coordinates
    	image=iic.clone();
        for(int j=1;j<=n;j++){
            Eigen::Vector4d w4;
            w4 << p[j].x,p[j].y,p[j].z,1;//World coordinates
            Eigen::Quaterniond q={-0.5,0.5,0.5,-0.5};//Gyroscope coordinates
            Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
            Eigen::Matrix3d rot_c_to_w = q.matrix();//Rotation matrix
            converter.block(0, 0, 3, 3) = rot_c_to_w.transpose().cast<double>();
            converter.block(0, 3, 3, 1) =-rot_c_to_w.transpose().cast<double>() * cam_w;
            converter(3, 3) = 1.;//Camera external parameters
            Eigen::Vector4d c4 = converter * w4;
            Eigen::Vector3d u3 = cam_f * c4;
            // 归⼀化像素坐标
            u3 /= u3(2, 0);
            cv::Point2d point_img(u3(0,0),u3(1,0));
            cv::circle(image,point_img,0.1,cv::Scalar(255,255,255));
        }
    	writer.write(image);
    }
    for(int i=0;i<10;i++){
        Eigen::Vector3d cam_w = {2,2,2};//Camera coordinates
    	image=iic.clone();
        for(int j=1;j<=n;j++){
            Eigen::Vector4d w4;
            w4 << p[j].x,p[j].y,p[j].z,1;//World coordinates
            Eigen::Quaterniond q={-0.5,0.5,0.5,-0.5};//Gyroscope coordinates
            Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
            Eigen::Matrix3d rot_c_to_w = q.matrix();//Rotation matrix
            converter.block(0, 0, 3, 3) = rot_c_to_w.transpose().cast<double>();
            converter.block(0, 3, 3, 1) =-rot_c_to_w.transpose().cast<double>() * cam_w;
            converter(3, 3) = 1.;//Camera external parameters
            Eigen::Vector4d c4 = converter * w4;
            Eigen::Vector3d u3 = cam_f * c4;
            // 归⼀化像素坐标
            u3 /= u3(2, 0);
            cv::Point2d point_img(u3(0,0),u3(1,0));
            cv::circle(image,point_img,0.1*(i+1),cv::Scalar(255,255,255));
        }
    	writer.write(image);
    }
    cv::imshow("woekf",image);
    cv::waitKey(0);
    for(int i=1;i<=20;i++){
        
        cv::Mat A=getStructuringElement(cv::MORPH_ELLIPSE, cv::Point(3,3));
        morphologyEx(image,image,cv::MORPH_OPEN,A,cv::Point(-1,-1),1);
    	writer.write(image);
    }
    writer.release();
    return 0;
}