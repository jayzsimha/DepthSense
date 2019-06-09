/*
 * PointExtraction.cpp
 *
 *  Created on: 30-May-2019
 *      Author: jayasimha
 */

#include "PointExtraction.h"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

PointExtraction::PointExtraction() {
	// TODO Auto-generated constructor stub
	// create a tracker object
	m_tracker = cv::TrackerKCF::create();
	m_firstTime = true;
}

PointExtraction::~PointExtraction() {
	// TODO Auto-generated destructor stub
}

void PointExtraction::trackerInit(cv::Mat confidentDepthImage) {
	m_roi = cv::Rect(135, 65, 85, 85);
	//quit if ROI was not selected
	if(m_roi.width==0 || m_roi.height==0)
		return;
	// initialize the tracker
	m_tracker->init(confidentDepthImage, m_roi);
}

void PointExtraction::trackerUpdate(const cv::Mat confidentDepthImage) {
	bool found = m_tracker->update(confidentDepthImage, m_roi);
	if(found)
		std::cout << "Object Found" << std::endl;
	else
		std::cout << "Object Not Found" << std::endl;
}

float PointExtraction::estimateThreshold(cv::Mat confidentDepthMap, uint16_t bins)
{
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	cv::minMaxLoc(confidentDepthMap, &minVal, &maxVal, &minLoc, &maxLoc);
	return (minVal + 250);
}

void PointExtraction::threshold(cv::Mat& ImgSrc, cv::Mat& ImgDest, float thres)
{
    for (int r = 0; r < ImgSrc.rows; r++) { //	Threshold ImgSrcCopy using Thres & copy the crusial pixels from ImgSrc to ImgDest
        for (int c = 0; c < ImgSrc.cols; c++) {
            if (ImgSrc.at<uint16_t>(r, c) < (thres)) { // Constant threshold should be adjusted with respect to image depths
                ImgDest.at<uchar>(r, c) = 255;
            }
            else {
                ImgDest.at<uchar>(r, c) = 0;
            }
        }
    }
    return;
}


bool PointExtraction::extractInterestingPoint(cv::Mat confidentDepthMap, cv::Point& cp)
{
	float thresh = estimateThreshold(confidentDepthMap, 100);
	cv::Mat temp = cv::Mat::zeros(confidentDepthMap.size(), CV_8U);
	this->threshold(confidentDepthMap, temp, thresh);
	imshow("ThreshImage", temp);
	cv::waitKey(10);

	std::vector<std::vector<cv::Point> > contours;
	cv::Rect r;
	cv::findContours(temp, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	if(!contours[0].empty()) {
		m_roi = cv::boundingRect(contours[0]);
		cv::rectangle(confidentDepthMap, m_roi, cv::Scalar(0, 0, 0), 2);
		imshow("confidentDepthMap", confidentDepthMap);
		if(m_roi.width >= 5 && m_roi.height >= 5)
		{
			cp = cv::Point((m_roi.x + m_roi.width/2), m_roi.y + m_roi.height/2);
			return true;
		}
	}
	return false;
//	if(m_firstTime) {
//		trackerInit(confidentDepthMap);
//		m_firstTime = false;
//	}
//	else
//		trackerUpdate(confidentDepthMap);

}
