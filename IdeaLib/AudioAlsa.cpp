#include "AudioAlsa.h"
#include "AudioEngine.h"

#define LOG(...) printf(__VA_ARGS__); putchar('\n');
#define AUDIO_DRV_SPK_FS 48000
#define AUDIO_DRV_MIC_FS 48000
#define AUDIO_DRV_SPK_BLK_SZ 1920
#define AUDIO_DRV_MIC_BLK_SZ 1920



CAudioAlsa::CAudioAlsa():
    m_capture_handle(NULL),
    m_playback_handle(NULL),
	m_iAudioThreadId(0)
{
    m_nChannelsPerFrame = 1;
    m_nBitsPerChannel   = 16;
    m_nBytesPerFrame    = m_nChannelsPerFrame * m_nBitsPerChannel / 8;
	m_bCapture = true;
}

CAudioAlsa::~CAudioAlsa()
{
    CloseInputDevice();
    CloseOutputDevice();
	pthread_join(m_iAudioThreadId, NULL);
	
}

int CAudioAlsa::OpenInputDevice(const char *szDeviceID)
{
    int err = 0;
	if ((err = snd_pcm_open(&m_capture_handle, szDeviceID, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
	    LOG("Error in opening audio input device %s: %s", szDeviceID, snd_strerror(err));
	    return -1;
	}
    
    if ((err = ConfigureDevice(true)) < 0) {
        LOG("Error in configuring audio input device");
        return -1;
    }
    
    LOG("Audio input device %s opened", szDeviceID);
    return 0;
}

int CAudioAlsa::OpenOutputDevice(const char *szDeviceID)
{
    int err = 0;
	if ((err = snd_pcm_open(&m_playback_handle, szDeviceID, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
	    LOG("Error in opening audio output device %s: %s", szDeviceID, snd_strerror(err));
	    return -1;
	}
    
    if ((err = ConfigureDevice(false)) < 0) {
        LOG("Error in configuring audio output device");
        return -1;
    }
    
    LOG("Audio output device %s opened", szDeviceID);
    return 0;
}

void CAudioAlsa::CloseInputDevice()
{
    if(m_capture_handle){
        snd_pcm_close(m_capture_handle);
        m_capture_handle = NULL;
    }
    
    LOG("Audio input device closed");
}

void CAudioAlsa::CloseOutputDevice()
{
    if(m_playback_handle){
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
    }

    LOG("Audio input device closed");
}

int CAudioAlsa::StartCapture()
{
    if(!m_capture_handle)
        return -1;
    /* drop any output we might got and stop */
    snd_pcm_drop(m_capture_handle);
    /* prepare for use */
    snd_pcm_prepare(m_capture_handle);
	m_bCapture = true;
    
    return 0;
}

int CAudioAlsa::StartPlayback()
{
    if(!m_playback_handle)
        return -1;
    
    /* drop any output we might got and stop */
    snd_pcm_drop(m_playback_handle);
    /* prepare for use */
    snd_pcm_prepare(m_playback_handle);
    
    return 0;
}

int CAudioAlsa::StopCapture()
{
    if(m_capture_handle)
        snd_pcm_drop(m_capture_handle);
	m_bCapture = false;
    
    return 0;
}

int CAudioAlsa::StopPlayback()
{
    if(m_playback_handle)
        snd_pcm_drop(m_playback_handle);
    
    return 0;
}

int CAudioAlsa::ReadData(char *pBuf, int nLen)
{
    if(!m_capture_handle)
        return -1;
    
    int nFrames = nLen / m_nBytesPerFrame;
    
    while (nFrames > 0) {
        int nRead = snd_pcm_readi(m_capture_handle, pBuf, nFrames);
        
        if (nRead == -EAGAIN || (nRead >= 0 && nRead < nFrames)) {
            snd_pcm_wait(m_capture_handle, 1000);
        } else if (nRead == -EPIPE) {
            if(snd_pcm_prepare(m_capture_handle) < 0)
                return -1;
        } else if (nRead == -ESTRPIPE) {
            int err;
            while ((err = snd_pcm_resume(m_capture_handle)) == -EAGAIN)
                sleep(1);   /* wait until suspend flag is released */
            if (err < 0) {
                if (snd_pcm_prepare(m_capture_handle) < 0) {
                    return -1;
                }
            }
        } else if (nRead < 0) {
            return -1;
        }
        printf("Frames to read = %d: Frames allready read = %d\n", nFrames, nRead);
        if (nRead > 0) {
            nFrames -= nRead;
            pBuf += nRead * m_nBytesPerFrame;
        }
		printf("Read Success\n");
    }
    
    return nLen;
}

int CAudioAlsa::WriteData(char *pBuf, int nLen)
{
	printf("I am in play\n");
    if(!m_playback_handle)
        return -1;
        
    int nFrames = nLen / m_nBytesPerFrame;
    int nTotal = 0;
    printf("ME\n");
    while (nFrames > 0) {
        int nWritten = snd_pcm_writei(m_playback_handle, pBuf, nFrames);
        
        if (nWritten == -EAGAIN || (nWritten >= 0 && nWritten < nFrames)) {
            snd_pcm_wait(m_playback_handle, 1000);
        }
        else if (nWritten == -EPIPE) {
            if(snd_pcm_prepare(m_playback_handle) < 0)
                return -1;
        } else if (nWritten == -ESTRPIPE) {
            int err;
            while ((err = snd_pcm_resume(m_playback_handle)) == -EAGAIN)
                sleep(1);   /* wait until suspend flag is released */
            if (err < 0) {
                if (snd_pcm_prepare(m_playback_handle) < 0) {
                    return -1;
                }
            }
        }
        else if (nWritten < 0) {
            return -1;
        }
        
        if (nWritten > 0) {
            nTotal += nWritten;
            nFrames -= nWritten;
            pBuf += nWritten * m_nBytesPerFrame;
        }
		printf("Write Success\n");
    }
    
    return nTotal;
}

int CAudioAlsa::ConfigureDevice(bool bInput)
{
    snd_pcm_t *handle = bInput ? m_capture_handle : m_playback_handle;
    if(!handle)
        return -1;

    unsigned int nSampleRate = bInput ? AUDIO_DRV_MIC_FS : AUDIO_DRV_SPK_FS;
    unsigned int nFragments  = 3;
    snd_pcm_uframes_t nFrames = (bInput ? AUDIO_DRV_MIC_BLK_SZ : AUDIO_DRV_SPK_BLK_SZ) / m_nBytesPerFrame * nFragments;
    int err = 0;
    snd_pcm_hw_params_t *hw_params;    
    
    /* allocate memory for hardware parameter structure */
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        LOG("Error in allocating parameter structure for audio %s device: %s", bInput ? "input" : "output", snd_strerror(err));
        return -1;
    }
    
    /* fill structure from current audio parameters */
    if ((err = snd_pcm_hw_params_any(handle, hw_params)) < 0) {
        LOG("Error in initializing parameter structure for audio %s device: %s", bInput ? "input" : "output", snd_strerror(err));
        return -1;
    }
    
    /* set access type, sample rate, sample format, channels */
    if ((err = snd_pcm_hw_params_set_access(handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        LOG("Error in setting access type for audio %s device: %s", bInput ? "input" : "output", snd_strerror(err));
        return -1;
    }
    snd_pcm_format_t format = m_nBitsPerChannel == 16 ? SND_PCM_FORMAT_S16_LE : SND_PCM_FORMAT_UNKNOWN;
    if ((err = snd_pcm_hw_params_set_format(handle, hw_params, format)) < 0) {
        LOG("Error in setting sample format for audio %s device: %s", bInput ? "input" : "output", snd_strerror(err));
        return -1;
    }
    if ((err = snd_pcm_hw_params_set_rate_near(handle, hw_params, &nSampleRate, 0)) < 0) {
        LOG("Error in setting sample rate for audio %s device: %s", bInput ? "input" : "output", snd_strerror(err));
        return -1;
    }
    if ((err = snd_pcm_hw_params_set_channels(handle, hw_params, m_nChannelsPerFrame)) < 0) {
        LOG("Error in setting channel count for audio %s device: %s", bInput ? "input" : "output", snd_strerror(err));
        return -1;
    }
    
    /* set fragments/periods */
    if ((err = snd_pcm_hw_params_set_periods_near(handle, hw_params, &nFragments, 0)) < 0) {
        LOG("Error in setting fragments count to %u for audio %s device: %s", nFragments, bInput ? "input" : "output", snd_strerror(err));
		return -1;
	}
    
    /* set buffer size in frames */
	if ((err = snd_pcm_hw_params_set_buffer_size_near(handle, hw_params, &nFrames)) < 0) {
        LOG("Error in setting buffer size to %u for audio %s device: %s", nFrames, bInput ? "input" : "output", snd_strerror(err));
		return -1;
	}
    
    /* apply the properties to the device */
	if ((err = snd_pcm_hw_params(handle, hw_params)) < 0) {
		LOG("Error in setting HW params to audio %s device: %s", bInput ? "input" : "output", snd_strerror(err));
		return -1;
	}

	return 0;
}

int CAudioAlsa::AvailableAudioDevices(bool bInput, std::vector<DeviceInfo>&Device)
{

	Device.clear();
	snd_ctl_card_info_t *info;
	snd_pcm_info_t *pcminfo;
	snd_ctl_card_info_alloca(&info);
	snd_pcm_info_alloca(&pcminfo);

	int card = -1;
	while (snd_card_next(&card) >= 0 && card >= 0) {
		int err = 0;
		snd_ctl_t *handle;
		char name[20];
		snprintf(name, sizeof(name), "hw:%d", card);
		if ((err = snd_ctl_open(&handle, name, 0)) < 0) {
			continue;
		}

		if ((err = snd_ctl_card_info(handle, info)) < 0) {
			snd_ctl_close(handle);
			continue;
		}

		int dev = -1;
		while (snd_ctl_pcm_next_device(handle, &dev) >= 0 && dev >= 0) {
			snd_pcm_info_set_device(pcminfo, dev);
			snd_pcm_info_set_subdevice(pcminfo, 0);
			snd_pcm_info_set_stream(pcminfo, bInput ? SND_PCM_STREAM_CAPTURE
					: SND_PCM_STREAM_PLAYBACK);
			if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0) {
				continue;
			}

			char szDeviceID[20];
			snprintf(szDeviceID, sizeof(szDeviceID), "plughw:%d,%d", card, dev);
			Device.push_back(DeviceInfo(szDeviceID,
					snd_ctl_card_info_get_name(info)));
		}

		snd_ctl_close(handle);
	}
	return Device.size();
}

void CAudioAlsa::SetAudioCaptureCallback(IEventHandler* pAudioDataEvent)
{
	m_pAudioDataEvent = pAudioDataEvent;
	m_bCapture = true;
	pthread_create(&m_iAudioThreadId, NULL, CAudioAlsa::AudioCaptureWithCallback, this);
}

void *CAudioAlsa::AudioCaptureWithCallback(void *param)
{
	printf("Audio capture thread started\n");
	CAudioAlsa *pThis = (CAudioAlsa*)param;

	while(true)
	{
		if(pThis->m_bCapture)
		{
			char buf[1920*3];
			int nLen = pThis->ReadData(buf, 1920);
			if(nLen > 0)
			pThis->m_pAudioDataEvent->OnAudioDataCaptured(buf, nLen);
		}
	}

	printf("Audio capture thread started\n");
	return NULL;
}

