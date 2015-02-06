/**
*  File:  ButtonSprite.h                                                                      
*                                                                                            
*  Description:  This header file is the class definition for the ButtonSprite class.         
*                ButtonSprite extends Sprite.                                                 
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/2/2013																	  
*/

#ifndef BUTTON_SPRITE_H_
#define BUTTON_SPRITE_H_

#include "Sprite.h"

class ButtonSprite : public Sprite
{
private:
	int centerX;
	int centerY;

public:
	ButtonSprite(int imgID, int sprtID, char fileName[], int cX, int cY);
	~ButtonSprite();
	void reposition(int cX, int cY);
	bool mouseOver(int mouseX, int mouseY);
	bool clicked(int mouseX, int mouseY);
};

#endif