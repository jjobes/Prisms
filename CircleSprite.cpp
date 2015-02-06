/**
*  File:  CircleSprite.cpp                                                                    
*                                                                                            
*  Description:  This file contains the method implementations for the CircleSprite class.  
*                A CircleSprite is a Sprite that also keeps track of its own radius and 
*                center point.
*                                                                                            
*  Author:  jjobes                                                                  
*																							  
*  Last Update:  6/20/2013																	  
*/

#include "CircleSprite.h"

/**
*  Function: CircleSprite(int imgID, int sprtID, char fileName[], int cX, int cY, int r) 
*
*  Purpose: This is the first constructor for the CircleSprite class.  It sets the radius
*           and the x- and y-coordinates of its center point.
*
*           This version is called by the BubbleSprite constructor that is
*           used to create the userBubble (the bubble created when the user
*           clicks the mouse).
*
*  Inputs: imgID, sprtID, fileName - passed up to the Sprite constructor.
*          cX - The x-coordinate of the CircleSprite's center point.
*          cY - The y-coordinate of the CircleSprite's center point.
*          r - The radius (in pixels) of the CircleSprite.
*/

CircleSprite::CircleSprite(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID)
            : Sprite(imgID, sprtID, bubbleFileName)
{
	setRadius( r );
	setCenterX( cX );
	setCenterY( cY );
}

/**
*  Function: CircleSprite(int imgID, int sprtID, char fileName[]) 
*
*  Purpose: This is the second constructor for the CircleSprite class.  
*
*           This version is called by the BubbleSprite constructor that is 
*           used to create the normal bubbles that are automatically 
*           populated on the screen.
*
*  Inputs: imgID, sprtID, fileName - passed up the the Sprite constructor.
*/

CircleSprite::CircleSprite(int imgID, int sprtID, char fileName[]) 
            : Sprite(imgID, sprtID, fileName)
{
	// Pass all incoming arguments up to the Sprite constructor.
}

/**
*  Function: ~CircleSprite() 
*
*  Purpose: This is the destructor for the CircleSprite class.  It is currently
*           empty, but is here in case any new additions to the class require
*           objects to be deleted, or memory to be freed.
*/


CircleSprite::~CircleSprite()
{
	// Destructor of the base class (Sprite) is automatically called.
}

/**
*  Function: getRadius()
*
*  Purpose: This function is used primarily in Prisms.cpp in boundaryCollisionCheck() 
*           and bubbleCollisionCheck().
*
*  Returns: The radius in pixels.
*/

int CircleSprite::getRadius() const
{
	return radius;
}

/**
*  Function: setRadius(int r)
*
*  Purpose: This function sets the radius of the circle to the
*           value passed to it.  The function ensures that the
*           value passed in is non-negative, and sets radius
*           to 0 otherwise.
*
*  Inputs: int r - The radius in pixels.
*/

void CircleSprite::setRadius(int r)
{
	if ( r >= 0 )  // Make sure r is non-negative.
	{
		radius = r;
	}
	else
	{
		radius = 0;  // Set to 0 otherwise.
	}
}

/**
*  Function: setCenterX(int cX)
*
*  Purpose: This function sets the x-coordinate of the circle's
*           center point.  It is called from BubbleSprite's move()
*           function.
*
*  Inputs: int cX - The x-coordinate of the circle's center point.
*/

void CircleSprite::setCenterX(int cX)
{
	centerX = cX;
}

/**
*  Function: getCenterX()
*
*  Purpose: This function returns the x-coordinate of the circle's
*           center point.  It is called from BubbleSprite's expand()
*           and contract() methods.
*
*  Returns: The x-coordinate of the CircleSprite's current center point.
*/

int CircleSprite::getCenterX() const
{
	return centerX;
}

/**
*  Function: setCenterY(int cY)
*
*  Purpose: This function sets the y-coordinate of the circle's
*           center point.  It is called from BubbleSprite's move()
*           function.
*
*  Inputs: int cY - The y-coordinate of the circle's center point.
*/

void CircleSprite::setCenterY(int cY)
{
	centerY = cY;
}

/**
*  Function: getCenterY()
*
*  Purpose: This function returns the y-coordinate of the circle's
*           center point.  It is called from BubbleSprite's expand()
*           and contract() methods.
*
*  Returns: The y-coordinate of the CircleSprite's current center point. 
*/

int CircleSprite::getCenterY() const
{
	return centerY;
}