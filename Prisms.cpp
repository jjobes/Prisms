/**
*  File:  Prisms.cpp                                                                      
*                                                                                        
*  Description:  This is the main application file.  The heart of the program takes place              
*                in the while( LoopGDK() ) loop - the main game loop.  
*
*				 The first half of the game loop continuously draws the background, updates 
*                the positions of the bubbles and draws them to the screen.  It also checks 
*                for collisions between bubbles, and for collisions between the bubbles and 
*                the screen edges.  It also listens for the user to click the mouse, and 
*                creates the userBubble on the screen at that point.    At the end of each 
*                iteration through the loop, it redraws the screen with dbSync().  
*
*				 The second half begins by checking whether or not enough bubbles have 
*                popped to win the level, and if so fades the screen semi-white.  It then
*                checks if the chain reaction is over.  If the chain reaction is done, it
*                shrinks all of the remaining bubbles on the screen.  It also then checks
*                if the user has won or lost the level, and invokes a different cut-screen
*                function depending on the outcome.
*
*				 Note on compiling: This was compiled using Visual C++ 2008 Express.  In
*                order for it to compile correctly with the <vector> include, you must modify
*                the VC++ settings:  Project -> Project Name Properties -> Configuration 
*                Properties ->  C/C++ Code Generation -> then change the Runtime Library from 
*                /MTd (Multi-threaded debug) to /MT (Multi-threaded).                    
*																						   																					  
*  Author:  jjobes                                                               
*                                                                                        
*  Last Update:  6/21/2013                                                                              
*/

#include "MediaFilePaths.h"
#include "NormalBubble.h"
#include "UserBubble.h"
#include "ButtonSprite.h"
#include <cmath>   // For sqrt() and pow().
#include <vector>  // To hold pointers to BubbleSprite objects.

// Used for error reporting
extern ofstream logFile;
char lineBuffer[MAX_ERROR_BUFFER];
time_t currentTime;

// Function prototypes
void setup();  
void loadImageFile(char * fileName, int id);
void loadSoundFile(char * fileName, int id);
void introScreen();
void instructionsScreen();
void aboutScreen();
void closeWindowListener();
void escapeKeyListener();
void levelStartScreen();
void showAllBubbleSprites();
void retryScreen();
void wonGameScreen();
void resetLevel();
void createAndLoadBubbles();
void shuffle(char * filenames[], int size);
void swap(int & x, int & y);
void detectUserClick();
void detectBoundaryCollisions();
void boundaryCollisionCheck( BubbleSprite & b );
void detectBubbleCollisions();
bool bubbleCollisionCheck( BubbleSprite & b1, BubbleSprite & b2 );
void popUserBubble();
void moveBubbles();
void popBubbles();
void pauseBubbles();
void contractRemainingBubbles();
void exitGame();
void deleteComponents();
void deleteBubbles();

int BubbleSprite::currentPopping = 0;  // Initialize the static variable.

int halfScreenWidth;
int halfScreenHeight;

int level = 1;
int levelPoints = 0;
int totalPoints = 0;
int fadeCounter = 0;

int levelBubbles[LEVELS] =  { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60 };
int goalPoints[LEVELS]   =  { 1,  2,  3,  5,  7, 10, 15, 21, 27, 33, 44, 55 };

Sprite * whiteOverlay;
Sprite * handPointer;

BubbleSprite * userBubble;

ButtonSprite * introPlayButton;
ButtonSprite * instructionsButton;
ButtonSprite * aboutButton;
ButtonSprite * okButton;
ButtonSprite * playButton;
ButtonSprite * retryButton;
ButtonSprite * playAgainButton;
ButtonSprite * exitButton;

vector <BubbleSprite*> bubbles;  // A vector is used in order to easily add the
                                 // userBubble when it is created by the user.

bool userBubbleCreated = false;  // Set in detectUserClick() and checked at top of 
                                 // detectBubbleCollisions() to see if we should even be
                                 // checking for collisions.  Also makes sure we don't
                                 // accept another user click.

int mouseX;  // Where the user first clicks the mouse, and where
int mouseY;  // we create their bubble (userBubble).
                     
bool fadeToWhite = false;
bool fadeComplete = false;
bool userBubbleContracting = false;
bool chainReactionStarted = false;
bool chainReactionDone = false;
bool remainingBubblesContracted = false;
bool wonLevel = false;
bool musicStarted = false;

char * bubbleFileNames[60] = { red1_png, red2_png, red3_png, red4_png, red5_png, red6_png, red7_png, purple1_png, 
							   purple2_png, purple3_png, purple4_png, blue1_png, blue2_png, blue3_png, blue4_png, 
							   blue5_png, blue6_png, blue7_png, blue8_png, blue9_png, blue10_png, green1_png, 
							   green2_png, green3_png, green4_png, green5_png, green6_png, green7_png, green8_png, 
							   green9_png, brown1_png, brown2_png, brown3_png, brown4_png, orange1_png, orange2_png, 
							   orange3_png, orange4_png, orange5_png, orange6_png, orange7_png, orange8_png, yellow1_png, 
							   yellow2_png, yellow3_png, yellow4_png, yellow5_png, yellow6_png, yellow7_png, tan1_png,
							   tan2_png, tan3_png, tan4_png, gray1_png, pink1_png, pink2_png, pink3_png, pink4_png, 
							   pink5_png, pink6_png };

char * noteFileNames[6] = { note1_wav, note2_wav, note3_wav, note4_wav, note5_wav, note6_wav };

