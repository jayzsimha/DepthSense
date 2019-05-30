/*
 * PointExtraction.h
 *
 *  Created on: 30-May-2019
 *      Author: jayasimha
 */

#ifndef POINTEXTRACTION_H_
#define POINTEXTRACTION_H_

#include <opencv2/opencv.hpp>

class PointExtraction {
public:
	PointExtraction();
	virtual ~PointExtraction();
	cv::Point extractInterestingPoint(cv::Mat confidentDepthMap);
};

#endif /* POINTEXTRACTION_H_ */
