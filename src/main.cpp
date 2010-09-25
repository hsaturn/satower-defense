#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <map>
#include <list>
#include <SDL/SDL_gfxPrimitives.h>
#include <csexception.hpp>
#include "coord.hpp"
#include "mapelt.hpp"
#include "walkerBase.hpp"
#include "towerBase.hpp"
#include "missileBase.hpp"
#include "missileFactory.hpp"
#include "bulletMissile.hpp"
#include "laserMissile.hpp"
#include "walkerGeneratorBase.hpp"
#include "sound.hpp"
#include "walkers.hpp"
#include "text.hpp"
#include "game.hpp"
#include <sstream>
#include <SimpleXml.hpp>
#include "tiled/tiledMap.hpp"
#include "coldLaser.hpp"
#include "button.hpp"
#include "waves.hpp"
#include <ogolVectoroid.hpp>

#include "gui/CWindow.hpp"

// Beuaark
void setWalkable(int x,int y);
void setUnwalkable(int x,int y);
void setBuildable(int x,int y);
void setUnbuildable(int x,int y);
bool isWalkable(int x,int y);
bool isBuildable(int x,int y);

using namespace std;

void initMap(const string sImage);
void drawMap();

extern void InitializePathfinder();
// Fixme, ca n'a rien � faire ici
// Attention, pour aStar, il faut que tileSizeX==tileSizeY

extern int tileSizeX;
extern int tileSizeY;

int towers_x=583;
int towers_y=39;
Uint32 glLastFrameTick;

SDL_Event event;
SDL_Surface *screen;
SDL_Surface *img=0, *back=0, *inter=0;
SDL_Rect coords, carre;
const SDL_VideoInfo *vi;
//Uint32 bleu;

coord oMousePos;

const coord mapTL(11,96);
const coord mapBR(560,545);

Game* gpGame;
list<walkerBase*>	lstWalkers;

int getRand(int iMin,int iMax)
{
	return iMin+(rand()%(iMax-iMin));
}

towerBase*	searchTower(list<towerBase*> lst, const coord& oPos)
{
	list<towerBase*>::const_iterator oit=lst.begin();
	while(oit!=lst.end())
	{
		towerBase* p=*oit++;
		float fDistance=oPos.xyMaxDistanceTo(p->getCenterCoord());
		if (fDistance<=tileSizeX/2)
			return p;
	}
	return 0;
}