/**
*  Function: DarkGDK()
*
*  Purpose: This is the main function that is run automatically when the program
*           starts.  It contains the main game loop ( while( LoopGDK() ) that 
*           executes a maximum of 60 times per second.  This is the main loop that
*           continuously checks for input, performs collision detection, and 
*           renders the output to the screen.
*/

void DarkGDK()
{
	setup();  

	introScreen();

	createAndLoadBubbles();

	dbShowMouse();   

	dbSyncOn();                  // Disable auto refresh.
	dbSyncRate( REFRESH_RATE );  // Set refresh rate to max 60 times per second.

	while ( LoopGDK() )
	{
		dbPasteImage( SPACE_BG_IMAGE_ID, 0, 0 );   // Paste background image at (0, 0).
		whiteOverlay->display( 0, 0 );             // Paste the white overlay (normally transparent).
		detectUserClick();                         // Start initial pop for user bubble?
		detectBoundaryCollisions();                // Check for bubbles colliding with walls.
		popUserBubble();                           // If necessary, continue pop animation for userBubble.
		popBubbles();                              // If necessary, pop animation for normal bubbles.
		moveBubbles();                             // Move animation for normal bubbles.
		detectBubbleCollisions();                  // Search for collisions.

		if ( levelPoints >= goalPoints[level-1] && fadeComplete == false )  // Have enough bubbles popped to win the level?
		{
			fadeToWhite = true;
		}

		if ( fadeToWhite == true )
		{
			fadeCounter++;

			whiteOverlay->setAlpha( fadeCounter );
			whiteOverlay->show();

			if ( fadeCounter >= 60 )
			{
				fadeComplete = true;
				fadeToWhite = false;
				fadeCounter = 0;
			}
		}

		if ( chainReactionStarted == true && BubbleSprite::currentPopping == 0 )  // Is the initial chain reaction over?
		{
			chainReactionDone = true;
			pauseBubbles();
		}

		if ( chainReactionDone )  // Is the chain reaction done?
		{
			if ( remainingBubblesContracted == false )  // Still need to contract remaining bubbles on the screen.
			{
				contractRemainingBubbles();

				int count = 0;

				for ( int i = 0; i < bubbles.size(); i++ )
				{
					if ( bubbles[i]->getRadius() <= 0 )
					{
						count++;
					}
				}

				if ( count == bubbles.size() )  // Have all remaining bubbles contracted?
				{
					remainingBubblesContracted = true;
				}
			}
			else  // Remaining bubbles done contracting, so check to see if you've won or lost level.
			{
				if ( levelPoints >= goalPoints[level-1] )  // Won level.
				{
					if ( level == LEVELS )  // Won entire game.
					{
						totalPoints += levelPoints;
						level = 1;  
						resetLevel();
						wonGameScreen();
					}
					else  // Won normal level.
					{
						wonLevel = true;  // Used for output.
						totalPoints += levelPoints;
						level++;
						resetLevel();
						levelStartScreen();
					}
				}
				else if ( levelPoints < goalPoints[level-1] )  // Lost level.
				{
					resetLevel();
					retryScreen();
				}
			}
		}
	
		dbSync();  // Refresh screen.
	}

	exitGame();
}

/**
*  Function: setup()
*
*  Purpose: This function is called once when the program first starts up. It performs
*           general setup functions.  It creates the log file, initializes display
*           settings, loads multimedia resources and creates sprites.
*/

void setup()
{
	logFile.open("Prisms.log");

	time( & currentTime );
	sprintf( lineBuffer, "%s Prisms.cpp: Game started.", ctime( & currentTime ) );
	logFile << lineBuffer << endl << endl;

	dbSetWindowTitle( "Prisms" );

	int colorDepth = dbScreenDepth();

	dbSetDisplayMode( SCREEN_WIDTH, SCREEN_HEIGHT, colorDepth );

	halfScreenWidth = SCREEN_WIDTH / 2;
	halfScreenHeight = SCREEN_HEIGHT / 2;

	loadImageFile( background_space_bmp, SPACE_BG_IMAGE_ID ); 

	loadImageFile( background_green_bmp, GREEN_BG_IMAGE_ID );

	whiteOverlay = new Sprite( WHITE_OVERLAY_IMAGE_ID, WHITE_OVERLAY_SPRITE_ID, background_overlay_bmp );
	whiteOverlay->display(0, 0);
	whiteOverlay->setAlpha( 0 );
	whiteOverlay->hide();

	loadSoundFile( success_wav, SUCCESS_SOUND_ID );

	playButton = new ButtonSprite(PLAY_BUTTON_IMAGE_ID, PLAY_BUTTON_SPRITE_ID, 
		                          play_button_png, halfScreenWidth, halfScreenHeight);
	playButton->hide();

	introPlayButton = new ButtonSprite(INTRO_PLAY_BUTTON_IMAGE_ID, INTRO_PLAY_BUTTON_SPRITE_ID, 
		                               play_button_png, halfScreenWidth, 140);
	introPlayButton->hide();

	instructionsButton = new ButtonSprite(INSTRUCTIONS_BUTTON_IMAGE_ID, INSTRUCTIONS_BUTTON_SPRITE_ID,
		                                  instructions_button_png, halfScreenWidth, 210);

	instructionsButton->hide();

	aboutButton = new ButtonSprite(ABOUT_BUTTON_IMAGE_ID, ABOUT_BUTTON_SPRITE_ID,
								   about_button_png, halfScreenWidth, 280);

	aboutButton->hide();

	okButton = new ButtonSprite(OK_BUTTON_IMAGE_ID, OK_BUTTON_SPRITE_ID,
		                        ok_button_png, halfScreenWidth, 300);

	okButton->hide();

	retryButton = new ButtonSprite(RETRY_BUTTON_IMAGE_ID, RETRY_BUTTON_SPRITE_ID, 
		                           retry_button_png, halfScreenWidth, 200 );
	retryButton->hide();

	playAgainButton = new ButtonSprite(PLAY_AGAIN_BUTTON_IMAGE_ID, PLAY_AGAIN_BUTTON_SPRITE_ID, 
		                               play_again_button_png, halfScreenWidth, halfScreenHeight);
	playAgainButton->hide();

	exitButton = new ButtonSprite(EXIT_BUTTON_IMAGE_ID, EXIT_BUTTON_SPRITE_ID, 
		                          exit_button_png, halfScreenWidth, halfScreenHeight+100);
	exitButton->hide();
	
	handPointer = new Sprite( HAND_POINTER_IMAGE_ID, HAND_POINTER_SPRITE_ID, hand_pointer_png );
	handPointer->display( dbMouseX(), dbMouseY() );
	handPointer->scale( 20 );
	handPointer->setOffset( 33, 0 );
}

