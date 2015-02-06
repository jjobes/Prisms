/**
*  File:  UserBubble.cpp                                                                    
*                                                                                            
*  Description:	 This file contains the method implementations for the UserBubble class.    
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/20/2013																	  
*/

#include "UserBubble.h"

/**
*  Function: UserBubble(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID, char * noteFileName, int sndID)
*
*  Purpose: This is the constructor for the UserBubble class.  It is called
*           from detectUserClick() in Prisms.cpp.  The incoming values are
*           all passed up to the BubbleSprite constructor.
*
*  Inputs: cX - The x-coordinate of the mouse pointer's location when the user clicks the mouse.
*          cY - The y-coordinate of the mouse pointer's location when the user clicks the mouse.
*          r - The radius that is passed in (12).
*          bubbleFileName - The path to the user bubble's .png image file.
*          imgID - The image ID to assign to this image resource.
*          sprtID - The sprite ID to assign to the sprite that is created.
*          noteFileName - The path to the sound file that is played when the user clicks the mouse.
*          sndID - The sound ID to assign to this sound resource.
*/

UserBubble::UserBubble(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID, char * noteFileName, int sndID)
          : BubbleSprite(cX, cY, r, bubbleFileName, imgID, sprtID)
{
	setIsUserBubble( true );
	
	setPaused( false );  
	setPopping( false );
	setExpanding( true );
	setContracting( false );

	setMaxRadius( getRadius() + 40 );

	display( getCenterX(), getCenterY() );  // Create initial sprite.

	setOffsetToCenter();

	setHoldCounter( 0 );

	setPopFileName( noteFileName );

	setPopSoundID( sndID );

	loadPopSound( getPopFileName(), getPopSoundID() );
}

/**
*  Function: ~UserBubble()
*
*  Purpose: This is the destructor for the UserBubble class.  It
*           automatically calls the destructor of its base class
*           (BubbleSprite).
*/

UserBubble::~UserBubble()
{
	// Automatically calls the destructor of the base class (BubbleSprite).
}