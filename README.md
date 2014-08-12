IdeaAudio
=========

IdeaAudio library with Duplex Alsa Audio capture and playback for CentOS 6.x and Ubuntu 
Advanced Linux Sound Architecture (known by the acronym ALSA) is a free and open source software framework released under the GNU GPL and the GNU LGPL that provides an API for sound card device drivers. It is part of the Linux kernel. Some of the goals of the ALSA project at its inception were automatic configuration of sound-card hardware and graceful handling of multiple sound devices in a system, goals which it has largely met. A couple of different frameworks, such as JACK, use ALSA to allow performing low-latency professional-grade audio editing and mixing.

Concepts
This section provides an overview of basic concepts pertaining to ALSA.
Typically, ALSA supports up to eight cards, numbered 0 through 7; each card is a physical or logical kernel device capable of input, output, or control of sound, and card number 0 is used by default when no particular card is specified. Furthermore, each card may also be addressed by its id, which is an explanatory string such as "Headset" or "ICH9".
A card has devices, numbered starting at 0; a device may be of playback type, meaning it outputs sound from the computer, or some other type such as capture, control, timer, or sequencer; device number 0 is used by default when no particular device is specified.
A device may have subdevices, numbered starting at 0; a subdevice represents some relevant sound endpoint for the device, such as a speaker pair. If the subdevice is not specified, or if subdevice number -1 is specified, then any available subdevice is used.
A card's interface is a description of an ALSA protocol for accessing the card; possible interfaces include: hw, plughw, default, and plug:dmix. The hw interface provides direct access to the kernel device, but no software mixing or stream adaptation support. The plughw and default enable sound output where the hw interface would produce an error.
An application typically describes sound output by combining all of the aforementioned specifications together in a device string, which has one of the following forms (which are case sensitive):

    interface:card,device,subdevice
    interface:CARD=1,DEV=3,SUBDEV=2.

An ALSA stream is a data flow representing sound; the most common stream format is PCM that must be produced in such a way as to match the characteristics or parameters of the hardware, including:

    sampling rate: 44.1 kHz on home stereos, and 48 kHz on home theaters
    sample width: measured in some number of bits per sample (such as 8, 16, 24, or 32 bits/sample)
    sample encoding
    number of channels: 1 for mono, 2 for stereo, or 6 for AC-3/IEC958


Now what IdeaAudio provide?
---------------------------------

IdeaAudio  provide you a shared library (.so) and an Interface . Then just need to add this library and interface in your project and use ALSA audio. It also provide a SampleApp compatible in Eclipse IDE with demonstrating use of a audio device.

Download:
------------

SampleApp source code and library Link

Direct Download:
-------------------
Link

Test:
-----

Successful test log is also provided in the link as IdeaAudio.log

API documentation:
----------------------

```
struct DeviceInfo{
    std::string m_sDeviceName;
    std::string m_sDeviceID;
};

/**
*    DeviceInfo stored the available audio devices information
*    @param m_sDeviceName is the name of the device
*    @param m_sDeviceID is the Device ID (ex: plughw:0,0 , plughw:1,0 etc.)
*/


int AvailableAudioDevices(bool bInput, std::vector<DeviceInfo>&Device);

/**
*    AvailableAudioDevices  used to find the available audio devices as plughw:X,Y(ex: plughw:0,0 plughw:1,0 etc.)
*    @param bInput is true for capture and false for playback
*    @param Device is the list of Device Info found after the execution of this API
*    @return number of audio devices.
*/

void ConfigureBetterOptionForAudio(char *sDeviceName, int &iSamplerate, int &iChannels, int &iBufferSize, int &iFragments);

 Note: Right now not active. 

/**
*    ConfigureBetterOptionForAudio autometically find the values for particular device
*    @param sDeviceName is the device name for which evalution is needed
*    @param iSamplerate is the sampling rate of audio data (ex: 48000 Hz)
*     @param iChannels is the channel number need to be provided
*    @param iBufferSize is the buffer size for which we are evaluating
*    @param iFragments the fragment which will be provided after evalution

*/


int OpenCaptureDevice(char *sDeviceName, int iSamplerate, int iChannels, int iBufferSize, int iFragments);


/**
*    OpenCaptureDevice is used to open the audio capture device
*    @param sDeviceName is the deviceID from std::vector<DeviceInfo>&Device (ex: plughw:0,0 plughw:1,0 etc.)
*    @param iSamplerate is the sampling rate of audio data (ex: 48000 Hz)
*     @param iChannels is the channel number
*    @param iBufferSize is the buffer size
*    @param iFragments is the fragment number
*/

int OpenPlaybackDevice(char *sDeviceName, int iSamplerate, int iChannels, int iBufferSize, int iFragments);


/**
*    OpenPlaybackDevice is used to open the audio playback device
*    @param sDeviceName is the deviceID from std::vector<DeviceInfo>&Device (ex: plughw:0,0 plughw:1,0 etc.)
*    @param iSamplerate is the sampling rate of audio data (ex: 48000 Hz)
*     @param iChannels is the channel number
*    @param iBufferSize is the buffer size
*    @param iFragments is the fragment number
*/

void StartCapture(char *pBuff, int nLen);

/**
*    StartCapture start the capturing from open capture audio device
*    @param pBuff is the buffer which is full after a successful capture
*    @param nLen is the size of the needed data buffer
*/

void StartPlayback(char *pBuff, int nLen);

/**
*    StartPlayback start the  playback on the open playback audio device
*    @param pBuff is the buffer which is full and need to play
*    @param nLen is the size of the data buffer
*/


void CloseCapture();

/**
*    CloseCapture close opened capture audio device
*/


void ClosePlayback();

/**
*    ClosePlayback close opened playback audio device
*/


virtual void OnAudioDataCaptured(char *pBuff, int iLen) = 0;


/**
*    OnAudioDataCaptured is an event , fired after a full length capture
*    @param pBuff is the buffer which is full after a successful capture
*    @param nLen is the size of the captured data buffer
**/
```
