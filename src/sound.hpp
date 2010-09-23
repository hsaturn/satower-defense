/*
 * sound.hpp
 *
 *  Created on: 27 juil. 2010
 *      Author: saturn
 */

#ifndef SOUND_HPP_
#define SOUND_HPP_
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <cfileparser.hpp>

#include <string>

using namespace std;

class Sound
{
public:
	/**
	 * The file is searched under rsrc/sound path
	 */
	Sound(CFileParser*);
	Sound(const string &sRsrcFile="");
	static void init();

	bool loadFile(const string &sFile);

	void play();
	void stop();

	void init(const string &sRsrcFile);

private:
	Sound(const Sound&);
	Sound operator=(const Sound);
	Mix_Chunk	*mpoSound;
	static bool bInitialized;
	Uint32 mlLastFrameTick;
};

#endif /* SOUND_HPP_ */
