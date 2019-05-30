#include <iostream>

#include <opencv2/opencv.hpp>

#include "ImageSequence.hpp"
#include "PointExtraction.h"

int main(int argc, char** argv)
{
	cv::Mat depthImage, confidenceMap;
	cv::Mat depth_norm, conf_norm;
	cv::Mat conf_mask, confidentDepth;

	CImageSequence is(std::string("dataset/ds536/rectangle_ccw"));
	std::vector<cv::Mat> imagePair;
	cv::Mat control_points;
	PointExtraction *pe = new PointExtraction();

	do {
		imagePair = is.getNextFrame();
		imagePair[0].copyTo(depthImage);
		imagePair[1].copyTo(confidenceMap);
		if(depthImage.empty())
			break;

		cv::normalize(confidenceMap, conf_norm, 0, 255, CV_MINMAX, CV_8U);
		cv::threshold(conf_norm, conf_mask, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);
		depthImage.copyTo(confidentDepth, conf_mask);

		cv::Point_<int> cp = pe->extractInterestingPoint(confidentDepth);
		control_points.at(cp) = 255;

		imshow("depthImage", depthImage);
		imshow("confidenceMap", conf_mask);
		imshow("confidentDepth", confidentDepth);
		cv::waitKey(100);
	} while(!depthImage.empty());
    return 0;
}
