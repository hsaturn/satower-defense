/*
 * sound.cpp
 *
 *  Created on: 27 juil. 2010
 *      Author: saturn
 */


#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <iostream>

using namespace std;

#include "sound.hpp"
#include "game.hpp"
#include "vars.hpp"

bool Sound::bInitialized=false;
extern Uint32 glLastFrameTick;

void Sound::init()
{
	Sound::bInitialized=true;
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
}

Sound::Sound(CFileParser *poDef)
:
mpoSound(0)
{
	init(poDef->getNextString("sound file"));

}

Sound::Sound(const string &sRsrcFile)
:
mpoSound(0)
{
	init(sRsrcFile);
}

void Sound::init(const string &sRsrcFile)
{
	if (mpoSound);	// FIXME free MIX resource
	string sRealFile=gpGame->findRsrcFile("sound/"+sRsrcFile);
	loadFile(sRealFile);
	if (Sound::bInitialized==false)
		Sound::init();
}

bool Sound::loadFile(const string &sFile)
{
	mpoSound=0;
	if (sFile.length())
	{
		mpoSound = Mix_LoadWAV(sFile.c_str());
		if (!mpoSound)
		{
			cerr << "Error loading [" << sFile << "] : " << Mix_GetError() << endl;
		}
	}
	return mpoSound>0;
}

void Sound::play()
{
	int channel;

	if (mpoSound && (mlLastFrameTick!=glLastFrameTick))
	{
		mlLastFrameTick=glLastFrameTick;
		channel = Mix_PlayChannel(-1, mpoSound, 0);
		if(channel == -1) {
			cerr << "Unable to play WAV file: " << Mix_GetError() << endl;
		}
	}
}
