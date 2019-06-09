/*
 * PointExtraction.h
 *
 *  Created on: 30-May-2019
 *      Author: jayasimha
 */

#ifndef POINTEXTRACTION_H_
#define POINTEXTRACTION_H_

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

class PointExtraction {
public:
	PointExtraction();
	virtual ~PointExtraction();
	bool extractInterestingPoint(cv::Mat confidentDepthMap, cv::Point& cp);
private:
	void trackerInit(cv::Mat confidentDepthImage);
	void trackerUpdate(const cv::Mat confidentDepthImage);
	float estimateThreshold(cv::Mat confidentDepthMap, uint16_t bins);
	void threshold(cv::Mat& ImgSrc, cv::Mat& ImgDest, float thres);
private:
	cv::Ptr<cv::Tracker> m_tracker;
	cv::Rect2d m_roi;
	bool m_firstTime;
};

#endif /* POINTEXTRACTION_H_ */
