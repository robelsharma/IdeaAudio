/*
 * IdeaAudioEngineTest.h
 *
 *  Created on: Jul 30, 2013
 *      Author: root
 */

#ifndef IDEAAUDIOENGINETEST_H_
#define IDEAAUDIOENGINETEST_H_

#include "AudioEngine.h"

class IdeaAudioEngineTest : public IEventHandler
{
public:
	IdeaAudioEngineTest();
 	virtual ~IdeaAudioEngineTest();

 	void OnAudioDataCaptured(char *pBuff, int iLen);
 	void StartAudioTest();

 	AudioEngine *m_pAudioEngine;
};

#endif /* IDEAAUDIOENGINETEST_H_ */
