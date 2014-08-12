//============================================================================
// Name        : IdeaAudioEngineTest.cpp
// Author      : Robel Sharma
// Version     :
// Copyright   : BSD
// Description : Test Application
//============================================================================

#include <iostream>
#include <stdio.h>
#include "IdeaAudioEngineTest.h"

using namespace std;

#define AUDIO_CALLBACK_SET

IdeaAudioEngineTest::IdeaAudioEngineTest()
{
	m_pAudioEngine = new AudioEngine();
	
}

IdeaAudioEngineTest::~IdeaAudioEngineTest()
{
	m_pAudioEngine->CloseCapture();
	m_pAudioEngine->ClosePlayback();
}

void IdeaAudioEngineTest::StartAudioTest()
{
	std::vector<DeviceInfo> Device;
	int iRet = m_pAudioEngine->AvailableAudioDevices(true, Device);
	cout<<"Number of device = "<<iRet<<endl;

	if(iRet == 0)return;
	for(int i = 0; i < (int)Device.size(); i++)
	{
		printf("%s %s\n", Device[i].m_sDeviceName.c_str(), Device[i].m_sDeviceID.c_str());
	}

	printf("OpenCapture Return : %d\n", m_pAudioEngine->OpenCaptureDevice(NULL/*(char*)Device[0].m_sDeviceName.c_str()*/, 0, 1, 0, 3));

	printf("OpenPlayback Return : %d\n", m_pAudioEngine->OpenPlaybackDevice(NULL/*(char*)Device[0].m_sDeviceName.c_str()*/, 0, 1, 0, 3));

	string sIn = "";
	cout<<"With audio data receive callback :(y/n)";
	cin>>sIn;

	if(sIn == "y")
		m_pAudioEngine->SetAudioCaptureCallback(this);
	else
	{	
		char buf[1920*3];
		int iCount = 0;
		while(iCount < 10000)
		{
			m_pAudioEngine->StartCapture(buf, 1920);
			m_pAudioEngine->StartPlayback(buf, 1920);
			++iCount;
		}
	}	
	
}
void IdeaAudioEngineTest::OnAudioDataCaptured(char *pBuff, int iLen)
{
	printf("Captured Data Length: %d\n", iLen);
	m_pAudioEngine->StartPlayback(pBuff, iLen);
}

IdeaAudioEngineTest *m_pTestApp = NULL;


int main() {

	m_pTestApp = new IdeaAudioEngineTest();
	m_pTestApp->StartAudioTest();

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	while(true)
	{
		sleep(1000);
	}
	return 0;
}