/**
*  Function: loadImageFile(char * fileName, int id)
*
*  Purpose: This function safely loads an image file, and writes an error to
*           the log file if it does not succeed.
*
*  Inputs: fileName - The file path, relative to the current game directory.
*          id - The ID passed to dbLoadImage(), that will be associated with
*               this image resource.
*/

void loadImageFile(char * fileName, int id)
{
	if ( dbFileExist( fileName ) )
	{
		dbLoadImage( fileName, id );
	}
	else
	{
		sprintf( lineBuffer, "Prisms.cpp: loadImageFile(): File %s does not exist.", fileName );
		logFile << lineBuffer << endl << endl;
	}
}

/**
*  Function: loadSoundFile(char * fileName, int id)
*
*  Purpose: This function safely loads a sound file, and writes an error to
*           the log file if it does not succeed.
*
*  Inputs: fileName - The file path, relative to the current game directory.
*          id - The ID passed to dbLoadSound(), that will be associated with
*               this sound resource.
*/

void loadSoundFile(char * fileName, int id)
{
	if ( dbFileExist( fileName ) )
	{
		dbLoadSound( fileName, id );
	}
	else
	{
		sprintf( lineBuffer, "Prisms.cpp: loadSoundFile(): File %s does not exist.", fileName );
		logFile << lineBuffer << endl << endl;
	}
}

/**
*  Function: introScreen()
*
*  Purpose: This function draws the Intro screen that appears at the start
*           of the game.  It checks to see if the user's mouse is hovering
*           over any of the buttons, and changes the normal arrow pointer
*           to a hand pointer.  It also determines which button the user has
*           clicked, and calls the function to draw that screen.  It also
*           calls listeners that check if the user has exited the program by 
*           clicking the X button in the top right corner, or if they have 
*           pressed the Esc key.
*/

void introScreen()
{
	bool buttonClicked = false;
	bool introPlayButtonClicked = false;
	bool instructionsButtonClicked = false;
	bool aboutButtonClicked = false;
	bool exitButtonClicked = false;

	int y1 = 40;

	exitButton->reposition( halfScreenWidth, 350 );

	dbInk( YELLOW, WHITE );

	while ( buttonClicked == false )
	{
		closeWindowListener();  // Check to see if user closes the window.

		escapeKeyListener();  
		
		dbPasteImage( GREEN_BG_IMAGE_ID, 0, 0 ); 

		dbSetTextSize(40);
		dbCenterText( halfScreenWidth, y1, "Prisms" );

		introPlayButton->show();
		instructionsButton->show();
		aboutButton->show();
		exitButton->show();

		if ( introPlayButton->mouseOver( dbMouseX(), dbMouseY() ) || 
			 instructionsButton->mouseOver( dbMouseX(), dbMouseY() ) ||
			 aboutButton->mouseOver( dbMouseX(), dbMouseY() ) ||
			 exitButton->mouseOver( dbMouseX(), dbMouseY() ) )
		{
			dbHideMouse();
			handPointer->display( dbMouseX(), dbMouseY() );
			handPointer->show();
		}
		else
		{
			dbShowMouse();
			handPointer->hide();
		}

		if ( introPlayButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			introPlayButtonClicked = true;
		}
		else if ( instructionsButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			instructionsButtonClicked = true;
		}
		else if ( aboutButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			aboutButtonClicked = true;
		}
		else if ( exitButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			exitButtonClicked = true;
		}

		dbSync();  
	}

	introPlayButton->hide();
	instructionsButton->hide();
	aboutButton->hide();
	exitButton->hide();
	exitButton->reposition( halfScreenWidth, halfScreenHeight+100 );
	handPointer->hide();

	while ( dbMouseClick() == 1 )
	{
		// Absorb last mouse click.
	}

	if ( introPlayButtonClicked == true )
	{
		levelStartScreen();
	}
	else if ( instructionsButtonClicked == true )
	{
		instructionsScreen();
	}
	else if ( aboutButtonClicked == true )
	{
		aboutScreen();
	}
	else if ( exitButtonClicked == true )
	{
		exitGame();
	}
}

