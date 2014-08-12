#ifndef AUDIO_ALSA_H
#define AUDIO_ALSA_H

#include <alsa/asoundlib.h>
#include <pthread.h>
#include <vector>

using namespace std;
class DeviceInfo;

class IEventHandler;

class CAudioAlsa
{
	snd_pcm_t *m_capture_handle;
	snd_pcm_t *m_playback_handle;
    
    unsigned int m_nChannelsPerFrame;
    unsigned int m_nBitsPerChannel;
    unsigned int m_nBytesPerFrame;
    
    int ConfigureDevice(bool bInput);

public:
    CAudioAlsa();
    ~CAudioAlsa();

	int AvailableAudioDevices(bool bInput, std::vector<DeviceInfo>&Device);

	int OpenInputDevice(const char *szDeviceID);
	int OpenOutputDevice(const char *szDeviceID);
	void CloseInputDevice();
	void CloseOutputDevice();
	
	int StartCapture();
	int StartPlayback();
	int StopCapture();
	int StopPlayback();
    
	int ReadData(char *pBuf, int nLen);
	int WriteData(char *pBuf, int nLen);

	void SetAudioCaptureCallback(IEventHandler* pAudioDataEvent);

public:
	bool m_bCapture;
	IEventHandler* m_pAudioDataEvent;

private:
	pthread_t m_iAudioThreadId;	
	static void *AudioCaptureWithCallback(void *param);
};

#endif  // AUDIO_ALSA_H
