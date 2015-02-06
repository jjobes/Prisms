/**
*  File:  UserBubble.h                                                                      
*                                                                                            
*  Description:	 This header file is the class definition for the UserBubble class,         
*                which extends the BubbleSprite class.  A single UserBubble object
*                is created in Prisms.cpp when the user first clicks the mouse button
*                on the screen.  This object is then loaded into the bubbles vector.
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/20/2013																	  
*/

#ifndef USER_BUBBLE_H
#define USER_BUBBLE_H

#include "BubbleSprite.h"

class UserBubble : public BubbleSprite
{
public:
	UserBubble(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID, char * noteFileName, int sndID);
	~UserBubble();
};

#endif