/*
 * vars.hpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#ifndef VARS_HPP_
#define VARS_HPP_
#include <sstream>

using namespace std;

class Game;

// Fixme, ca n'a rien � faire ici
extern int tileSizeX;
extern int tileSizeY;

extern int towers_x;
extern int towers_y;

extern SDL_Event event;
extern SDL_Surface *screen;
extern SDL_Surface *img, *back;
extern SDL_Rect coords, carre;
extern const SDL_VideoInfo *vi;

extern Game* gpGame;


template< class T>
    inline string to_string( const T & Value)
{
    stringstream streamOut;
    streamOut << Value;
    return streamOut.str( );
}

#endif /* VARS_HPP_ */
