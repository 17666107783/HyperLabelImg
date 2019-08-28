#pragma once
/*
	photoshopһЩ ͼ��Ч���Ĵ���
	ͼ���Ϸ�ʽ����͸��ͨ���Ļ��
	���Ȼ��-�ϲ�����Ȼ�ϵ��²㣬����lab�ռ�
	sunjunlishi
*/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
//photoshop������ͼ������ںϣ�4��ͨ������͸��ͨ��
 //Ϊʲô����opnecv��addWeight���Ǻǣ���Ϊ�����˲��У��������ô��˰�
//src�ϲ㣬dst�²㣬alpha�ϲ��͸����
void MixLayer(Mat& src,Mat& dst,float alpha = 1.0);

void MixLayerPos(Mat& src,Mat& dst,int posDstX,int posdstY,float alpha = 1.0);

void MixLayerDarkPos(Mat& src,Mat& dst,int posDstX,int posdstY,float alpha = 1.0);
//photoshop��ɫ���ģʽ 
void MatColorMix(Mat& src,Mat& dst);

//���ϲ����������²������
Mat MatMixByLight(Mat&src ,Mat& dst,float alpha = 1.0);

//��ӱ߽磬�߽���ɫ��ɫ
Mat MatMixAddEdge(Mat&src,Mat& dst);

void MixLayerPoint(Mat& src,Mat& dst,int& pos,float& alpha);