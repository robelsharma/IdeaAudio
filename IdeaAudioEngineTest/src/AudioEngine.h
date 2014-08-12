/*
 * AudioEngine.h
 *
 *  Created on: Jul 29, 2013
 *      Author: Robel Sharma
 */

#ifndef AUDIOENGINE_H_
#define AUDIOENGINE_H_

#include <vector>
#include <string>

struct DeviceInfo{
	std::string m_sDeviceName;
	std::string m_sDeviceID;
	DeviceInfo(
	        const std::string &sDeviceID = "",
	        const std::string &sDeviceName = "")
	{
		 m_sDeviceName = sDeviceName;
		 m_sDeviceID = sDeviceID;
	}
};

class IEventHandler
{
public:
virtual void OnAudioDataCaptured(char *pBuff, int iLen) = 0;

};

class DiscoverDeviceBetter;

class AudioEngine {
	DiscoverDeviceBetter *m_pDiscoverDevice;
public:
	AudioEngine();
	virtual ~AudioEngine();
	int AvailableAudioDevices(bool bInput, std::vector<DeviceInfo>&Device);
	void ConfigureBetterOptionForAudio(char *sDeviceName, int &iSamplerate, int &iChannels, int &iBufferSize, int &iFragments);
	int OpenCaptureDevice(char *sDeviceName, int iSamplerate, int iChannels, int iBufferSize, int iFragments);
	int OpenPlaybackDevice(char *sDeviceName, int iSamplerate, int iChannels, int iBufferSize, int iFragments);
	void SetAudioCaptureCallback(IEventHandler* pAudioDataEvent);
	void StartCapture(char *pBuff, int nLen);
	void StartPlayback(char *pBuff, int nLen);
	void CloseCapture();
	void ClosePlayback();

};

#endif /* AUDIOENGINE_H_ */
