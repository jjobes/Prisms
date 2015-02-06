/**
*  File:  CircleSprite.h                                                                      
*                                                                                            
*  Description:  This header file is the class definition for the CircleSprite class.         
*                CircleSprite extends Sprite and keeps track of its own radius and            
*                center point.                                                                
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/2/2013																	  
*/

#ifndef CIRCLE_SPRITE_H_
#define CIRCLE_SPRITE_H_

#include "Sprite.h"

class CircleSprite : public Sprite
{
private:
	int radius;
	int centerX;
	int centerY;

public:
	CircleSprite(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID);
	CircleSprite(int imgID, int sprtID, char fileName[]);
	~CircleSprite();
	int getRadius() const;
	void setRadius(int r);
	void setCenterX(int cX);
	int getCenterX() const;
	void setCenterY(int cY);
	int getCenterY() const;
};

#endif