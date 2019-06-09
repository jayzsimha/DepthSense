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

	CImageSequence(std::string dirPath, std::string ext)
	{
		m_dirPath = dirPath;
		m_ext = ext;
		m_count = 0;
		char df[200];
		sprintf(df, "%s/%06d_depth.tiff", m_dirPath.c_str(), m_count);
		cv::Mat temp = cv::imread(df, -1);
		m_width = temp.cols;
		m_height = temp.rows;
	}
	int open(std::string dirPath, std::string ext);
	std::vector<cv::Mat> getNextFrame();
	size_t width() {return m_width;}
	size_t height() {return m_height;}
public:
	std::string m_dirPath;
	std::string m_ext;
private:
	int m_count;
	size_t m_width;
	size_t m_height;
};

int CImageSequence::open(std::string dirPath, std::string ext)
{
	m_dirPath = dirPath;
	m_ext = ext;
	char df[200];
	sprintf(df, "%s/%06d_depth.%s", m_dirPath.c_str(), m_count, m_ext.c_str());
	cv::Mat temp = cv::imread(df, -1);
	m_width = temp.cols;
	m_height = temp.rows;
	return 0;
}

std::vector<cv::Mat> CImageSequence::getNextFrame()
{
	std::vector<cv::Mat> imagePair(2);
	char df[200], cf[200];
	sprintf(df, "%s/%06d_depth.%s", m_dirPath.c_str(), m_count, m_ext.c_str());
	sprintf(cf, "%s/%06d_confidence.%s", m_dirPath.c_str(), m_count, m_ext.c_str());
	std::cout << df << std::endl;
	imagePair[0] = cv::imread(df, -1);
	imagePair[1] = cv::imread(cf, -1);
	m_count++;
	return imagePair;
}

#endif /* IMAGESEQUENCE_HPP_ */
