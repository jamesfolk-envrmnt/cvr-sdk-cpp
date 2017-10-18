
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "stdafx.h"
#include "CognitiveVRAnalytics.h"


#if defined(_MSC_VER)
//  Microsoft 
#ifdef COGNITIVEVRANALYTICS_EXPORTS  
#define COGNITIVEVRANALYTICS_API __declspec(dllexport)
#else  
#define COGNITIVEVRANALYTICS_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
//  GCC
#ifdef COGNITIVEVRANALYTICS_EXPORTS  
#define COGNITIVEVRANALYTICS_API __attribute__((visibility("default")))
#else  
#define COGNITIVEVRANALYTICS_API
#endif
#else
//  do nothing and hope for the best?
#define COGNITIVEVRANALYTICS_EXPORTS
#pragma warning Unknown dynamic link import/export semantics.
#endif

namespace cognitive
{
class CognitiveVRAnalyticsCore;

class COGNITIVEVRANALYTICS_API GazeTracker
{
private:
	::std::shared_ptr<CognitiveVRAnalyticsCore> cvr = nullptr;
	long lastIntervalTime = -1;
	int jsonPart = 1;
	//int gazeCount = 0;
	
	//set from config
	float PlayerSnapshotInterval = 0.1f;
	::std::string HMDType = "";

public:

	nlohmann::json BatchedGazeSE = nlohmann::json();

	GazeTracker(::std::shared_ptr<CognitiveVRAnalyticsCore> cog);
	void SetInterval(float interval);
	void SetHMDType(::std::string hmdtype);

	void RecordGaze(::std::vector<float> &Position, ::std::vector<float> &Rotation, ::std::vector<float> &Gaze, int objectId = -1);
	void RecordGaze(::std::vector<float> &Position, ::std::vector<float> &Rotation);

	void SendData();
	void EndSession();
};
}