int main(int argc, char **argv)
{
	Uint32		lTimerFps=0;
	Uint32		lFrames=0;
	bool		bRedrawAll=true;
	towerBase*	pDisplayTower=0;
	towerBase*	pSelectedTower=0;
	Button*		pButtonOver=0;
	Button*		pButtonClicked=0;
	string		sTheme="default";

	int iArg=1;
	while(iArg<argc)
	{
		string sArg(argv[iArg]);
		if (sArg=="-theme")
		{
			sTheme=argv[++iArg];
		}
		iArg++;
	}

	try
	{
		InitializePathfinder();
		/* Initialisation de SDL */
		if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
		{
			cerr << "Erreur SDL: " << SDL_GetError() << endl;
			exit(1);
		}


		atexit(SDL_Quit); /* Pour sortir proprement */
		SDL_WM_SetCaption("Tower Defense",NULL);
		coord posText(100,100);
		towerBase*			mouseTower=0;
		list<towerBase*>	lstTowers;
		gpGame=new Game();
		gpGame->readTheme(sTheme);
		tileSizeX=gpGame->getTileSizeX();	// FIXME, sizes should exist only at one place
		tileSizeY=gpGame->getTileSizeY();	// FIXME

		list<walkerBase*>	lstPathFinders;
		list<coord>::const_iterator oitStarts=gpGame->beginStarts();
		list<coord>::const_iterator oitEnds;

		// Build walkers in order to check if all paths are reachable
		// when the player builds a tower
		while(oitStarts!=gpGame->endStarts())
		{
			oitEnds=gpGame->beginEnds();
			while(oitEnds!=gpGame->endEnds())
			{
				walkerBase*	p=new walkerBase(1,coordToPixel(*oitStarts),1,*oitEnds,0);
				lstPathFinders.push_front(p);
				oitEnds++;
			}
			oitStarts++;
		}
		//walkerBase*	poFindPath=new walkerBase(1,oStartPixels,0,oDest,0);

		// readMissileFactories();

		gpGame->changeLevel("menu");
		Sound oWrong("wrong.wav");
		Sound oBuild("build.wav");
		Sound oNoMoney("nomoney.wav");

		gpGame->readConfig(gpGame->findRsrcFile("config.def"));
		Waves*		poWaves=new Waves();

		//missileBase::readFromDef(gpGame->findRsrcFile("weapons.def"));
		//towerBase::readTowers(gpGame->findRsrcFile("towers.def"));

		initMap(gpGame->findRsrcFile("vierge.png")); // FIXME game responsability

		bool bMouseTowerValid=false;	// Position de la souris valide pour poser une tour.
		glLastFrameTick=SDL_GetTicks();
		Uint32	lEllapsed;

		bool bRun=true;
		while(bRun)
		{
			if (gpGame->state()==STATE_PAUSED)
			{
				lEllapsed=0;
				glLastFrameTick=SDL_GetTicks();
			}
			else
			{
				lEllapsed=glLastFrameTick;
				glLastFrameTick=SDL_GetTicks();
				lEllapsed=glLastFrameTick-lEllapsed;
			}

			list<towerBase*>::const_iterator oitTowers;
			list<walkerBase*>::iterator oitWalkers;

			oitTowers=lstTowers.begin();
			while(oitTowers!=lstTowers.end())
			{
				towerBase* p=*oitTowers++;
				p->update(lEllapsed);
			}

			oitWalkers=lstWalkers.begin();
			while(oitWalkers!=lstWalkers.end())
			{
				bool bDelete=false;
				walkerBase* p=*oitWalkers;
				if (	(p->getShootPoint().x()>mapBR.x()+50) ||
						(p->getShootPoint().y()>mapBR.y()+50) ||
						(p->getShootPoint().x()<mapTL.x()-50) ||
						(p->getShootPoint().y()<mapTL.y()-50) ||
						(p->getVisible()==false)
					)
				{
					if (p->getRefCount()==0)
					{
						bDelete=true;
					}
					else
						p->setVisible(false);
				}
				if (bDelete)
				{
					delete p;
					oitWalkers=lstWalkers.erase(oitWalkers);
					cout << "walker deleted, left=" << lstWalkers.size() << endl;
				}
				else
				{
					p->update(lEllapsed);
					oitWalkers++;
				}
			}

			if (gpGame->state()==STATE_PLAY)
			{
				walkerBase* p=poWaves->update(lEllapsed);
				if (p)
				{
					lstWalkers.push_back(p);
				}
			}

			// Window system test
			{
				SDL_Rect	oRect;
				oRect.x=100;
				oRect.y=100;
				oRect.w=100;
				oRect.h=50;

				CWindow*	p=new CWindow(oRect,ogolPencil::stringToColor("blue",50));

				oRect.x=300;
				CWindowBorder* pNewBorderedWindow=
						new CWindowBorder
						(new CWindow(oRect,ogolPencil::stringToColor("blue",50)),
						3,ogolPencil::stringToColor("yellow",50));

				p->draw(back);
				pNewBorderedWindow->draw(back);
				delete p;
				delete pNewBorderedWindow;
			}

			pButtonClicked=0;

			while (SDL_PollEvent(&event))
			{
				bool bCreateWalker=false;

				switch(event.type)
				{
					case SDL_MOUSEBUTTONDOWN:
						pButtonClicked=Button::searchButton(coord(event));

						if (gpGame->state()==STATE_PLAY)
						{
							if (mouseTower && event.button.button==SDL_BUTTON_LEFT && mouseTower->getCoord().inSquare(mapTL,mapBR))
							{
								// Poser une tour.
								bool bOk=true;

								towerBase*	pNewTower=new towerBase(*mouseTower);	// GAG, c'est du copycons par d�faut
								pNewTower->setViewRange(false);

								if (!pNewTower->isBuildable())
								{
									bOk=false;
									oNoMoney.play();
								}
								else
								{
									bOk=true;
									// A new tower has been built
									// recompute all path
									//walkerBase::resetClosedList();
									Uint32 xTile=pNewTower->getCoord().getTilesCoordX();
									Uint32 yTile=pNewTower->getCoord().getTilesCoordY();
									if (isBuildable(xTile,yTile))
									{
										if (isWalkable(xTile,yTile))
										{
											setUnwalkable(xTile,yTile);
											list<walkerBase*>::iterator oitPathFinder=lstPathFinders.begin();
											while(oitPathFinder!=lstPathFinders.end())
											{
												walkerBase* p=*oitPathFinder++;
												if (!p->computePath("main"))
												{
													bOk=false;
													cout << "Path finder blocked" << endl;
													break;
												}
											}
											if (bOk)
											{
												// cerr << "Recomputing paths" << endl;
												list<walkerBase*>::iterator oit=lstWalkers.begin();
												while(oit!=lstWalkers.end())
												{
													walkerBase* pWalker=*oit++;
													if (pWalker->computePath("main2")==false)
													{
														cout << "no path for a walker" << endl;
														bOk=false;
														break;
													}
												}
											}
											else
											{
												cout << "no path !" << endl;
												bOk=false;
											}
											if (!bOk)
												setWalkable(xTile,yTile);
										}
									}
									else
									{
										bOk=false;
									}
									if (bOk)
									{
										setUnbuildable(xTile,yTile);
										bRedrawAll=true;
										oBuild.play();
										lstTowers.push_back(pNewTower);
										pNewTower->spendCosts();
										cout << "Nombre de tours : " << lstTowers.size() << endl;
									}
									else
									{
										oWrong.play();
									}
								}
							}
							else
							{
								mouseTower=towerBase::buildTowerAs(coord(event));
								if (mouseTower)
								{
									pSelectedTower=0;
									if (!mouseTower->isBuildable())
									{
										oNoMoney.play();
										delete mouseTower;
										mouseTower=0;
									}
								}
								else if (event.button.button==SDL_BUTTON_LEFT)
								{
									pDisplayTower=searchTower(lstTowers,coord(event));
									if (pDisplayTower && pDisplayTower!=mouseTower)
										pSelectedTower=pDisplayTower;
								}
							}
						}
						break;

					case SDL_MOUSEMOTION:
						pButtonOver=Button::searchButton(coord(event));
						if (gpGame->state()==STATE_PLAY)
						{
							pDisplayTower=towerBase::buildTowerAs(coord(event));
							if (pDisplayTower==0)
							{
								pDisplayTower=searchTower(lstTowers,coord(event));
							}
							if (mouseTower==0)
							{
								bCreateWalker=true;
							}
							else
							{
								coord p(event);
								if (p.inSquare(mapTL,mapBR))
								{
									p.snapToGrid();
								}
								mouseTower->setCoord(p);
							}
						}
						break;
				}
			}

			if (pButtonClicked)
			{
				string sAction=pButtonClicked->getAction();
				if (sAction=="sell_tower")
				{
					cout << "sell tower 1" << endl;
					if (pSelectedTower)
					{
						cout << "sell tower 2" << endl;
						list<towerBase*>::iterator oit=lstTowers.begin();
						while(oit!=lstTowers.end())
						{
							towerBase* p=*oit;
							if (p==pSelectedTower)
							{
								coord pos=pSelectedTower->getCoord();
								cout << "Selling tower" << endl;
								setBuildable(pos.getTilesCoordX(),pos.getTilesCoordY());
								gpGame->mlBank+=pSelectedTower->getSellValue();
								lstTowers.erase(oit);
								delete pSelectedTower;
								pSelectedTower=0;
								bRedrawAll=true;
								break;
							}
							oit++;
						}
					}
				}
				else if (sAction=="pause")
				{
					gpGame->togglePause();
				}
				else if (sAction=="next_wave")
				{

					poWaves->nextWave();
				}
				else
				{
					cerr << "Unknown button action: [" << sAction << "]" << endl;
				}
			}


			if (bRedrawAll)
			{
				SDL_BlitSurface(img, &coords, inter, &coords);
				gpGame->drawMap(inter,true); // FIXME game responsability
				// Display towers if needed
				oitTowers=lstTowers.begin();
				while(oitTowers!=lstTowers.end())
				{
					towerBase* p=*oitTowers++;
					p->checkTargets();
					p->draw(inter);
				}
			}

			// Prepare draw area (back)
			SDL_BlitSurface(inter, &coords, back, &coords);

			if (pButtonOver)
			{
				pButtonOver->highlight(back);
			}
			if (pSelectedTower)
			{
				gpGame->displayTowerInfos(back,pSelectedTower);
			}
			else if (pDisplayTower)
			{
				gpGame->displayTowerInfos(back,pDisplayTower);
			}
			else if (mouseTower)
			{
				gpGame->displayTowerInfos(back,mouseTower);
			}

			gpGame->printScores(back);
			gpGame->displayCurrentNextWalkers(lEllapsed,back,poWaves->getCurWalker());

			// Gestion du curseur en forme de tour.
			// Avant de dessiner la tour, on cale la tour sur la grille.
			if (mouseTower)
			{
				coord p=mouseTower->getCoord();

				if (p.inSquare(mapTL,mapBR))
				{
					bMouseTowerValid=true;
				}
				else
				{
					bMouseTowerValid=false;
					mouseTower->setCoord(coord(-100,-100));
				}
				mouseTower->setViewRange(bMouseTowerValid);
				mouseTower->draw(back);
			}

			// Affichage des walkers
			oitWalkers=lstWalkers.begin();
			while(oitWalkers!=lstWalkers.end())
			{
				walkerBase* p=*oitWalkers++;
				p->draw(back);
			}

			if (lstWalkers.size()<=1)
			{
				//cout << "Asking next wave..." << endl;
				poWaves->nextWave(3000);	// FIXME should be a theme parameter
			}
			// Affichage des missiles
			oitTowers=lstTowers.begin();
			while(oitTowers!=lstTowers.end())
			{
				towerBase* p=*oitTowers++;
				p->checkTargets();
				p->drawMissiles(back);
			}

			ogolVectoroid::update(lEllapsed);
			ogolVectoroid::draw(back);

			gpGame->drawMap(back,false);
			SDL_BlitSurface(back, &coords, screen, &coords);
			SDL_UpdateRects(screen,1,&coords);

			Uint8 *keys;
			keys = SDL_GetKeyState(NULL);
			if ( keys[SDLK_ESCAPE] == SDL_PRESSED )
			{
				printf("Bye bye...\n");
				exit(0);
			}
			lFrames++;
			if (glLastFrameTick>lTimerFps)
			{
				static int iLessThan30=0;
				cout << "FPS : " << lFrames << ", vectoroids count: " << ogolVectoroid::count() << " walkers " << lstWalkers.size() << endl ;
				lTimerFps=glLastFrameTick+1000;

				if (lFrames<30)
				{
					iLessThan30++;
					if ((iLessThan30>10) && (giOgolVectoroidTimeMs>500))
					{
						iLessThan30=0;
						giOgolVectoroidTimeMs=(int)(giOgolVectoroidTimeMs*0.9);
						cout <<  "Reducing fragment display time (fps < 30) to " << giOgolVectoroidTimeMs << endl;
					}
				}
				else
					iLessThan30=0;

				lFrames=0;
			}
			bRedrawAll=false;
		}
	}
	catch(CSException* e)
	{
		cerr << e->getCompleteError() << endl;
	}
	catch(string &s)
	{
		cout << "Exception : " << s << endl;
	}
	catch(const char* p)
	{
		cout << "Exception const char*" << p << endl;
	}
	return 0;
}