/**
*  Function: levelStartScreen()
*
*  Purpose: This function draws the screen that appears at the start of
*           each level.  It checks to see if the user's mouse is hovering
*           over the Play button, and changes the normal arrow pointer
*           to a hand pointer.  If the Play button is clicked, the screen
*           contents are hidden, showAllBubbleSprites() is called, control
*           is passed to the main game loop and the level begins.  It also
*           calls listeners that check if the user has exited the program 
*           by clicking the X button in the top right corner, or if they 
*           have pressed the Esc key.
*/

void levelStartScreen()
{
	bool buttonClicked = false;

	dbSetTextSize(40);

	char buffer1[15];  

	if ( level == LEVELS )
	{
		sprintf( buffer1, "Final Level" );
	}
	else
	{
		sprintf( buffer1, "Level %d / %d", level, LEVELS );  
	}
	
	char buffer2[20];
	sprintf( buffer2, "Goal: %d out of %d", goalPoints[level-1], levelBubbles[level-1] );

	char buffer3[20]; 
	sprintf( buffer3, "Total Score: %d", totalPoints );  

	int y1 = 50;
	int y2 = 100;
	int y3 = 150;
	int y4 = 400;
	
	while ( buttonClicked == false )
	{
		closeWindowListener();  // Check to see if user closes the window.

		escapeKeyListener();

		dbPasteImage( GREEN_BG_IMAGE_ID, 0, 0 );

		if ( wonLevel == true )
		{
			dbInk( BLUE, BLUE );
			dbCenterText( halfScreenWidth, y1, "Good job!" );
		}
		
		dbInk( YELLOW, YELLOW );
		dbCenterText( halfScreenWidth, y2, buffer1 );  // Level %d  

		dbInk( YELLOW, YELLOW );
		dbCenterText( halfScreenWidth, y3, buffer2 );  // Goal: %d out of %d

		playButton->show();                            // Play Button

		dbCenterText( halfScreenWidth, y4, buffer3 );  // Total Score: %d

		if ( playButton->mouseOver( dbMouseX(), dbMouseY() ) )
		{
			dbHideMouse();
			handPointer->display( dbMouseX(), dbMouseY() );
			handPointer->show();
		}
		else
		{
			dbShowMouse();
			handPointer->hide();
		}

		if ( playButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			handPointer->hide();
		}

		dbSync(); 
	}

	showAllBubbleSprites();    

	playButton->hide();
	handPointer->hide();

	dbShowMouse(); // Show normal mouse.

	while ( dbMouseClick() == 1 )
	{
		// Absorb last mouse click.
	}

	levelPoints = 0;  // Reset levelPoints here so we still have access to it for output above.
	wonLevel = false;
}

/**
*  Function: showAllBubbleSprites()
*
*  Purpose: This function renders each of the bubble sprites to the screen
*           that are currently stored in the bubbles vector.  It is called
*           at the end of levelStartScreen().
*/

void showAllBubbleSprites()
{
	for ( int i = 0; i < bubbles.size(); i++ )
	{
		bubbles[i]->show();
	}
}

/**
*  Function: instructionsScreen()
*
*  Purpose: This function draws the screen that appears when the user clicks
*           the Instructions button on the Intro screen.  It checks to see if 
*           the user's mouse is hovering over the OK button, and changes the 
*           normal arrow pointer to a hand pointer.  If the OK button is clicked, 
*           the introScreen() function is called and the user is returned to 
*           that screen. It also checks if the user has exited the program by
*           clicking the X button in the top right corner, or if they have
*           pressed the Esc key.
*/

void instructionsScreen()
{
	bool buttonClicked = false;

	okButton->reposition( halfScreenWidth, 210 );

	while ( buttonClicked == false )
	{
		closeWindowListener();

		escapeKeyListener();

		dbPasteImage( GREEN_BG_IMAGE_ID, 0, 0 );

		dbInk( BLUE, BLUE );
		dbSetTextSize( 40 );
		dbCenterText( halfScreenWidth, 50, "Instructions" );

		dbInk( WHITE, WHITE );
		dbSetTextSize( 20 );

		dbText( 50, 100, "Click the mouse once to create a bubble." );
		dbText( 50, 120, "If your bubble touches any other bubbles on" );
		dbText( 50, 140, "the screen, it will create a chain reaction!" );

		okButton->show();

		if ( okButton->mouseOver( dbMouseX(), dbMouseY() ) )
		{
			dbHideMouse();
			handPointer->display( dbMouseX(), dbMouseY() );
			handPointer->show();
		}
		else
		{
			dbShowMouse();
			handPointer->hide();
		}

		if ( okButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			handPointer->hide();
		}

		dbSync();
	}

	okButton->hide();
	okButton->reposition( halfScreenWidth, 300 );  // put back in its normal position
	handPointer->hide();

	dbShowMouse(); // Show normal mouse.

	while ( dbMouseClick() == 1 )
	{
		// Absorb last mouse click.
	}

	introScreen();
}

/**
*  Function: aboutScreen()
*
*  Purpose: This function draws the screen that appears when the user clicks
*           the About button on the Intro screen.  It checks to see if the
*           user's mouse is hovering over the OK button, and changes the 
*           normal arrow pointer to a hand pointer.  If the OK button is clicked, 
*           the introScreen() function is called and the user is returned to 
*           that screen. It also checks if the user has exited the program by
*           clicking the X button in the top right corner, or if they have
*           pressed the Esc key.
*/

