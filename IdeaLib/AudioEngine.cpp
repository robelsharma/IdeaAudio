//============================================================================
// Name        : AudioEngine.cpp
// Author      : Robel Sharma
// Version     :
// Copyright   : BSD
// Description : Shared library application
//============================================================================

#include "AudioEngine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <getopt.h>
#include <pthread.h>
#include "AudioAlsa.h"

CAudioAlsa *GetAlsaInstance();
int ReadAudio(char *pchBuffer, int nBytes);
int OpenMic(const char *pszDeviceName, int nSamplingRate, int nBufferSize);
int CloseMic();

int WriteAudio(char *pchBuffer, int nBytes);
int OpenSpk(const char *pszDeviceName, int nSamplingRate, int nBufferSize);
int CloseSpk();


AudioEngine::AudioEngine()
{
	
}

AudioEngine::~AudioEngine()
{
	
}



int AudioEngine::AvailableAudioDevices(bool bInput, std::vector<DeviceInfo>&Device)
{
	return GetAlsaInstance()->AvailableAudioDevices(bInput, Device);
}

void AudioEngine::ConfigureBetterOptionForAudio(char *sDeviceName, int &iSamplerate, int &iChannels, int &iBufferSize, int &iFragments)
{
	
}

int AudioEngine::OpenCaptureDevice(char *sDeviceName, int iSamplerate, int iChannels, int iBufferSize, int iFragments)
{
	return OpenMic(sDeviceName, iSamplerate, iBufferSize);
}

int AudioEngine::OpenPlaybackDevice(char *sDeviceName, int iSamplerate, int iChannels, int iBufferSize, int iFragments)
{
	return OpenSpk(sDeviceName, iSamplerate, iBufferSize);

}

void AudioEngine::SetAudioCaptureCallback(IEventHandler* pAudioDataEvent)
{
	GetAlsaInstance()->SetAudioCaptureCallback(pAudioDataEvent);
}

int AudioEngine::StartCapture(char *pBuff, int nLen)
{
	return ReadAudio(pBuff, nLen);
}

int AudioEngine::StartPlayback(char *pBuff, int nLen)
{
	return WriteAudio(pBuff, nLen);
}

void AudioEngine::CloseCapture()
{
	CloseMic();
}

void AudioEngine::ClosePlayback()
{
 	CloseSpk();
}



