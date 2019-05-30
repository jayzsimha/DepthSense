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

class GestureTemplate;

class CDollarOneRecogniser {
public:
	CDollarOneRecogniser();
	virtual ~CDollarOneRecogniser();
	std::vector<cv::Point> resample(std::vector<cv::Point> points, int n);
	float indicativeAngle(std::vector<cv::Point> points);
	std::vector<cv::Point> rotateBy(std::vector<cv::Point> points, float omega);
	std::vector<cv::Point> scaleTo(std::vector<cv::Point> points, int size);
	std::vector<cv::Point> translateTo(std::vector<cv::Point> points, cv::Point k);
	GestureTemplate* recognise(std::vector<cv::Point> points,
			std::vector<GestureTemplate*> templates, float& score);
	float distanceAtBestAngle(std::vector<cv::Point> points, GestureTemplate* T,
			float theta_a, float theta_b, float theta_d);
private:
	int pathLength(std::vector<cv::Point> points);
	float distanceAtAngle(std::vector<cv::Point> points, GestureTemplate* T, float theta);
	float pathDistance(std::vector<cv::Point> A, std::vector<cv::Point> B);
	cv::Point2f centroid(std::vector<cv::Point> points);
};

#endif /* CDOLLARONERECOGNISER_H_ */