void aboutScreen()
{
	bool buttonClicked = false;

	okButton->reposition( halfScreenWidth, 380 );

	while ( buttonClicked == false )
	{
		closeWindowListener();

		escapeKeyListener();

		dbPasteImage( GREEN_BG_IMAGE_ID, 0, 0 );

		dbInk( BLUE, BLUE );
		dbSetTextSize( 40 );
		dbCenterText( halfScreenWidth, 50, "About" );

		dbInk( WHITE, WHITE );
		dbSetTextSize( 20 );

		dbText( 50, 100, "Language: C++" );
		dbText( 50, 120, "Game Programming Library: DarkGDK" );
		dbText( 50, 140, "IDE: Visual C++ 2008 Express Edition" );
		dbText( 50, 160, "Installer: InstallShield 2010" );
		dbText( 50, 180, "Graphics: GIMP" );
		dbText( 50, 200, "Sound Effects: Reason" );

		dbText( 50, 240, "The source code is available in the 'source'" );
		dbText( 50, 260, "directory in the game's program directory." );

		dbText( 50, 300, "Programmer: Jason Jobes ( jjobes@gmail.com )" );

		okButton->show();

		if ( okButton->mouseOver( dbMouseX(), dbMouseY() ) )
		{
			dbHideMouse();
			handPointer->display( dbMouseX(), dbMouseY() );
			handPointer->show();
		}
		else
		{
			dbShowMouse();
			handPointer->hide();
		}

		if ( okButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			handPointer->hide();
		}

		dbSync();
	}

	okButton->hide();
	okButton->reposition( halfScreenWidth, 300 );
	handPointer->hide();

	dbShowMouse(); // Show normal mouse.

	while ( dbMouseClick() == 1 )
	{
		// Absorb last mouse click.
	}

	introScreen();
}

/**
*  Function: retryScreen()
*
*  Purpose: This function draws the screen that appears after the level when
*           the user does not pop enough bubbles.  It checks to see if the
*           user's mouse is hovering over the Retry or Exit buttons, and changes 
*           the normal arrow pointer to a hand pointer.  If the Retry button is 
*           clicked, levelStartScreen() is called and that screen is displayed.  
*           If the Exit button is clicked, the program exits.  It also checks 
*           if the user has exited the program by clicking the X button in the 
*           top right corner, or if they have pressed the Esc key.
*/

void retryScreen()
{
	bool buttonClicked = false;

	exitButton->reposition( halfScreenWidth, 300 );

	dbInk( WHITE, WHITE );
	dbSetTextSize(40);

	char buffer1[30]; 
	sprintf( buffer1, "You got %d out of %d.", levelPoints, goalPoints[level-1] );

	char buffer2[30];
	sprintf( buffer2, "Total Score: %d", totalPoints ); 

	while ( buttonClicked == false )
	{
		closeWindowListener();

		escapeKeyListener();

		dbPasteImage( GREEN_BG_IMAGE_ID, 0, 0 );

		dbInk( BLUE, BLUE );
		dbCenterText( halfScreenWidth, 50, "Too bad." );   // Too bad.

		dbInk( WHITE, WHITE );
		dbCenterText( halfScreenWidth, 100, buffer1 );     // You got %d out of %d.

		dbCenterText( halfScreenWidth, 400, buffer2 );     // Total Score: %d

		retryButton->show();
		exitButton->show();

		if ( retryButton->mouseOver( dbMouseX(), dbMouseY() ) || 
			 exitButton->mouseOver( dbMouseX(), dbMouseY() ) )
		{
			dbHideMouse();
			handPointer->display( dbMouseX(), dbMouseY() );
			handPointer->show();
		}
		else
		{
			dbShowMouse();
			handPointer->hide();
		}

		if ( retryButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			handPointer->hide();
		}
		else if ( exitButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			exitGame();
		}

		dbSync();
	}

	retryButton->hide();
	exitButton->reposition( halfScreenWidth, halfScreenHeight+100 );
	exitButton->hide();
	handPointer->hide();

	dbShowMouse();  // Show normal mouse.

	while ( dbMouseClick() == 1 )
	{
		// Absorb last mouse click.
	}

	levelStartScreen();
}

/**
*  Function: wonGameScreen()
*
*  Purpose: This function draws the screen that appears after the user has won
*           the final level.  It checks to see if the user's mouse is hovering 
*           over the Play Again or Exit buttons, and changes the normal arrow 
*           pointer to a hand pointer.  If the Play Again button is clicked, 
*           introScreen() is called and that screen is displayed. If the Exit 
*           button is clicked, exitGame() is called and the program exits.  It 
*           also checks if the user has exited the program by clicking the X 
*           button in the top right corner, or if they have pressed the Esc key.
*/

