#include <iostream>
#include <cmath>
#include <utility>
#include<opencv2/opencv.hpp>

void recognize(cv::Mat &src);
std::string point_to_str(const cv::Point &p, std::string name);

int main() {
    cv::VideoCapture capture("../IMG_8511.mp4");
    long num = capture.get(cv::CAP_PROP_FRAME_COUNT);
    long n = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    long m = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    // printf("%ld %ld\n",n,m);return 0;
    // 568*320
    cv::Size size = cv::Size(n, m);
    double fps = capture.get(cv::CAP_PROP_FPS);
    cv::VideoWriter output = cv::VideoWriter("../wtf.mp4",cv::VideoWriter::fourcc('P', 'I', 'M', '1'),fps,size);

    cv::Mat frame;
    while (true) {
        capture >> frame;
        if (frame.empty()) break;
        imshow("origin-frame", frame);
        recognize(frame);
        imshow("recognized-frame", frame);
        cv::waitKey(100);
        output<<frame;
    }

    output.release();
    capture.release();
    return 0;
}
inline double sqr(double x){return x*x;}
inline bool check_dis(cv::Point2f A,cv::Point2f B){return std::sqrt(sqr(A.x-B.x)+sqr(A.y-B.y))<150;}
double ss[1001];
    cv::Point2f pp[1001];
    int numP=0;
inline int cmp(int x,int y){return ss[x]>ss[y];}
void recognize(cv::Mat &src) {
    cv::Mat grey;
    cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);
    cv::Mat binary_img;
    cv::threshold(grey, binary_img, 100, 200, cv::THRESH_BINARY);
    cv::Mat A=getStructuringElement(cv::MORPH_ELLIPSE, cv::Point(3,3));
    morphologyEx(binary_img,binary_img,cv::MORPH_OPEN,A,cv::Point(-1,-1),1);
    
    cv::Mat canny_result;
    cv::Canny(binary_img, canny_result, 50, 100, 3);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierachy;
    cv::findContours(canny_result, contours, hierachy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    int p[1001];
    int len=contours.size();
    for(int i=0;i<len;i++){
        p[i]=i;
		cv::RotatedRect rotatedrect = cv::minAreaRect(cv::Mat(contours[p[i]]));
        double nn=rotatedrect.size.width,mm=rotatedrect.size.height;
		cv::Point2f ps[4];
		rotatedrect.points(ps);
        if(ps[0].x<200&&ps[0].y>200) ss[i]=0;
        else ss[i]=nn*mm;
	}
    std::sort(p,p+len,cmp);
    for(int i=0;i<len&&i<2;i++){
		cv::RotatedRect rotatedrect = cv::minAreaRect(cv::Mat(contours[p[i]]));
        double nn=rotatedrect.size.width,mm=rotatedrect.size.height;
        if(nn>mm) std::swap(nn,mm);
        if(nn*mm<120) break;
        if(i&&mm/nn<10) break;
		// 存储旋转矩形的四个点
		cv::Point2f ps[4];
		rotatedrect.points(ps);
//        printf("(%lf , %lf) ",ps[0].x,ps[0].y);
		std::vector<std::vector<cv::Point>> tmpContours;    // 创建一个InputArrayOfArrays 类型的点集
		std::vector<cv::Point> contour;
		for (int i = 0; i != 4; ++i) {
			contour.emplace_back(cv::Point2i(ps[i]));
		}
		// 插入到轮廓容器中
		tmpContours.insert(tmpContours.end(), contour);
		// 绘制轮廓，也就是绘制旋转矩形
		drawContours(src, tmpContours, -1, cv::Scalar(0,0,255), 1, 16);  // 填充mask
        circle(src, rotatedrect.center, 4, (0,0,255), -1);
        int is_matched=0;
        for(int i=numP;i>0&&i>numP-2;i--){
            if(check_dis(pp[i],rotatedrect.center)){
                is_matched=i;
                break;
            }
        }
        if(!is_matched) is_matched=++numP;
        pp[is_matched]=rotatedrect.center;
        cv::putText(src, std::to_string(is_matched), {(int)rotatedrect.center.x-50,(int)rotatedrect.center.y+20}, cv::FONT_HERSHEY_TRIPLEX, 0.6,{0, 0, 255}, 1, false);
    }

}
std::string point_to_str(const cv::Point &p, std::string name) {
    std::string res = name + "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ")";
    return res;
}
