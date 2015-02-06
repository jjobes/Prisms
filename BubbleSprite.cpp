/**
*  File:  BubbleSprite.cpp                                                                    
*                                                                                            
*  Description:	 This file contains the method implementations for the BubbleSprite class.    
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/20/2013																	  
*/

#include "BubbleSprite.h"

extern ofstream logFile;

/**
*  Function: BubbleSprite(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID, char * noteFileName, int sndID)
*
*  Purpose: This is the constructor for the BubbleSprite class.  It is called by
*           the UserBubble and NormalBubble constructors when creating their objects.
*           Its primary purpose is to create the initial sprite.
*
*  Inputs: imgID, sprtID, bubbleFileName, cX, cY, r - passed up to the CircleSprite constructor.
*          noteFileName - The filename of the pop sound that is used to load the sound.
*          sndID - The ID assigned to the pop sound that is used to load the sound.
*/

BubbleSprite::BubbleSprite(int cX, int cY, int r, char * bubbleFileName, int imgID, int sprtID)
            : CircleSprite(cX, cY, r, bubbleFileName, imgID, sprtID)
{
	// Pass all incoming values up to the CircleSprite constructor.
}

/**
*  Function: ~BubbleSprite()
*
*  Purpose: This is the destructor for the BubbleSprite class.  It makes sure
*           the pop sound exists, and then deletes it from memory.  It writes
*           an error to the log file otherwise.
*/

BubbleSprite::~BubbleSprite()
{
	if ( dbSoundExist( popSoundID ) )
	{
		dbDeleteSound( popSoundID );
	}
	else
	{
		sprintf( lineBuffer, "~BubbleSprite(): popSoundID %d does not exist.", popSoundID );
		logFile << lineBuffer << endl << endl;
	}
}

/**
*  Function: loadPopSound()
*
*  Purpose: This function loads the pop sound into memory.  It is called by the
*           UserBubble and NormalBubble constructors.  The reason each object has 
*           its own pop sound is that this enables each bubble to have a different 
*           note.  It writes an error to the log file if the sound file is not found.
*
*  Inputs: fileName - The file path and name of the pop sound to be loaded.
*          soundID - The ID to be associated with this sound.
*/

void BubbleSprite::loadPopSound(char * fileName, int soundID)
{
	if ( dbFileExist( fileName ) )
	{
		dbLoadSound( fileName, soundID );
	}
	else
	{
		sprintf( lineBuffer, "BubbleSprite::loadPopSound(): File %s does not exist.", fileName );
		logFile << lineBuffer << endl << endl;
	}	
}

/**
*  Function: playPopSound()
*
*  Purpose: This function makes sure the pop sound exists, and then
*           plays the sound.  If the sound hasn't been loaded into
*           memory, it writes an error to the log file.
*/

void BubbleSprite::playPopSound()
{
	if ( dbSoundExist( popSoundID ) )
	{
		dbPlaySound( popSoundID );
	}
	else
	{
		sprintf( lineBuffer, "BubbleSprite::playPopSound(): popSoundID %d does not exist.", popSoundID );
		logFile << lineBuffer << endl << endl;
	}
}

/**
*  Function: move()
*
*  Purpose: This function moves the normal bubbles (the userBubble doesn't
*           move - it just expands/contracts).  It moves the sprite one pixel 
*           in the current direction it is facing.  It then updates the center 
*           point of the sprite.
*/

void BubbleSprite::move()
{
	if ( paused == false )
	{
		dbMoveSprite( getSpriteID(), 1.0 );  // Move sprite 1 pixel at current angle

		setCenterX( dbSpriteX( getSpriteID() ) );  // Update centerX and centerY for detecting collisions
		setCenterY( dbSpriteY( getSpriteID() ) );
	}
}

/**
*  Function: pop()
*
*  Purpose: This is a control function that determines whether to call expand(), hold() 
*           or contract() based on the state of the expanding, holding and contracting 
*           flags and also the radius.  It expands the bubble to its maxRadius, holds at 
*           the maxRadius, then contracts it to 0.  It calls setAlpha() to ensure the
*           bubble is transparent while it is in the expand/contract process.
*/

void BubbleSprite::pop()
{
	setAlpha( 140 );  // Make sprite semi-transparent.

	if ( popping == true )
	{
		if ( getRadius() == maxRadius )
		{
			expanding = false;
			holding = true;
		}

		if ( expanding == true && getRadius() < maxRadius )
		{
			expand();
		}
		
		if ( holding == true )
		{
			hold();
		}
		
		if ( contracting == true && getRadius() > 0 )
		{
			contract();
		}
	}
}

/**
*  Function: expand()
*
*  Purpose: This is called by pop().  It increases the radius by 1 pixel, then
*           resizes the sprite with that new radius.  It then recalculates the 
*           center offset to ensure that we're pasting the sprite around the
*           new center.  Finally, it displays the sprite at the new location. 
*/

void BubbleSprite::expand()
{
	setRadius( getRadius() + 1 );
		
	resize( getRadius()*2, getRadius()*2 );

	setOffsetToCenter();

	display( getCenterX(), getCenterY() );
}

/**
*  Function: hold()
*
*  Purpose: This is called by pop().  Each time called, it increases
*           holdCounter by 1 until it reaches 90 (roughly 1.5 seconds).
*           At that point, it sets the holding flag to false and the 
*           contracting flag to true, signalling to pop() to now begin
*           contracting the bubble.  Visually, this has the effect of 
*           the bubble holding its maximum size momentarily before
*           contracting.
*/

void BubbleSprite::hold()
{
	if ( holdCounter < 90 )  // Hold max radius for 1.5 seconds.
	{
		holdCounter++;
	}
	else
	{
		holding = false;
		contracting = true;
	}
}

