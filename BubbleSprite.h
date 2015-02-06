/**
*  File:  BubbleSprite.h                                                                      
*                                                                                            
*  Description:	 This header file is the class definition for the BubbleSprite class,         
*                which extends the Sprite class.  BubbleSprite is the base class of
*                both UserBubble and NormalBubble.
*                                                                                            
*  Author:  jjobes                                                                
*																							  
*  Last Update:  6/20/2013																	  
*/

#ifndef BUBBLE_SPRITE_H_
#define BUBBLE_SPRITE_H_

#include "CircleSprite.h"

class BubbleSprite : public CircleSprite
{
private:
	bool paused;
	bool popping;
	bool expanding;
	bool holding;
	bool contracting;
	bool isUserBubble;
	int maxRadius;
	int holdCounter;
	int popSoundID;
	char * popFileName;
	char lineBuffer[MAX_ERROR_BUFFER];
	time_t currentTime;

public:
	static int currentPopping;  // Total number of bubbles currently popping.  Shared by all BubbleSprite objects.
	BubbleSprite(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID);
	~BubbleSprite();
	void loadPopSound(char * fileName, int soundID);
	void playPopSound();
	void move();
	void pop();
	void expand();
	void hold();
	void contract();
	void setPaused(bool p);
	void setPopping(bool p);
	bool getPopping() const;
	void setExpanding(bool e);
	void setContracting(bool c);
	void setIsUserBubble(bool u);
	void setMaxRadius(int m);
	void setHoldCounter(int h);
	void setPopSoundID(int id);
	int getPopSoundID() const;
	void setPopFileName(char * name);
	char * getPopFileName() const;
	bool getIsUserBubble() const;
};

#endif
