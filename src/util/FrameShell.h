/**
* This file is part of DSO.
* 
* Copyright 2016 Technical University of Munich and Intel.
* Developed by Jakob Engel <engelj at in dot tum dot de>,
* for more information see <http://vision.in.tum.de/dso>.
* If you use this code, please cite the respective publications as
* listed on the above website.
*
* DSO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DSO. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "util/NumType.h"
#include "algorithm"
#include "opencv2/highgui/highgui.hpp"
namespace dso
{

	class FrameShell
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
		//? 两个ID有啥不同, 这两个i id 是从0开始的帧的id, 理解为id_true, incoming_id是主函数中处理过的dataset中图像的id,(可能会受倍率的影响而不同)
		int id;			  // INTERNAL ID, starting at zero.
		int incoming_id;  // ID passed into DSO
		double timestamp; // timestamp passed into DSO.

		// set once after tracking
		SE3 camToTrackingRef;
		FrameShell *trackingRef;

		// constantly adapted.
		SE3 camToWorld; // Write: TRACKING, while frame is still fresh; MAPPING: only when locked [shellPoseMutex].
		AffLight aff_g2l;
		bool poseValid;

		// statisitcs
		int statistics_outlierResOnThis;
		int statistics_goodResOnThis;
		int marginalizedAt; //!< 被边缘化时最新关键帧的id
		double movedByOpt;	//!< 边缘化时得到的距离线性化点位姿的6维模

		cv::Mat basecolorImg_raw;
		cv::Mat normalImg_raw;
		cv::Mat metallicImg_raw;
		cv::Mat roughnessImg_raw;

		inline FrameShell()
		{
			id = 0;
			poseValid = true;
			camToWorld = SE3();
			timestamp = 0;
			marginalizedAt = -1;
			movedByOpt = 0;
			statistics_outlierResOnThis = statistics_goodResOnThis = 0;
			trackingRef = 0;
			camToTrackingRef = SE3();
		}
	};

}