void wonGameScreen()
{
	dbPlaySound( SUCCESS_SOUND_ID );

	bool buttonClicked = false;
	bool playAgainButtonClicked = false;
	bool exitButtonClicked = false;

	dbSetTextSize(40);

	char buffer1[30];  
	sprintf( buffer1, "Final Score: %d out of %d", totalPoints, TOTAL_POSSIBLE_POINTS ); 
	
	while ( buttonClicked == false )
	{
		closeWindowListener();

		escapeKeyListener();

		dbPasteImage( GREEN_BG_IMAGE_ID, 0, 0 );
		
		dbInk( YELLOW, WHITE );

		dbCenterText( halfScreenWidth, 50, "Congratulations!" );
		dbCenterText( halfScreenWidth, 100, "You have won the game!" );

		dbCenterText( halfScreenWidth, 150, buffer1 );  // Final Score: %d out of 390

		playAgainButton->show();
		exitButton->show();

		if ( playAgainButton->mouseOver( dbMouseX(), dbMouseY() ) || 
			 exitButton->mouseOver( dbMouseX(), dbMouseY() ) )
		{
			dbHideMouse();
			handPointer->display( dbMouseX(), dbMouseY() );
			handPointer->show();
		}
		else
		{
			dbShowMouse();
			handPointer->hide();
		}

		if ( playAgainButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			playAgainButtonClicked = true;
			handPointer->hide();
		}
		else if ( exitButton->clicked( dbMouseX(), dbMouseY() ) )
		{
			buttonClicked = true;
			exitButtonClicked = true;
			handPointer->hide();
		}

		dbSync(); 
	}

	playAgainButton->hide();
	exitButton->hide();

	handPointer->hide();

	dbShowMouse(); // Show normal mouse.

	while ( dbMouseClick() == 1 )
	{
		// Absorb last mouse click.
	}

	totalPoints = 0;
	levelPoints = 0;
	wonLevel = false; 

	if ( playAgainButtonClicked == true )
	{
		introScreen();
	}
	else if ( exitButtonClicked == true )
	{
		exitGame();
	}
}

/**
*  Function: closeWindowListener()
*
*  Purpose: This function checks if the user has clicked on the X in the
*           upper right-hand corner of the window.  It does this by checking
*           if LoopGDK() returns 0, which is what it does when the user 
*           closes the window by pressing X.  If the user clicks the X,
*           exitGame() is called and the program exits.
*/

void closeWindowListener()
{
	if ( LoopGDK() == 0 )
	{
		exitGame();
	}
}

/**
*  Function: escapeKeyListener()
*
*  Purpose: This function checks if the user has pressed the Esc key on
*           the keyboard.  If so, exitGame() is called and the program
*           exits.
*/

void escapeKeyListener()
{
	if ( dbEscapeKey() == 1 )
	{
		exitGame();
	}
}

/**
*  Function: resetLevel()
*
*  Purpose: This function is called from the main game loop whenever the
*           entire game is won, a level is won, or a level is lost.  It
*           resets the variables back to their beginning-of-level state.
*			It also calls functions that delete all of the bubble objects
*           from the bubbles vector and then repopulates the vector with
*           new bubble objects.  It makes the white overlay invisible, then
*           hides all currently visible sprites to prepare for entry into
*           the cut-screen.
*/

void resetLevel()
{
	BubbleSprite::currentPopping = 0;
	chainReactionStarted = false;
	chainReactionDone = false;
	userBubbleCreated = false;
	remainingBubblesContracted = false;
	fadeComplete = false;

	deleteBubbles();   
	createAndLoadBubbles(); 
    whiteOverlay->setAlpha( 0 );
	dbHideAllSprites();
}

/**
*  Function: createAndLoadBubbles()
*
*  Purpose: This function creates each bubble in a for-loop and loads them
*           into the bubbles vector.  It is called from resetLevel(), which
*           gets called after every level.  Prior to loading the bubbles, 
*           the bubble image file names and the sound file names are shuffled
*           in their arrays, so that each level has a uniquely filled bubbles
*           vector, with different colored bubbles and sounds.  If it is unable
*           to load one of the image or sound files, it writes an error to the
*           log file.
*/

void createAndLoadBubbles()
{
	// These are random numbers to help seed the random number generator.
	// This is a work-around for the unreliable dbRND() function.
	int nums[60] = {  17, 26, 32, 45, 53, 67, 76, 80, 87, 91, 94, 101,
	                 109, 114, 121, 138, 142, 149, 150, 152, 163, 178,
					 183, 191, 192, 205, 211, 217, 222, 231, 233, 237,
					 241, 244, 246, 247, 251, 252, 255, 258, 259, 260,
					 263, 266, 271, 275, 279, 283, 284, 288, 291, 298,
					 299, 322, 324, 339, 345, 348, 349, 355 };

	shuffle( bubbleFileNames, 60 );
	shuffle( noteFileNames, 6 );     

	int j;
	int centerX;
	int centerY;
	int radius = 12;  // Since sprite is 24 px wide, hard-code radius of 12 px.

	for ( int i = 0; i < levelBubbles[level-1]; i++ )
	{
		dbRandomize( dbTimer() + nums[i] );

		centerX = dbRND( 600-2*radius ) + radius + 5;  // Ensure bubble is within bounds of screen.

		centerY = dbRND( 440-2*radius ) + radius + 5;  // Ensure bubble is within bounds of screen.

		j = dbRND(5);

		if ( dbFileExist( bubbleFileNames[i] ) )
		{
			if ( dbFileExist( noteFileNames[j] ) )
			{
				bubbles.push_back( new NormalBubble( centerX, centerY, radius, bubbleFileNames[i], i+1, i+1, noteFileNames[j], i+1 ) );
			}
			else
			{
				sprintf( lineBuffer, "Prisms.cpp: createAndLoadBubbles(): Sound file %s does not exist.", noteFileNames[j] );
				logFile << lineBuffer << endl << endl;
			}
		}
		else
		{
			sprintf( lineBuffer, "Prisms.cpp: createAndLoadBubbles(): Image file %s does not exist.", bubbleFileNames[i] );
			logFile << lineBuffer << endl << endl;
		}	
	}
}

