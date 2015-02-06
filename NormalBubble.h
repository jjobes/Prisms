/**
*  File:  NormalBubble.h                                                                      
*                                                                                            
*  Description:	 This header file is the class definition for the NormalBubble class,         
*                which extends the BubbleSprite class.  Prior to each level, in Prisms.cpp's
*                createAndLoadBubbles(), a series of NormalBubble objects are created and
*                loaded into the bubbles vector.  These are the bubbles that appear on the
*                screen during the level.
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/14/2013																	  
*/

#ifndef NORMAL_BUBBLE_H
#define NORMAL_BUBBLE_H

#include "BubbleSprite.h"

class NormalBubble : public BubbleSprite
{
public:
	NormalBubble(int cX, int cY, int radius, char * bubbleFileName, int imgID, int sprtID, char * noteFileName, int sndID);
	~NormalBubble();
};

#endif