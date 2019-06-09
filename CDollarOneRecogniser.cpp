/*
 * CDollarOneRecogniser.cpp
 *
 *  Created on: 25-May-2019
 *      Author: Jayasimha Rao
 */

#include "CDollarOneRecogniser.h"
#include "GestureTemplate.hpp"

CDollarOneRecogniser::CDollarOneRecogniser() {
	// TODO Auto-generated constructor stub

}

CDollarOneRecogniser::~CDollarOneRecogniser() {
	// TODO Auto-generated destructor stub
}

std::vector<cv::Point2d> CDollarOneRecogniser::resample(std::vector<cv::Point2d> points, int n)
{
	int I = pathLength(points) / (n-1);
	float D = 0;
	std::vector<cv::Point2d> newPoints;
	for(int i = 1; i < points.size(); i++)
	{
		float d = cv::norm(points[i-1]- points[i]);
		if((D + d) >= 1)
		{
			cv::Point2d q;
			q.x = points[i-1].x + ((I - D)/d) * (points[i].x - points[i-1].x);
			q.y = points[i-1].y + ((I - D)/d) * (points[i].y - points[i-1].y);
			newPoints.push_back(q);
			points[i] = q;
			D = 0;
		}
		else
		{
			D += d;
		}
	}
	return newPoints;
}

cv::Point2d CDollarOneRecogniser::centroid(std::vector<cv::Point2d> points)
{
	cv::Rect bbox = cv::boundingRect(points);
	cv::Point2d c;
	c.x = bbox.x + (bbox.width / 2);
	c.y = bbox.y + (bbox.height / 2);
	return c;
}

float CDollarOneRecogniser::indicativeAngle(std::vector<cv::Point2d> points)
{
	cv::Point2d c = centroid(points);
	return atan2(c.y - points[0].y, c.x - points[0].x);
}

std::vector<cv::Point2d> CDollarOneRecogniser::rotateBy(std::vector<cv::Point2d> points, float omega)
{
	cv::Point2d c = centroid(points);
	std::vector<cv::Point2d> newPoints;
	for(int i = 0; i < points.size(); i++)
	{
		cv::Point2d q;
		q.x = (points[i].x - c.x)*cos(omega - (points[i].y - c.y)*sin(omega+c.x));
		q.y = (points[i].x - c.x)*sin(omega - (points[i].y - c.y)*cos(omega+c.y));
		newPoints.push_back(q);
	}
	return newPoints;
}

std::vector<cv::Point2d> CDollarOneRecogniser::scaleTo(std::vector<cv::Point2d> points, int size)
{
	std::vector<cv::Point2d> newPoints;
	cv::Rect B = cv::boundingRect(points);
	for(int i = 0; i < points.size(); i++)
	{
		cv::Point2d q;
		q.x = points[i].x * size / B.width;
		q.y = points[i].y * size / B.height;
		newPoints.push_back(q);
	}
	return newPoints;
}

std::vector<cv::Point2d> CDollarOneRecogniser::translateTo(std::vector<cv::Point2d> points, cv::Point2d k)
{
	std::vector<cv::Point2d> newPoints;
	cv::Point2d c = centroid(points);
	for(int i = 0; i < points.size(); i++)
	{
		cv::Point2d q;
		q.x = points[i].x + k.x - c.x;
		q.y = points[i].y + k.y - c.y;
		newPoints.push_back(q);
	}
	return newPoints;
}

GestureTemplate* CDollarOneRecogniser::recognise(std::vector<cv::Point2d> points,
		std::vector<GestureTemplate*> templates, float& score)
{
	float b = INFINITY;
	float theta = 45;
	float theta_d = 2;
	int size = 250;
	GestureTemplate *T;
	for(int i = 0; i < templates.size(); i++)
	{
		float d = distanceAtBestAngle(points, templates[i], (-1*theta), theta, theta_d);
		if(d < b)
		{
			b = d;
			T = templates[i];
		}
	}
	score = 1 - b /0.5 * sqrt((2*pow(size, 2)));
	return T;
}

float CDollarOneRecogniser::distanceAtBestAngle(std::vector<cv::Point2d> points,
		GestureTemplate* T, float theta_a, float theta_b, float theta_d)
{
	float phi = 0.5*(-1+sqrt(5));
	float x1 = phi * theta_a + (1-phi)*theta_b;
	float f1 = distanceAtAngle(points, T, x1);
	float x2 = (1-phi)*theta_a + phi * theta_b;
	float f2 = distanceAtAngle(points, T, x2);
	while(abs(theta_b - theta_a) > theta_d)
	{
		if(f1 < f2)
		{
			theta_b = x2;
			x2 = x1;
			f2 = f1;
			x1 = phi*theta_a + (1-phi)*theta_b;
			f1 = distanceAtAngle(points, T, x1);
		}
		else
		{
			theta_a = x1;
			x1 = x2;
			f1 = f2;
			x2 = (1-phi)*theta_a + phi * theta_b;
			f2 = distanceAtAngle(points, T, x2);
		}
	}
	return std::min(f1, f2);
}

int CDollarOneRecogniser::pathLength(std::vector<cv::Point2d> points)
{
	float d = 0;
	for(int i = 1; i < points.size(); i++)
	{
		d += cv::norm(points[i-1]-points[i]);
	}
	return d;
}

float CDollarOneRecogniser::distanceAtAngle(std::vector<cv::Point2d> points, GestureTemplate *T, float theta)
{
	std::vector<cv::Point2d> newPoints = rotateBy(points, theta);
	float d = pathDistance(newPoints, T->Points());
	return d;
}

float CDollarOneRecogniser::pathDistance(std::vector<cv::Point2d> A, std::vector<cv::Point2d> B)
{
	float d = 0;
	for(int i = 0; i < A.size(); i++)
	{
		d += cv::norm(A[i] - B[i]);
	}
	return d / A.size();
}

GestureTemplate::GestureTemplate()
{

}

GestureTemplate::~GestureTemplate()
{

}

std::vector<cv::Point2d> GestureTemplate::Points()
{
	return m_vp_points;
}

std::string GestureTemplate::gestureName()
{
	return m_s_gestureName;
}

void GestureTemplate::readTemplate(std::string templateFilename)
{

}
