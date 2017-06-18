/*********************************************************************
 *	Copyright 1992-2008, ZheJiang HuaRay Technology Stock Co.Ltd.
 *						All Rights Reserved
 *	@file 	    		odebug.hpp
 *	@author				dzf
 *	@brief				
 *  
 *
 *	@date	        	2017:6:6 
 *	@version			1.00.1
 *	@todo				
**********************************************************************/
#ifndef __ODEBUG_H__
#define __ODEBUG_H__

#include "opencv.hpp"
#include <stdint.h>
#include <string.h>
#include <iostream>

using namespace cv;

/** ÑÕÉ«ÐÅÏ¢ */
typedef struct OCVColor
{
	Scalar red    = Scalar(0,0,255);                                            /**< {255,0,0} */
	Scalar orange = Scalar(0,165,255);                                          /**< {255,165,0} */
	Scalar yellow = Scalar(0,255,255);                                          /**< {255,255,0} */
	Scalar green  = Scalar(0,255,0);                                            /**< {0,255,0} */
	Scalar cyan   = Scalar(255,255,0);                                          /**< {0,255,255} */

	Scalar blue   = Scalar(255,0,0);                                            /**< {0,0,255} */
	Scalar purple = Scalar(128,0,128);                                          /**< {128,0,128} */
	Scalar white  = Scalar(255,255,255);                                        /**< {255,255,255} */
	Scalar sliver = Scalar(192,192,192);                                        /**< {192,192,192} */
	Scalar gray  = Scalar(127,127,127);                                         /**< {127,127,127} */

	Scalar pink        = Scalar(203,192,255);                                   /**< {255,192,203} */
	Scalar springgreen = Scalar(127,255,0);                                     /**< {0,255,127} */
	Scalar darkblue    = Scalar(139,0,0);                                       /**< {0,0,139} */
	Scalar deepskyblue = Scalar(255,191,0);                                     /**< {0,191,255} */

	Scalar bisque = Scalar(196,228,255);                                        /**< {255,228,196} */
	Scalar tan    = Scalar(140,180,210);                                        /**< {210,180,140} */
	Scalar tomato = Scalar(71,99.255);                                          /**< {255,99,71} */
	Scalar black  = Scalar(0,0,0);                                              /**< {0,0,0} */
} OCVColor;

template <typename T>
static int32_t ocv_type()
{
	int32_t ocvt = -1;
	const std::string type = typeid(T).name();
	const std::string type_int8 = typeid(int8_t).name();
	const std::string type_uint8 = typeid(uint8_t).name();
	const std::string type_int16 = typeid(int16_t).name();
	const std::string type_uint16 = typeid(uint16_t).name();
	const std::string type_int32 = typeid(int32_t).name();
	const std::string type_uint32 = typeid(uint32_t).name();
	const std::string type_float = typeid(float).name();
	const std::string type_double = typeid(double).name();

	if (type == type_int8)
	{
		ocvt = CV_8SC(3);
	}else if (type == type_uint8)
	{
		ocvt = CV_8UC(3);
	}else if (type == type_int16)
	{
		ocvt = CV_16SC(3);

	}else if (type == type_uint16)
	{
		ocvt = CV_16UC(3);

	}else if (type == type_uint32 || type == type_int32)
	{
		ocvt = CV_32SC(3);

	}else if (type == type_float)
	{
		ocvt = CV_32FC(3);
	}
	else if (type == type_double)
	{
		ocvt = CV_64FC(3);
	}
	else
	{
		ocvt = -1;
	}

	return ocvt;
}

template <typename T>
static void ocv_buf2mat(cv::Size size, T *pBuf, cv::Mat& mat)
{
	int32_t type = ocv_type<T>();
	if (type < 0)
	{
		std::cout << "not support the type in opencv..." << std::endl;
	}

	mat.release();
	mat.create(size.height,size.width, type);

	for (int32_t y = 0; y < size.height; y++)
	{
		T *ptr = mat.ptr<T>(y);
		for (int32_t x = 0; x < size.width; x++)
		{
			T pix = pBuf[y*size.width + x];
			ptr[x * 3] = pix;
			ptr[x * 3+1] = pix;
			ptr[x * 3+2] = pix;
		}
	}
}

