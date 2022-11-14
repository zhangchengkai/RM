#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Eigen/Dense"
#include "opencv2/core/eigen.hpp"

using namespace cv;
using namespace Eigen;
using namespace std;
int main(int argc,char** argv)
{
    namedWindow("show",WINDOW_NORMAL);
    resizeWindow("show",800,600);
    // read two images
    Mat img1 = imread("../9407.png");
    Mat img2 = imread("../9408.png");
    // using gray image to compute
    Mat gray1,gray2;
    cvtColor(img1,gray1,COLOR_BGR2GRAY);
    cvtColor(img2,gray2,COLOR_BGR2GRAY);
        //create orb detector
    Ptr<ORB> orb = ORB::create();
        // create the container of Key points
    vector<KeyPoint> feature_points1,feature_points2;
    // do Orient_FAST detect Keypoint
    orb->detect(gray1,feature_points1);
    orb->detect(gray2,feature_points2);
    // compute the descriptors
    Mat descriptor1,descriptor2;
    orb->compute(gray1,feature_points1,descriptor1);
    orb->compute(gray2,feature_points2,descriptor2);
        //do matching
    //create Matcher
    BFMatcher matcher(NORM_HAMMING); //O(N^2)
    vector<DMatch> pairs;
    matcher.match(descriptor1,descriptor2,pairs);
    printf("DMatch contains the matched points like (%d in img1,%d in img2) their distance is %.3f (in Hamming Norm).\n"
           ,pairs[0].queryIdx,pairs[0].trainIdx,pairs[0].distance);
        vector<DMatch> good;
    double min_dist = 100000;
    // compute the minimum of the distance
    for(const DMatch&m:pairs)
    {
        if(m.distance < min_dist) min_dist = m.distance;
    }
    // filter
    for(const DMatch&m:pairs)
    {
        if(m.distance < max(min_dist*2,30.))
        {
            good.push_back(m);
        }
    }
    Mat canvas;
    drawMatches(img1,feature_points1,img2,feature_points2,good,canvas);
    imwrite("../good_match.jpg",canvas);
    imshow("show",canvas);
    waitKey(0);
        // draw the keypoint
    drawKeypoints(img1,feature_points1,canvas,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    imwrite("../keypoints.jpg",canvas);
    imshow("show",canvas);
    waitKey(0);
    return 0;
}