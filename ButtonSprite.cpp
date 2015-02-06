/**
*  File:  ButtonSprite.cpp                                                                    
*                                                                                            
*  Description:  This file contains the method implementations for the ButtonSprite class.    
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/20/2013																	  
*/

#include "ButtonSprite.h"

/**
*  Function: ButtonSprite(int imgID, int sprtID, char fileName[], int cX, int cY)
*
*  Purpose: This is the constructor for the ButtonSprite class.  It sets the x- and
*           y-coordinates of the center point, creates the initial sprite at that 
*           point, then sets the offset to its center point.  Subsequent calls to 
*           reposition the button will treat the arguments passed in as the button's
*           new center point.
*
*  Inputs: imgID, sprtID, fileName - passed up to the Sprite constructor.
*          cX - The x-coordinate of the ButtonSprite's center point.
*		   cY - The y-coordinate of the ButtonSprite's center point.
*/

ButtonSprite::ButtonSprite(int imgID, int sprtID, char fileName[], int cX, int cY) 
            : Sprite(imgID, sprtID, fileName)
{
	centerX = cX;
	centerY = cY;

	display( centerX, centerY );  // Create initial sprite and display it.

	setOffsetToCenter();

	setWidth( dbSpriteWidth( getSpriteID() ) );
	setHeight( dbSpriteHeight( getSpriteID() ) );
}

/**
*  Function: ~ButtonSprite()
*
*  Purpose: This is the destructor for the ButtonSprite class.  It is currently
*           empty, but is here in case any new additions to the class require
*           objects to be deleted, or memory to be freed.
*/

ButtonSprite::~ButtonSprite()
{
	// Destructor of base class (Sprite) is automatically called.
}

/**
*  Function: reposition(int cX, int cY)
*
*  Purpose: This function is used to reposition the button in a new location.
*           This is useful because in the cut-screen functions in Prisms.cpp,
*           the same button might be in a different location in two different
*           screens.
*
*  Inputs: cX - The x-coordinate of the button's new center point.
*          cY - The y-coordinate of the button's new center point.
*/

void ButtonSprite::reposition(int cX, int cY)
{
	centerX = cX;
	centerY = cY;

	display( centerX, centerY ); 
	hide();

	setOffsetToCenter();                 
}

/**
*  Function: mouseOver(int mouseX, int mouseY)
*
*  Purpose: This function is called from the cut-screen functions in Prisms.cpp.
*           It is used to determine whether or not the user's mouse is currently
*           hovering over the button.
*
*  Inputs: mouseX - The current x-coordinate of the user's mouse pointer.
*          mouseY - The current y-coordinate of the user's mouse pointer.
*
*  Returns: True if the mouse pointer is currently over the button.
*           False if the mouse pointer is not currently over the button.
*/

bool ButtonSprite::mouseOver(int mouseX, int mouseY)
{
	int halfWidth = getWidth() / 2;
	int halfHeight = getHeight() / 2;

	if ( mouseX >= centerX-halfWidth && mouseX <= centerX+halfWidth &&
		 mouseY >= centerY-halfHeight && mouseY <= centerY+halfHeight )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
*  Function: clicked(int mouseX, int mouseY)
*
*  Purpose: This function is used to determine whether or not the button
*           has been clicked by the user.
*
*  Inputs: mouseX - The x-coordinate of the user's current mouse position.
*          mouseY - The y-coordinate of the user's current mouse position.
*
*  Returns: True if the user clicked within the area of the button.
*           False if the user clicked outside of the button.
*/

bool ButtonSprite::clicked(int mouseX, int mouseY)
{
	if ( mouseOver( mouseX, mouseY ) )
	{
		if ( dbMouseClick() == 1 )
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}