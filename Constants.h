/**
*  File:  Constants.h                                                                         
*                                                                                            
*  Description:  This header file stores constants all in one place.                          
*                It is imported into Sprite.h.											  
*                                                                                            
*  Author:  jjobes                                                                   
*																							  
*  Last Update:  6/10/2013																	  
*/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/**
*  Application control constants:
*
*  These are primarily used in Prisms.cpp (the main application file),
*  but MAX_ERROR_BUFFER is used in Sprite.cpp, BubbleSprite.cpp and
*  Prisms.cpp to fix the size of the char buffer used to build error
*  strings.
*/

const int REFRESH_RATE = 60;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVELS = 12;
const int TOTAL_POSSIBLE_POINTS = 390;
const int MAX_ERROR_BUFFER = 150;

/**
*  Color constants:
* 
*  These are mainly used to change the color of the displayed text
*  in the cut-screens.
*/

const DWORD BLACK = dbRGB( 0, 0, 0 );
const DWORD WHITE = dbRGB( 255, 255, 255 );
const DWORD BLUE = dbRGB( 32, 171, 229 );
const DWORD YELLOW = dbRGB( 255, 255, 0 );

/**
*  Multimedia resource constants:
*
*  These are used in Prisms.cpp when creating sprites, images and
*  sounds.  For instance, a call to dbSprite() requires a sprite ID
*  and an image ID, and these constants are passed in this and similar
*  cases.
*/

const int SUCCESS_SOUND_ID = 1000;
const int USER_BUBBLE_IMAGE_ID = 1000;
const int USER_BUBBLE_SPRITE_ID = 1000;
const int HAND_POINTER_IMAGE_ID = 1001;
const int HAND_POINTER_SPRITE_ID = 1001;
const int INTRO_PLAY_BUTTON_IMAGE_ID = 1002;
const int INTRO_PLAY_BUTTON_SPRITE_ID = 1002;
const int INSTRUCTIONS_BUTTON_IMAGE_ID = 1003;
const int INSTRUCTIONS_BUTTON_SPRITE_ID = 1003;
const int ABOUT_BUTTON_IMAGE_ID = 1004;
const int ABOUT_BUTTON_SPRITE_ID = 1004;
const int CREDITS_BUTTON_IMAGE_ID = 1005;
const int CREDITS_BUTTON_SPRITE_ID = 1005;
const int OK_BUTTON_IMAGE_ID = 1006;
const int OK_BUTTON_SPRITE_ID = 1006;
const int PLAY_BUTTON_IMAGE_ID = 1007;
const int PLAY_BUTTON_SPRITE_ID = 1007;
const int RETRY_BUTTON_IMAGE_ID = 1008;
const int RETRY_BUTTON_SPRITE_ID = 1008;
const int PLAY_AGAIN_BUTTON_IMAGE_ID = 1009;
const int PLAY_AGAIN_BUTTON_SPRITE_ID = 1009;
const int EXIT_BUTTON_IMAGE_ID = 1010;
const int EXIT_BUTTON_SPRITE_ID = 1010;
const int WHITE_OVERLAY_IMAGE_ID = 1011;
const int WHITE_OVERLAY_SPRITE_ID = 1011;
const int SPACE_BG_IMAGE_ID = 1012;
const int GREEN_BG_IMAGE_ID = 1013;

#endif