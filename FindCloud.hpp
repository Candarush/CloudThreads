#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <string>

using namespace std;
using namespace cv;

class CloudFinder
{
    VideoCapture cap;
    int gridX, gridY, numthreads;
    Mat compareMat;
    vector<float**> results;
    void CompareHistograms(CloudFinder cl, int gridSizeX, int gridSizeY, Mat* src, Mat compareMat, vector<float**> results, int* threadMarker, mutex* mut);
    
    public:
    float GetCloudness(Mat* inHist , Mat* inImage, int curCellX, int curCellY, int cellSizeX, int cellSizeY);
    CloudFinder(VideoCapture icap, int igridX, int igridY, int inumthreads, Mat icompareMat);
    void ProcessCloudVideo();
    string GetData();
};
