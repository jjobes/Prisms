/**
*  File:  NormalBubble.cpp                                                                    
*                                                                                            
*  Description:	 This file contains the method implementations for the NormalBubble class.    
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/20/2013																	  
*/

#include "NormalBubble.h"

/**
*  Function: NormalBubble(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID, char * noteFileName, int sndID)
*
*  Purpose: This is the constructor for the NormalBubble class.  It is called
*           from createAndLoadBubbles() in Prisms.cpp.  The incoming values are
*           all passed up to the BubbleSprite constructor.
*
*  Inputs: cX - A randomly chosen x-value for the bubble's starting location center point.
*          cY - A randomly chosen y-value for the bubble's starting location center point.
*          r - The radius that is passed in (12).
*          bubbleFileName - The path to the bubble's .png image file.
*          imgID - The image ID to assign to this image resource.
*          sprtID - The sprite ID to assign to the sprite that is created.
*          noteFileName - The path to the sound file that is played when bubble is popped.
*          sndID - The sound ID to assign to this sound resource.
*/

NormalBubble::NormalBubble(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID, char * noteFileName, int sndID)
            : BubbleSprite(cX, cY, r, bubbleFileName, imgID, sprtID)
{
	setIsUserBubble( false );

	setPaused( false ); 
	setPopping( false );
	setExpanding( true );
	setContracting( false );

	setMaxRadius( getRadius() + 40 );  // Set max radius - the max radius that the bubble expands to.

	display( getCenterX(), getCenterY() );  // Create initial sprite.

	resize( 24, 24 );  // Resize sprite to 24x24 px.

	dbRandomize( dbTimer() + 91 );
	rotate( dbRND( 310 ) + 30 );  // Starting angle from 30-340 degrees.

	setOffsetToCenter();

	setCenterX( dbSpriteX( getSpriteID() ) );
	setCenterY( dbSpriteY( getSpriteID() ) );

	setHoldCounter( 0 );

	setPopFileName( noteFileName );

	setPopSoundID( sndID );

	loadPopSound( getPopFileName(), getPopSoundID() );
}

/**
*  Function: ~NormalBubble()
*
*  Purpose: This is the destructor for the NormalBubble class.  It
*           automatically calls the destructor of its base class
*           (BubbleSprite).
*/

NormalBubble::~NormalBubble()
{
	// Automatically calls the destructor of the base class (BubbleSprite).
}