/**
*  Function: shuffle(char * filenames[], int size)
*
*  Purpose: This function randomly shuffles the contents of an array of strings
*           that is passed to it.  It is called by createAndLoadBubbles() to
*           shuffle the bubbleFileNames[] and noteFileNames[] arrays.
*
*  Inputs: filenames[] - The array of strings that needs shuffling.
*          size - The size of this array.
*/

void shuffle(char * filenames[], int size)
{
	int randomIndex;

	dbRandomize( dbTimer() + 156 );  // The '156' is an additional integer to help seed the buggy dbRND() function.

	for ( int i = 0; i < size; i++ )
	{
		randomIndex = dbRND( size-1 );
		swap( filenames[i], filenames[randomIndex] );
	}
}

/**
*  Function: swap(int & x, int & y)
*
*  Purpose: This function is called from shuffle(), and swaps the contents of
*           the two variables passed in.
*
*  Inputs: x - The first integer.
*          y - The second integer.
*/

void swap(int & x, int & y)
{
	int temp = x;

	x = y;

	y = temp;
}

/**
*  Function: detectUserClick()
*
*  Purpose: This function is called each time through the main game loop.  It
*           checks if the user has clicked the left mouse button in the game
*           screen.  If they have, and this is the first time they have clicked,
*           the userBubble object is created and added to the bubbles vector.
*           It is drawn at the current coordinates and its pop() function is 
*           called, beginning its expand/contract animation.
*/

void detectUserClick()
{
	// Start initial pop for userBubble?
	if ( ( dbMouseClick() == 1 ) && userBubbleCreated == false )  // Left mouse button clicked
	{															  // and userBubble not yet created.

		userBubbleCreated = true;  // Now we should start checking for normal bubble collisions 
		                           // with anything else, and pop them.

		handPointer->hide();
			
		mouseX = dbMouseX();
		mouseY = dbMouseY();

		if ( dbFileExist( user_bubble_png ) )
		{
			userBubble = new UserBubble( mouseX, mouseY, 12, user_bubble_png, USER_BUBBLE_IMAGE_ID, 
				                         USER_BUBBLE_SPRITE_ID, noteFileNames[0], bubbles.size()+1 );

			bubbles.push_back( userBubble );  // Add userBubble to bubbles vector.
		}
		else
		{
			sprintf( lineBuffer, "Prisms.cpp: detectUserClick(): Image file %s does not exist.", user_bubble_png );
			logFile << lineBuffer << endl << endl;
		}	

		userBubble->setPaused( true );
		userBubble->setPopping( true );
		userBubble->pop();

		userBubble->playPopSound();

		BubbleSprite::currentPopping++;
		chainReactionStarted = true;
	}
}

/**
*  Function: detectBoundaryCollisions()
*
*  Purpose: This function is called once each time through the main
*           game loop.  It looks through the bubbles vector and finds
*           every bubble that is not the user bubble and that is not
*           currently popping.  It then sends a reference to that bubble
*           to the boundaryCollisionCheck() function, which checks if 
*           that bubble is currently colliding with a wall.
*/

void detectBoundaryCollisions()
{
	for ( int i = 0; i < bubbles.size(); i++ )  // For each bubble
	{
		if ( !bubbles[i]->getIsUserBubble() )   // that is not userBubble
		{
			if ( !bubbles[i]->getPopping() )    // and is not popping
			{
				boundaryCollisionCheck( *bubbles[i] );  // see if it is hitting a wall and
			}                                           // change direction if necessary.
		}
	}
}

/**
*  Function: boundaryCollisionCheck()
*
*  Purpose: This function accepts a bubble object and checks if it is
*           currently colliding with any of the four walls (sides of the
*           screen).  If it is, then the bubble is rotated a number of
*           degrees to make it look as though it has bounced off of and
*           away from it.
*
*  Inputs: b - A reference to the bubble object being tested for collision
*              with the walls.
*/

void boundaryCollisionCheck( BubbleSprite & b )
{
	if ( b.getCenterX() + b.getRadius() >= SCREEN_WIDTH )  // Hitting right wall.
	{
		b.rotate( 360 - b.getDegrees() );
	}

	if ( b.getCenterX() - b.getRadius() <= 0 )  // Hitting left wall.
	{
		b.rotate( 360 - b.getDegrees() );
	}

	if ( b.getCenterY() - b.getRadius() <= 0 )  // Hitting top wall.
	{
		b.rotate( 180 - b.getDegrees() );
	}

	if ( b.getCenterY() + b.getRadius() >= SCREEN_HEIGHT )  // Hitting bottom wall.
	{
		b.rotate( 180 - b.getDegrees() );
	}
}

/**
*  Function: detectBubbleCollisions()
*
*  Purpose: This function looks at every bubble that is popping, and checks
*           if it is currently touching another bubble that is not yet
*           popping.  If it is touching another bubble that is not yet
*           popping, then that bubble starts popping.
*/

void detectBubbleCollisions()
{
	if ( userBubbleCreated )
	{
		for ( int i = 0; i < bubbles.size(); i++ )  // For each bubble on the screen
		{
			if ( ( bubbles[i]->getPopping() ) )     // if this bubble is popping
			{                                          
				for ( int j = 0; j < bubbles.size(); j++ )  // see if it is touching any other non-popping bubble, and
				{
					if ( ( bubbles[j]->getPopping() ) == false && bubbles[j]->getRadius() != 0 )  // don't test bubbles with radius 0 (they are done popping)
					{
						if ( bubbleCollisionCheck( *bubbles[i], *bubbles[j] ) )
						{
							BubbleSprite::currentPopping++;

							bubbles[j]->playPopSound();

							levelPoints++;

							bubbles[j]->setPaused( true );
							bubbles[j]->setPopping( true );
							bubbles[j]->pop();
						}
					}
				}
			}
		}
	}
}

