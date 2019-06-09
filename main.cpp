#include <iostream>

#include <opencv2/opencv.hpp>


#include "ImageSequence.hpp"
#include "PointExtraction.h"
#include "CDollarOneRecogniser.h"

void readTemplates(std::vector<GestureTemplate*>& templates)
{
	SampleGestures sg;
	GestureTemplate* T;

	T = new GestureTemplate;
	T->m_s_gestureName = "Circle";
	T->m_vp_points = sg.getGestureCircle();
	templates.push_back(T);

	T = new GestureTemplate;
	T->m_s_gestureName = "Rectangle";
	T->m_vp_points = sg.getGestureRectangle();
	templates.push_back(T);

	T = new GestureTemplate;
	T->m_s_gestureName = "Star";
	T->m_vp_points = sg.getGestureStar();
	templates.push_back(T);

	T = new GestureTemplate;
	T->m_s_gestureName = "Triangle";
	T->m_vp_points = sg.getGestureTriangle();
	templates.push_back(T);

	T = new GestureTemplate;
	T->m_s_gestureName = "LeftSquareBracket";
	T->m_vp_points = sg.getGestureLeftSquareBracket();
	templates.push_back(T);

	T = new GestureTemplate;
	T->m_s_gestureName = "RightSquareBracket";
	T->m_vp_points = sg.getGestureRightSquareBracket();
	templates.push_back(T);

	T = new GestureTemplate;
	T->m_s_gestureName = "LeftToRightLine";
	T->m_vp_points = sg.getGestureLeftToRightLine();
	templates.push_back(T);

	T = new GestureTemplate;
	T->m_s_gestureName = "RightToLeftLine";
	T->m_vp_points = sg.getGestureRightToLeftLine();
	templates.push_back(T);
}

cv::Mat refineDepthMap(cv::Mat depthImage, cv::Mat confidenceMap)
{
	cv::Mat depth_norm, conf_norm;
	cv::Mat conf_mask, confidentDepth;
	cv::normalize(confidenceMap, conf_norm, 0, 255, CV_MINMAX, CV_8U);
	cv::threshold(conf_norm, conf_mask, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3), cv::Point(1,1));
	cv::morphologyEx(conf_mask, conf_mask, cv::MORPH_OPEN, kernel, cv::Point(1,1));
	cv::morphologyEx(conf_mask, conf_mask, cv::MORPH_CLOSE, kernel, cv::Point(1,1));

	confidentDepth.setTo(UINT16_MAX);
	depthImage.copyTo(confidentDepth, conf_mask);

	for(int i = 0; i < confidentDepth.cols; i++)
	{
		for(int j = 0; j < confidentDepth.rows; j++)
		{
			if(confidentDepth.at<uint16_t>(j, i) == 0)
				confidentDepth.at<uint16_t>(j, i) = UINT16_MAX;
		}
	}

	return confidentDepth;
}

int main(int argc, char** argv)
{
	cv::Mat depthImage, confidenceMap, confidentDepth;

	CImageSequence is(std::string("dataset/ds536/rectangle_cw"), std::string("tif"));
	std::vector<cv::Mat> imagePair;
	std::vector<cv::Point2d> control_points;
	PointExtraction *pe = new PointExtraction();

	do {
		imagePair = is.getNextFrame();
		imagePair[0].copyTo(depthImage);
		imagePair[1].copyTo(confidenceMap);
		if(depthImage.empty())
			break;
		confidentDepth = refineDepthMap(depthImage, confidenceMap);
		cv::Point cp;
		if(pe->extractInterestingPoint(confidentDepth, cp))
		{
			control_points.push_back(cp);
		}
	} while(!depthImage.empty());

	CDollarOneRecogniser *recogniser = new CDollarOneRecogniser();
	std::vector<GestureTemplate*> templates;
	readTemplates(templates);
	float score;
	GestureTemplate *T = recogniser->recognise(control_points, templates, score);
	std::cout << "Recognised pattern : " << T->gestureName() << " with score : " << score << std::endl;
    return 0;
}
