/*
 * ImageSequence.hpp
 *
 *  Created on: 26-May-2019
 *      Author: jayasimha
 */

#ifndef IMAGESEQUENCE_HPP_
#define IMAGESEQUENCE_HPP_

#include <string>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <cstdio>

class CImageSequence
{
public:
	CImageSequence(){m_count = 0;}

	CImageSequence(std::string dirPath)
	{
		m_dirPath = dirPath;
		m_count = 0;
	}
	int open(std::string dirPath);
	std::vector<cv::Mat> getNextFrame();
public:
	std::string m_dirPath;
private:
	int m_count;
};

int CImageSequence::open(std::string dirPath)
{
	m_dirPath = dirPath;
	return 0;
}

std::vector<cv::Mat> CImageSequence::getNextFrame()
{
	std::vector<cv::Mat> imagePair(2);
	char df[200], cf[200];
	sprintf(df, "%s/%06d_depth.tif", m_dirPath.c_str(), m_count);
	sprintf(cf, "%s/%06d_confidence.tif", m_dirPath.c_str(), m_count);
	std::cout << df << std::endl;
	imagePair[0] = cv::imread(df, -1);
	imagePair[1] = cv::imread(cf, -1);
	m_count++;
	return imagePair;
}

#endif /* IMAGESEQUENCE_HPP_ */