void initMap(const string sImage)
{
	img = IMG_Load(sImage.c_str()); /* Charge l'image */
	if(!img)
	{
		cerr << "Impossible de charger l'image " << sImage << SDL_GetError() << endl;
		exit(1);
	}

	if (img->format->BitsPerPixel!=24)
	{
		cerr << sImage << " must have 24 bits per pixel." << endl;
	}

	/* Alloue un second buffer de la m�me taille que l'image */
	inter = SDL_AllocSurface(SDL_SWSURFACE, img->w, img->h,
			img->format->BitsPerPixel,
			img->format->Rmask, img->format->Gmask,
			img->format->Bmask,img->format->Amask);

	/* Alloue un second buffer de la m�me taille que l'image */
	back = SDL_AllocSurface(SDL_SWSURFACE, img->w, img->h,
			img->format->BitsPerPixel,
			img->format->Rmask, img->format->Gmask,
			img->format->Bmask,img->format->Amask);

	vi = SDL_GetVideoInfo();
	if(vi && vi->wm_available) /* Change les titres */
		SDL_WM_SetCaption("Demo SDL", "Icone SDL");

	/* Initialise un mode vid�o id�al pour cette image */
	screen = SDL_SetVideoMode(img->w, img->h,
			img->format->BitsPerPixel,SDL_HWSURFACE);
	//,
	//                          SDL_FULLSCREEN)
	if(!back || !screen || !inter)
	{
		cerr << "Unable to allocate a surface : " << sImage
			<< SDL_GetError() << endl;
		exit(1);
	}

	//bleu = SDL_MapRGB(screen->format, 0, 0, 255);
	coords.x = coords.y = 0;
	coords.w = img->w; coords.h = img->h;
	carre.w = carre.h = 30;
	carre.y = (img->h - 30) / 2;
}

