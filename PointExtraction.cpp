/*
 * PointExtraction.cpp
 *
 *  Created on: 30-May-2019
 *      Author: jayasimha
 */

#include "PointExtraction.h"

PointExtraction::PointExtraction() {
	// TODO Auto-generated constructor stub

}

PointExtraction::~PointExtraction() {
	// TODO Auto-generated destructor stub
}

cv::Point PointExtraction::extractInterestingPoint(cv::Mat confidentDepthMap)
{
	double minVal, maxVal;
	cv::Point_<int> minLoc, maxLoc;
	cv::minMaxLoc(confidentDepthMap, &minVal, &maxVal, &minLoc, &maxLoc);
	return minLoc;
}
