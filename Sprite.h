/**
*  File:  Sprite.h                                                                            
*                                                                                            
*  Description:  This header file contains the class definition for the Sprite class.
*                Sprites are dynamically created in Prisms.cpp.  They are used to 
*                represent the menu interface buttons, but most importantly to represent
*                each of the colored bubbles that appears on the screen during the game.
*
*                The Sprite class forms the top of the inheritance hierarchy, which can
*                be described as follows:
*
*
*                                  Sprite
*                                     |
*                             ------------------
*                             |                |
*                        ButtonSprite     CircleSprite
*                                              |
*                                         BubbleSprite
*                                              |
*                                      ------------------
*                                      |                |
*                                 NormalBubble      UserBubble
*
*
*                ButtonSprite, NormalBubble and UserBubble are #included in Prisms.cpp.
*                         
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/20/2013																	  
*/

#ifndef SPRITE_H_
#define SPRITE_H_

/**
*  Sprite.h #includes the following header files, and because
*  this class forms the top of the inheritance hierarchy, the 
*  contents of these files will also get imported into Prisms.cpp.
*/

#include "DarkGDK.h"    // Import the game programming library functions (these functions begin with the letters 'db').
#include "Constants.h"  // Ensure access to application-wide constants.
#include <fstream>      // Used to create an ofstream object called logFile (Prisms.log).
#include <time.h>       // Needed to record the game start and end times in logFile.

using namespace std;

class Sprite
{
private:
	int imageID;
	int spriteID;
	char lineBuffer[MAX_ERROR_BUFFER];  // For writing potential errors to logFile.
	int degrees;
	int width;
	int height;

public:
	Sprite(int imgID, int sprtID, char fileName[]);
	~Sprite();
	void display(int cX, int cY) const;
	void hide() const;
	void show() const;
	void resize(int width, int height);
	void rotate(int a);
	void scale(int s) const;
	void setOffsetToCenter();
	void setOffset(int x, int y) const;
	int getSpriteID() const;
	int getDegrees() const;
	void setWidth(int w);   
	int getWidth() const;  
	void setHeight(int h); 
	int getHeight() const;  
	void setAlpha(int a);
};

#endif