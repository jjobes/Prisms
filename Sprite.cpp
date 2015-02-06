/**
*  File:  Sprite.cpp                                                                          
*                                                                                            
*  Description:  This file contains the method implementations for the Sprite class.  
*                A sprite is a graphical image that can be moved and manipulated
*                in various ways.
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/20/2013																	  
*/

#include "Sprite.h"

ofstream logFile;  // Created as Prisms.log in Prisms.cpp's setup()

/**
*  Function: Sprite(int imgID, int sprtID, char fileName[])
*
*  Purpose: This is the constructor for the Sprite class. The actual
*           sprite is created in the constructors of its child classes,
*           ButtonSprite and BubbleSprite.
*
*  Inputs: imgID - the imageID that is used to create the
*                  initial image with dbImage().
*          sprtID - the spriteID that is used to create the
*                   actual sprite with dbSprite().
*          fileName[] - the filename that is also used when
*                       creating the initial sprite.
*/

Sprite::Sprite(int imgID, int sprtID, char fileName[])
{
	imageID = imgID;
	spriteID = sprtID;

	dbLoadImage( fileName, imageID );
}

/**
*  Function: ~Sprite()
*
*  Purpose: This is the destructor for the Sprite class.  It first
*           attempts to delete the sprite from memory and writes an
*           error to the log file if unable to do so.  It then attempts
*           to delete the image from memory, and writes an error to the
*           log file if unsuccessful.
*/

Sprite::~Sprite()
{
	if ( dbSpriteExist( spriteID ) )
	{
		dbDeleteSprite( spriteID );
	}
	else
	{
		sprintf( lineBuffer, "~Sprite(): spriteID %d does not exist.", spriteID );
		logFile << lineBuffer << endl << endl;
	}

	if ( dbImageExist( imageID ) )
	{
		dbDeleteImage( imageID );
	}
	else
	{
		sprintf( lineBuffer, "~Sprite(): imageID %d does not exist.", imageID );
		logFile << lineBuffer << endl << endl;
	}
}

/**
*  Function: display(int cX, int cY)
*
*  Purpose: The first time this function is called, the actual sprite
*           will be created and drawn to the screen.  Subsequent calls
*           will simply draw the sprite to the screen.
*
*  Inputs: cX - The x-coordinate of the center point where the sprite is displayed.
*		   cY - The y-coordinate of the center point where the sprite is displayed.
*/

void Sprite::display(int cX, int cY) const
{
	dbSprite( spriteID, cX, cY, imageID );
}

/**
*  Function: hide()
*
*  Purpose: This function is used to hide the sprite when not in use.  For
*           example, it is used to hide buttons when they are no longer needed.
*/

void Sprite::hide() const
{
	dbHideSprite( spriteID );
}

/**
*  Function: show()
*
*  Purpose: This function is used to show the sprite when it is needed.  For example,
*           it is used to show the buttons when they are needed during the cut-screens.
*/

void Sprite::show() const
{
	dbShowSprite( spriteID );
}

/**
*  Function: resize(int w, int h)
*
*  Purpose: This function is used to resize the sprite.  For example,
*           it is used to resize the size of the bubble sprites in the
*           BubbleSprite constructor.
*
*  Inputs: w - The new width of the sprite in pixels
*          h - The new height of the sprite in pixels
*/

void Sprite::resize(int w, int h)
{
	dbSizeSprite( spriteID, w, h );

	width = dbSpriteWidth( spriteID );
	height = dbSpriteHeight( spriteID );
}

/**
*  Function: rotate(int d)
*
*  Purpose: This function is used to rotate the sprite a number of degrees
*           around its insertion point, which in our case is its center.
*
*  Inputs: d - The number of degrees to rotate the sprite.  Acceptable values
*              are in the range of 0-359.
*/

void Sprite::rotate(int d)
{
	degrees = d;
	dbRotateSprite( spriteID, degrees );
}