/**
*  Function: contract()
*
*  Purpose: This is called by pop().  Each time it is called, it decrements the
*           radius by 1.  It does so until the radius reaches 0, at which point 
*           it sets the popping flag to false to signal to pop() to stop the 
*           entire process for this bubble.  It also decrements the static 
*           currentPopping variable at this point, which helps Prisms.cpp determine 
*           when the chain reaction has finished.
*/

void BubbleSprite::contract()
{
	setRadius( getRadius() - 1 );

	if ( getRadius() == 0 && popping == true )
	{
		popping = false;
		BubbleSprite::currentPopping--;
	}
		
	resize( getRadius()*2, getRadius()*2 );

	setOffsetToCenter();

	display( getCenterX(), getCenterY() );
}

/**
*  Function: setPaused(bool p)
*
*  Purpose: This is called from Prisms.cpp's detectUserClick() and detectBubbleCollisions().
*           They pause the bubble (stop it from moving) when the userBubble is created, and when
*           each bubble gets hit by another bubble.  Pausing the bubble only stops it from
*           moving.  It can still expand/contract.
*
*  Inputs: p - The boolean value that paused gets set to.
*/

void BubbleSprite::setPaused(bool p)
{
	paused = p;
}

/**
*  Function: setPopping(bool p)
*
*  Purpose: This is called from Prisms.cpp's detectUserClick() and detectBubbleCollisions().
*          
*  Inputs: p - The boolean value that popping gets set to.
*/

void BubbleSprite::setPopping(bool p)
{
	popping = p;
}

/**
*  Function: getPopping()
*
*  Purpose: This is called in Prisms.cpp by detectBoundaryCollisions() and detectBubbleCollisions().
*           detectBoundaryCollisions() only checks for a boundary collision if popping is false.
*           detectBubbleCollisions() only checks for collisions between bubbles if one of these bubbles
*           has popping set to true, as only popping bubbles can pop other non-popping bubbles.
*
*  Returns: True if popping is set to true, and false if popping is set to false.
*/

bool BubbleSprite::getPopping() const
{
	return popping;
}

/**
*  Function: setExpanding(bool e)
*
*  Purpose: This function sets expanding to the passed in value.  It is
*           called from the NormalBubble and UserBubble constructors.
*          
*  Inputs: e - The boolean value that expanding gets set to.
*/

void BubbleSprite::setExpanding(bool e)
{
	expanding = e;
}

/**
*  Function: setContracting(bool c)
*
*  Purpose: This function sets contracting to the passed in value.  It is
*           called from the NormalBubble and UserBubble constructors.
*          
*  Inputs: c - The boolean value that contracting gets set to.
*/

void BubbleSprite::setContracting(bool c)
{
	contracting = c;
}

/**
*  Function: setIsUserBubble(bool u)
*
*  Purpose: This function sets isUserBubble to the passed in value.  It is
*           called from the NormalBubble and UserBubble constructors.
*          
*  Inputs: u - The boolean value that isUserBubble gets set to.
*/

void BubbleSprite::setIsUserBubble(bool u)
{
	isUserBubble = u;
}

/**
*  Function: setMaxRadius(bool m)
*
*  Purpose: This function sets maxRadius to the passed in value.  It is
*           called from the NormalBubble and UserBubble constructors.
*          
*  Inputs: m - The boolean value that maxRadius gets set to.
*/

void BubbleSprite::setMaxRadius(int m)
{
	maxRadius = m;
}

/**
*  Function: setHoldCounter(bool h)
*
*  Purpose: This function sets holdCounter to the passed in value.  It is
*           called from the NormalBubble and UserBubble constructors.
*          
*  Inputs: h - The boolean value that holdCounter gets set to.
*/

void BubbleSprite::setHoldCounter(int h)
{
	holdCounter = h;
}

/**
*  Function: setPopSoundID(bool id)
*
*  Purpose: This function sets popSoundID to the passed in value.  It is
*           called from the NormalBubble and UserBubble constructors.
*          
*  Inputs: id - The boolean value that popSoundID gets set to.
*/

void BubbleSprite::setPopSoundID(int id)
{
	popSoundID = id;
}

/**
*  Function: getPopSoundID()
*
*  Purpose: This function returns the popSoundID.  It is called from
*           the NormalBubble and UserBubble constructors.
*
*  Returns: The popSoundID of the current BubbleSprite.
*/

int BubbleSprite::getPopSoundID() const
{
	return popSoundID;
}

/**
*  Function: setPopFileName(char * name)
*
*  Purpose: This function sets popFileName to the passed in value.  The 
*           popFileName is the path and name of the file that is used to 
*           create the pop sound.  It is called from the NormalBubble and 
*           UserBubble constructors.
*          
*  Inputs: name - The char * value that popFileName gets set to.
*/

void BubbleSprite::setPopFileName(char * name)
{
	popFileName = name;
}

/**
*  Function: getPopFileName()
*
*  Purpose: This function returns the popFileName.  The popFileName is
*           the path and name of the file that is used to create the pop
*           sound.  It is called from the NormalBubble and UserBubble 
*           constructors.
*
*  Returns: The popFileName of the current BubbleSprite.
*/

char * BubbleSprite::getPopFileName() const
{
	return popFileName;
}

/**
*  Function: getIsUserBubble()
*
*  Purpose: This function is called from Prisms.cpp's detectBoundaryCollisions(), as 
*           detectBoundaryCollisions() only checks if normal bubbles are colliding with 
*           the side walls, and ignores the userBubble.  isUserBubble is set to true
*           in the UserBubble constructor, and is set to false in the NormalBubble
*           constructor.
*
*  Returns: True if the current bubble is the userBubble, and false if it is a normal bubble.
*/

bool BubbleSprite::getIsUserBubble() const
{
	return isUserBubble;
}