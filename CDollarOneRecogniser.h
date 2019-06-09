/*
 * CDollarOneRecogniser.h
 *
 *  Created on: 25-May-2019
 *      Author: Jayasimha Rao
 */

#ifndef CDOLLARONERECOGNISER_H_
#define CDOLLARONERECOGNISER_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include "GestureTemplate.hpp"

class CDollarOneRecogniser {
public:
	CDollarOneRecogniser();
	virtual ~CDollarOneRecogniser();
	std::vector<cv::Point2d> resample(std::vector<cv::Point2d> points, int n);
	float indicativeAngle(std::vector<cv::Point2d> points);
	std::vector<cv::Point2d> rotateBy(std::vector<cv::Point2d> points, float omega);
	std::vector<cv::Point2d> scaleTo(std::vector<cv::Point2d> points, int size);
	std::vector<cv::Point2d> translateTo(std::vector<cv::Point2d> points, cv::Point2d k);
	GestureTemplate* recognise(std::vector<cv::Point2d> points,
			std::vector<GestureTemplate*> templates, float& score);
	float distanceAtBestAngle(std::vector<cv::Point2d> points, GestureTemplate* T,
			float theta_a, float theta_b, float theta_d);
private:
	int pathLength(std::vector<cv::Point2d> points);
	float distanceAtAngle(std::vector<cv::Point2d> points, GestureTemplate* T, float theta);
	float pathDistance(std::vector<cv::Point2d> A, std::vector<cv::Point2d> B);
	cv::Point2d centroid(std::vector<cv::Point2d> points);
};

#endif /* CDOLLARONERECOGNISER_H_ */