/**
*  Function: scale(int s)	
*
*  Purpose: This function is used to expand or contract the entire sprite.
*           It is used to shrink the handPointer image down to size.  I use
*           resize() to shrink the bubble sprites because I can pass the radius
*           to that function, which is more natural when shrinking circles.
*
*  Inputs: s - the percentage of the original image to scale it to.  For example,
*              passing in 50 will shrink the sprite to 50% of its current size.
*              Passing in 150 will grow it to 150% of its current size.
*/

void Sprite::scale(int s) const
{
	dbScaleSprite( spriteID, s );
}

/**
*  Function: setOffsetToCenter()
*
*  Purpose: This function is used to set the positioning point from the default 
*           upper left point to the center point of the sprite.
*/

void Sprite::setOffsetToCenter()
{
	int offsetX = dbSpriteWidth( spriteID ) / 2;
	int offsetY = dbSpriteHeight( spriteID ) / 2;

	dbOffsetSprite( spriteID, offsetX, offsetY );	
}

/**
*  Function: setOffset(int x, int y)
*
*  Purpose: This function is used to set the positioning point of the sprite
*           to the new location that is passed in.
*
*  Inputs: int x - the x-coordinate of the new positioning point
*          int y - the y-coordinate of the new positioning point
*/

void Sprite::setOffset(int x, int y) const
{
	dbOffsetSprite( spriteID, x, y );
}

/**
*  Function: getSpriteID()
*
*  Purpose: This function returns the private member spriteID.  It is used
*           in the ButtonSprite and BubbleSprite constructors.
*/

int Sprite::getSpriteID() const
{
	return spriteID;
}

/**
*  Function: getDegrees()
*
*  Purpose: This function returns the current angle of the sprite in degrees.
*           It is used in Prisms.cpp in boundaryCollisionCheck() to set the 
*           new direction of the sprite movement after it hits a wall.
*
*  Returns: The current angle of the sprite in degrees.
*/

int Sprite::getDegrees() const
{
	return degrees;
}

/**
*  Function: setWidth(int w)
*
*  Purpose: This function sets the width of the sprite to the
*           value passed to it.  It is used in ButtonSprite's
*           constructor.
*
*  Inputs: int w - The width of the sprite.
*/

void Sprite::setWidth(int w) 
{
	width = w;
}

/**
*  Function: getWidth()
*
*  Purpose: This function returns the current width of the sprite.
*           It is used in ButtonSprite's mouseOver() function.
*
*  Returns: The current width of the sprite.
*/

int Sprite::getWidth() const 
{
	return width;
}

/**
*  Function: setHeight(int h)
*
*  Purpose: This function sets the height of the sprite to the
*           value passed to it.  It is used in ButtonSprite's
*           constructor.
*
*  Inputs: int h - The height of the sprite.
*/

void Sprite::setHeight(int h)  
{
	height = h;
}

/**
*  Function: getHeight()
*
*  Purpose: This function returns the current height of the sprite.
*           It is used in ButtonSprite's mouseOver() function.
*
*  Returns: The current height of the sprite.
*/

int Sprite::getHeight() const  
{
	return height;
}

/**
*  Function: setAlpha(int a)
*
*  Purpose: This function sets the alpha (transparency) channel of the sprite.
*           It is used in BubbleSprite::pop() to make the bubble transparent 
*           during its expand/contract sequence.  It is also used in Prisms.cpp
*           with the whiteOverlay image to gradually fade in once the user has
*           popped enough bubbles to win the level.
*
*  Inputs: a - The alpha value that determines the level of transparency of the 
*              sprite.  Acceptible values are in the range of 0-255.  The default
*              value is 255, which is completely opaque.  A value of 0 makes the 
*              sprite completely invisible, and any values in between make the 
*              sprite semitransparent.  The function ensures that the value is
*              within range (0-255), and sets the alpha channel to 0 otherwise.
*/

void Sprite::setAlpha(int a)
{
	if ( a >= 0 && a <= 255 )  // Make sure value is within range.
	{
		dbSetSpriteAlpha( spriteID, a );
	}
	else
	{
		dbSetSpriteAlpha( spriteID, 0 );  // Set to 0 (completely invisible) otherwise.
	}
}