
#include "AudioAlsa.h"

static CAudioAlsa *g_pAudioAlsa;

int OpenMic(const char *pszDeviceName, int nSamplingRate, int nBufferSize)
{
    if(!g_pAudioAlsa)
        g_pAudioAlsa = new CAudioAlsa();
    
    const char *szDeviceID = pszDeviceName ? pszDeviceName : "default";
    int nRet = g_pAudioAlsa->OpenInputDevice(szDeviceID);

    if(nRet == 0)
        nRet = g_pAudioAlsa->StartCapture();

	return nRet;
}

CAudioAlsa *GetAlsaInstance()
{

	if(!g_pAudioAlsa)
        g_pAudioAlsa = new CAudioAlsa();
	return g_pAudioAlsa;
}

int CloseMic()
{

	if(g_pAudioAlsa)
		g_pAudioAlsa->CloseInputDevice();
	return 0;
}

int ReadAudio(char *pchBuffer, int nBytes)
{
    if(!g_pAudioAlsa)
        return -1;
    
    return g_pAudioAlsa->ReadData(pchBuffer, nBytes);
}

int OpenSpk(const char *pszDeviceName, int nSamplingRate, int nBufferSize)
{
    if(!g_pAudioAlsa)
	{
		printf("Failure on Speaker\n");
        g_pAudioAlsa = new CAudioAlsa();
    }
    const char *szDeviceID = pszDeviceName ? pszDeviceName : "default";
    int nRet = g_pAudioAlsa->OpenOutputDevice(szDeviceID);
    
    if(nRet == 0)
        nRet = g_pAudioAlsa->StartPlayback();

    return nRet;
}

int CloseSpk()
{
	if(g_pAudioAlsa)
		g_pAudioAlsa->CloseOutputDevice();
	return 0;
}

int WriteAudio(char *pchBuffer, int nBytes)
{
    if(!g_pAudioAlsa)
        return -1;
    
    return g_pAudioAlsa->WriteData(pchBuffer, nBytes);
}

int CloseAudioDevices() {
    if(g_pAudioAlsa){
        CloseMic();
        CloseSpk();
        delete g_pAudioAlsa;
        g_pAudioAlsa = NULL;
    }
    return 0;
}
