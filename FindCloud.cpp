#include "FindCloud.hpp"
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

CloudFinder::CloudFinder(VideoCapture icap, int igridX, int igridY, int inumthreads, Mat icompareMat)
    {
        cap = icap;
        gridX = igridX;
        gridY = igridY;
        numthreads = inumthreads;
        compareMat = icompareMat;
    }
    
    float CloudFinder::GetCloudness(Mat* inHist , Mat* inImage, int curCellX, int curCellY, int cellSizeX, int cellSizeY)
    {
        Mat section = (*inImage)(Rect(curCellX*cellSizeX, curCellY*cellSizeY, cellSizeX, cellSizeY));
        Mat greyMat;
        Mat rgbchannel[3];
        split( section, rgbchannel );
        greyMat = rgbchannel[2];
        Mat greyHist;
        int histSize = 256;
        float range[] = { 0, 256 };
        const float* histRange = { range };
        bool uniform = true, accumulate = false;
        Mat hist = Mat(Size(256,256),CV_8UC3);
        calcHist( &greyMat, 1, 0, Mat(), greyHist, 1, &histSize, &histRange, uniform, accumulate );
        normalize(greyHist, greyHist, 0, 256, NORM_MINMAX, -1, Mat() );
        
        MatND secHist = MatND(greyHist);
        MatND comHist = MatND(*inHist);
        
        float cloudness = compareHist(secHist, comHist, HISTCMP_BHATTACHARYYA);
        return cloudness;
        
    }

void CloudFinder::CompareHistograms(int gridSizeX, int gridSizeY, Mat* src, Mat compareMat, vector<float**> results, int* threadMarker, mutex* mut)
{
    if( src->empty() )
    {
        cout << "Картинка не найдена!" << endl;
        return;
    }
    if( compareMat.empty() )
    {
        cout << "Картинка не найдена!" << endl;
        return;
    }
    Mat greyMat;
    Mat rgbchannel[3];
    split( compareMat, rgbchannel );
    greyMat = rgbchannel[2];
    Mat greyHist;
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true, accumulate = false;
    Mat hist = Mat(Size(256,256),CV_8UC3);
    calcHist( &greyMat, 1, 0, Mat(), greyHist, 1, &histSize, &histRange, uniform, accumulate );
    normalize(greyHist, greyHist, 0, 256, NORM_MINMAX, -1, Mat() );
    
    Mat resultImage = Mat(src->clone());
    int imageSizeX = src->cols;
    int imageSizeY = src->rows;
    cout << "Размер фото: " << imageSizeX << " " << imageSizeY << endl;

    int cellSizeX = imageSizeX/gridSizeY;
    int cellSizeY = imageSizeY/gridSizeX;
    
    int i,j;
    float** output = new float*[gridSizeY];
    for (i=0;i<gridSizeY;i++)
    {
        output[i] = new float[gridSizeX];
    }
    
    for (i=0;i<gridSizeX;i++)
    {
        for (j=0;j<gridSizeY;j++)
        {
            output[i][j] = GetCloudness(&greyHist,src,i,j,cellSizeY,cellSizeX);
        }
    }
    
    mut->lock();
    results.push_back(output);
    (*threadMarker)--;
    delete src;
    mut->unlock();
}
            
    string CloudFinder::GetData()
    {
        string output = "";
        float cloudness;
                int i,j,k;
        for (k=0;i<output.size();k++)
        {
            for (i=0;i<gridX;i++)
            {
                for (j=0;j<gridY;j++)
                {
                    cloudness+=results[k][i][j];
                }
            }
            output.append(to_string(cloudness)+" ");
            cloudness = 0;
        }
        return output;
    }

    void CloudFinder::ProcessCloudVideo()
    {
        
        int threadsWorking = 0;
        
        if(!cap.isOpened())
        {
            cout << "Ошибка при чтении видеопотока." << endl;
            return;
        }
        
        Mat frameMat;
        Mat* matCopy;

        vector<thread> activeThreads;
        mutex mut;
        
        while (!frameMat.empty())
        {
            while (threadsWorking >= numthreads)
            {
                // ждем и не вызываем потоки
            }
            cap >> frameMat;
            Mat matCopy(frameMat);
            threadsWorking++;
            activeThreads.push_back(thread(&CloudFinder::CompareHistograms, this, gridX, gridY, &frameMat, compareMat, results, threadsWorking, &mut));
        }
    }