/**
*  Function: bubbleCollisionCheck( BubbleSprite & b1, BubbleSprite & b2 )
*
*  Purpose: This function accepts two bubble objects and checks if they are
*           overlapping.  Since these bubbles are just circles, it first finds
*           the sum of their radii and stores that in totalRadius.  It then 
*           finds the distance between their two center points (using the
*           Pythagorean Theorem).  If the distance between those two center
*           points is less than or equal to totalRadius, then we know the
*           two circles are at least touching at their outer edge, and possibly
*           overlapping.
*
*  Inputs: b1 - The first bubble to be compared.
*          b2 - The second bubble to be compared.
*
*  Returns: True if b1 intersects with b2.  False otherwise.
*/

bool bubbleCollisionCheck( BubbleSprite & b1, BubbleSprite & b2 )
{
	int totalRadius = b1.getRadius() + b2.getRadius();
	
	double x = b1.getCenterX() - b2.getCenterX();  // Difference between their x-coordinates.
	double y = b1.getCenterY() - b2.getCenterY();  // Difference between their y-coordinates.

	float distance = sqrt( pow(x, 2) + pow(y, 2) );  // Distance between centers, basically
                                                     // the hypotenuse.
	if ( distance <= totalRadius )  // They are overlapping.
	{
		return true;
	}

	return false;
}

/**
*  Function: popUserBubble()
*
*  Purpose: This function is called once each time through the main game loop.
*           It checks if the userBubble has been created, and if it is currently
*           supposed to be popping, then it continues to do so.
*/

void popUserBubble()
{
	if ( userBubbleCreated )                   
	{
		if ( userBubble->getPopping() )   
		{
			userBubble->pop();
		}
	}
}

/**
*  Function: popBubbles()
*
*  Purpose: This function is called from the main game loop.  It moves through
*           the bubbles vector and, if that bubble's popping flag is currently
*           set to true, then it continues to pop.
*/

void popBubbles()
{
	for ( int i = 0; i < bubbles.size(); i++ )
	{
		if ( bubbles[i]->getPopping() ) 
		{
			bubbles[i]->pop();
		}
	}
}

/**
*  Function: pauseBubbles()
*
*  Purpose: This function is called from the main game loop when the chain
*           reaction is over.  It moves through the bubbles vector and sets
*           all of their paused flags to true.  This occurs right before all
*           of the remaining un-popped bubbles on the screen contract.
*/

void pauseBubbles()
{
	for ( int i = 0; i < bubbles.size(); i++ )
	{
		bubbles[i]->setPaused( true );
	}
}

/**
*  Function: contractRemainingBubbles()
*
*  Purpose: This function is called from the main game loop, after the chain
*           reaction is done, and after all of the remaining bubbles on the 
*           screen have been paused.  After this, the game moves to a cut-screen.
*/

void contractRemainingBubbles()
{
	for ( int i = 0; i < bubbles.size(); i++ )
	{
		if ( bubbles[i]->getRadius() > 0 )
		{
			bubbles[i]->contract();
		}
	}	
}

/**
*  Function: moveBubbles()
*
*  Purpose: This function is called from the main game loop.  It moves
*           through the bubbles vector and moves each of the bubbles
*           forward 1 pixel in the direction they are facing.  The user
*           bubble is not affected by this because it is set to paused
*           on creation, and the move() function only moves non-paused
*           bubbles.
*/

void moveBubbles()
{
	for ( int i = 0; i < bubbles.size(); i++ )
	{
		bubbles[i]->move();
	}
}

/**
*  Function: exitGame()
*
*  Purpose: This function is called if the user clicks the X in the top
*           right corner of the screen, presses the Esc key, clicks the
*           Exit button and after the main game loop.  It removes objects
*           from memory by calling deleteComponents() and deleteBubbles().
*           It also writes a line to the log file indicating the time the
*           game ended.
*/

void exitGame()
{
	deleteComponents();
	deleteBubbles();

	time( & currentTime );
	sprintf( lineBuffer, "%s Prisms.cpp: Game ended.", ctime( & currentTime ) );
	logFile << lineBuffer << endl << endl;

	logFile.close();

	exit(0);	
}

/**
*  Function: deleteComponents()
*
*  Purpose: This function is called from exitGame() and removes from memory
*           the sounds and images loaded in Prisms.cpp.  The other multimedia
*           resources are removed from memory in their objects' destructors.
*/

void deleteComponents()
{
	dbDeleteSound( SUCCESS_SOUND_ID );
	dbDeleteImage( SPACE_BG_IMAGE_ID );  
	dbDeleteImage( HAND_POINTER_IMAGE_ID );  
	dbDeleteImage( GREEN_BG_IMAGE_ID );
}

/**
*  Function: deleteBubbles()
*
*  Purpose: This function is called after each level and from exitGame(). 
*           It removes the objects from memory that are being stored in
*           the bubbles vector.
*/

void deleteBubbles()
{
	vector <BubbleSprite*>::iterator pb;

	for ( pb = bubbles.begin(); pb != bubbles.end(); pb++ )
	{
		delete *pb;	
	}

	bubbles.clear();
}

