#pragma once

/*
����  �����ؼ������

����:https://github.com/TadasBaltrusaitis/OpenFace

��Ȩ��

*/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#ifdef THIS_CLM_LANDMARK
#define DLL_EXPORT_CLMLANDMARK __declspec(dllexport)
#else
#define DLL_EXPORT_CLMLANDMARK
#endif

extern int dstH,dstS,dstV,dstH1,dstS1,dstV1;

extern bool gBLeftNeedUpdate;
extern bool gBRightNeedUpdate;
extern 	int mIndexIter;
extern cv::Mat_<double> updateOut3D;
extern cv::Vec6d   gparamGlobal;



bool RectifyPtsSeg(cv::Mat& labMat,cv::Mat& outSeg,bool bShow );
//�ҳ��ָ���
bool FindSegLine(cv::Mat& labMat,bool bShow );

bool RectifyPtsSegLeft(cv::Mat& labMat,cv::Mat& outSeg,bool bShow );
bool FindSegLineLeft(cv::Mat& labMat,bool bShow );

 void FilterTheShape(cv::Mat_<double>& outShape,cv::Vec6d params_global);


//��������У��
DLL_EXPORT_CLMLANDMARK void FaceFilterRotate(std::vector<cv::Point>& mainPts, cv::Mat& clmResult);

void FilterParaLocal(cv::Mat_<double> mean_shape,cv::Mat_<double> princ_comp ,cv::Mat_<double>& params_local);