static uint8_t* ocv_img2gbuf(cv::Mat &mat, uint8_t *pBuf = NULL)
{	
	if (mat.depth() != CV_8U)
	{
		std::cout << "not uchar matrix" << std::endl;
	}

	if (mat.channels() == 3 || mat.channels() == 4)
	{
		cv::Mat dst(mat.rows, mat.cols, CV_8U);
		cv::cvtColor(mat, dst,CV_BGR2GRAY, 0);
		mat.release();

		mat = dst.clone();
	}

	if (pBuf == NULL) pBuf = new uint8_t[mat.rows*mat.cols];
	for (int32_t y = 0; y < mat.rows; y++)
	{
		for (int32_t x = 0; x < mat.cols; x++)
		{
			pBuf[y*mat.cols + x] = mat.at<uint8_t>(y, x);
		}
	}

	return pBuf;
}

template <typename T>
static void ocv_setpix(cv::Mat& mat,const cv::Point& pt, const cv::Scalar& color)
{
	int32_t cha;
	T *ptr = mat.ptr<T>(pt.y);
	cha = mat.channels();
	if (cha == 1)
	{
		ptr[pt.x] = static_cast<T>(color.val[0]);
	}else
	{
		ptr += cha * pt.x;
		for (int32_t i = 0; i < cha; i++)
		{
			*ptr++ = static_cast<T>(color.val[i]);
		}
	}
}

static void ocv_polygon(cv::Mat& mat, int32_t nPoint, const cv::Point *pPoint, const cv::Scalar& color, int32_t thick = 1)
{
	if (nPoint > 2 && mat.depth() == CV_8U)
	{
		cv::circle(mat, pPoint[0], thick + 1, color, -1, CV_AA, 0);

		for (int32_t i = 0; i < nPoint; i++)
		{
			int32_t j = (i + 1) % nPoint;
			cv::line(mat, pPoint[i], pPoint[j], color, thick, CV_AA, 0);
		}
	}
}

static void ocv_nrect(cv::Mat& mat, const cv::Rect& rect, float rotate, float skew, const cv::Scalar& color, int32_t thick = 1)
{
	float a, b, c, d;
	cv::Point rpt[4];
	cv::Point cpt;
	if (mat.depth() == CV_8U)
	{
		cpt.x = (rect.tl().x + rect.br().x) >> 1;
		cpt.y = (rect.tl().y + rect.br().y) >> 1;

		a = cosf(rotate);
		b = -sinf(rotate + skew);
		c = sinf(rotate);
		d = cosf(rotate + skew);

		rpt[0].x = (int32_t)((rect.tl().x - cpt.x) * a + (rect.tl().y - cpt.y) * b + cpt.x);
		rpt[0].y = (int32_t)((rect.tl().x - cpt.x) * c + (rect.tl().y - cpt.y) * d + cpt.y);

		rpt[1].x = (int32_t)((rect.br().x - cpt.x) * a + (rect.tl().y - cpt.y) * b + cpt.x);
		rpt[1].y = (int32_t)((rect.br().x - cpt.x) * c + (rect.tl().y - cpt.y) * d + cpt.y);

		rpt[2].x = (int32_t)((rect.br().x - cpt.x) * a + (rect.br().y - cpt.y) * b + cpt.x);
		rpt[2].y = (int32_t)((rect.br().x - cpt.x) * c + (rect.br().y - cpt.y) * d + cpt.y);

		rpt[3].x = (int32_t)((rect.tl().x - cpt.x) * a + (rect.br().y - cpt.y) * b + cpt.x);
		rpt[3].y = (int32_t)((rect.tl().x - cpt.x) * c + (rect.br().y - cpt.y) * d + cpt.y);
		
		ocv_polygon(mat, 4, rpt, color, 1);
	}
}

static void ocv_linecoef(cv::Mat& mat, float a, float b, float c, const cv::Scalar color, int32_t thick = 1)
{
	Point temp[2];
	Point head, tail;
	float x, y;

	if (fabsf(a) > fabsf(b))
	{
		head.x = -c / a;
		head.y = 0;

		
		tail.y = mat.rows - 1;
		tail.x = -(b * tail.y + c) / a;
	}
	else
	{
		head.x = 0;
		head.y = -c / b;

		tail.x = mat.cols - 1;
		tail.y = -(a * tail.x + c) / b;
	}


	line(mat, head, tail, color, thick, CV_AA);

}

#endif // __ODEBUG_H__