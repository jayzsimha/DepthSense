/*
 * GestureTemplate.hpp
 *
 *  Created on: 25-May-2019
 *      Author: jayasimha
 */

#ifndef GESTURETEMPLATE_HPP_
#define GESTURETEMPLATE_HPP_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class GestureTemplate
{
public:
	GestureTemplate();
	virtual ~GestureTemplate();
	std::vector<cv::Point> Points();
	std::string gestureName();
	void readTemplate(std::string templateFilename);
private:
	std::string m_s_gestureName;
	std::vector<cv::Point> m_vp_points;
};

GestureTemplate::GestureTemplate()
{

}

GestureTemplate::~GestureTemplate()
{

}

std::vector<cv::Point> GestureTemplate::Points()
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


#endif /* GESTURETEMPLATE_HPP_ */
