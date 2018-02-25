//////////////////////////////////////////
//                                      //
// A Fast Windows Program               //
//                                      //
// You found this at bobobobo's weblog, //
// https://bobobobo.wordpress.com        //
//                                      //
// Creation date:  Feb 3/08             //
// Last modified:  Feb 9/08             //
//                                      //
//////////////////////////////////////////

#include <windows.h>
#include <math.h>   // for sin() and cos() in calculating npc pos

// Function prototypes.
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow );

////////////////////////////
// Introduction:
// 
// In the "MostBasicWindow" project, we talked
// about creating a basic window, and how
// MOST NORMAL Windows programs use something
// called "EVENT DRIVEN PROGRAMMING".

// If you don't remember, please review
// PREREQUISITES:  BASIC WINDOWS PROGRAMMING
// (see https://bobobobo.wordpress.com/2008/01/31/how-to-create-a-basic-window-in-c/
// to get started).

// In EVENT DRIVEN PROGRAMMING, your
// program is entirely driven by EVENTS.

// If there are no events, then your
// Windows program DOES NOTHING AT ALL,
// until the next EVENT is passed to it
// by the Windows O/S in the form of a 
// MESSAGE.

// Now, this is a PROBLEM when it comes
// to programming a GAME AS A Windows application.

// If your app NORMALLY does nothing
// except when the user interacts with
// the application, then every time
// the user let go of all of the keys
// on the keyboard, the game would just
// simply PAUSE.  This is because
// the normal structure of a Windows
// application is to execute code
// IN RESPONSE TO EVENTS THAT COME
// to the window.  If there are no
// events, then the application is idle.

// That structure makes perfect sense
// for applications like MSWord or firefox, and its
// the only way things are sane inside
// that Windows Government and the
// processor doesn't blow up when you
// have 50 windows open at the same time.

// However, GAMES are different than normal
// standard windows apps like Firefox or
// MSWord.

// Games need to CONSTANTLY be running.  Games
// need to constantly update their state.
// Even if the player isn't doing anything,
// the game AI has to keep computing what
// the MONSTERS will do next.  The game
// has to have a concept of TIME that keeps
// on going on, even when the player isn't
// pressing any keys too.

///////////////////////
// RECALL:  The NORMAL WINDOWS PROGRAM MESSAGE LOOP:
/*
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        // GetMessage is a function that will not
        // return until the Windows O/S HAS A message
        // for our program.

        // Since the GetMessage() function call is
        // INSIDE the bracket for the while Loop,
        // this means that our program is essentially
        // "put on hold" or halted until the GetMessage function
        // returns.

        // If and when the user interacts with our
        // application's window, then the GetMessage()
        // function WILL return and the variable
        // msg will be filled with interesting details
        // about exactly what the user did to the window.

        TranslateMessage( &msg );   // translates 
        // the message so WndProc can process it
        // more easily.

        DispatchMessage( &msg );    // this line RESULTS IN
        // a call to WndProc(), passing the message and
        // the HWND.  This is OUR APPS CHANCE
        // to execute some code in response
        // to user events.
    }
*/

// The point is, GAMES CAN'T _WAIT_ FOR
// MESSAGES FROM THE WINDOWS O/S TO BE
// ALLOWED TO EXECUTE THEIR CODE!!

// Games need to run very quickly, and
// execute the following instructions
// __at least__ 60 times a second!

// {
//      checkWhatKeysUserIsPushingDown();
//      makeChangesToGameStateAndCalculateNextFrame();
//      draw();
// }

// Because GetMessage() blocks (doesn't return
// control to our app immediately!), it really
// creates a bottleneck for us!

// IN OTHER WORDS, WE HAVE TO STOP USING
// THE GetMessage() function!

// For our game, we DO NOT want to be WAITING
// for messages AT ALL.  Instead, we will
// use something else called PeekMessage()
// to VERY QUICKLY check to see if the
// Windows O/S has any messages for our
// Window.

// IF the Windows Government does have a message for us,
// THEN we process that message, then its
// back to our computing and drawing functions
// as usual.

// IF the Windows O/S DOES NOT
// have any messages for our Window, WE
// WANT TO CONTINUE RUNNING OUR OWN CODE
// ANYWAY -- WE DO NOT WANT TO GIVE UP
// CONTROL TO THE WINDOWS O/S.

#pragma region globals
HWND hwnd;      // store the HWND in a global now,
                // so our DRAW function can use it freely.

// player x and y positions.
float playerXPos = 50;
float playerYPos = 50;

// NPC x and y positions.
float npcXPos = 80;
float npcYPos = 80;
#pragma endregion

void check_for_user_input_through_keyboard()
{
    float diff = 0.01f;

    if( GetAsyncKeyState( VK_UP ) )
    {
        // up key being pushed, move player up
        playerYPos -= diff;
    }

    if( GetAsyncKeyState( VK_DOWN ) )
    {
        playerYPos += diff;
    }

    if( GetAsyncKeyState( VK_RIGHT ) )
    {
        playerXPos += diff;
    }

    if( GetAsyncKeyState( VK_LEFT ) )
    {
        playerXPos -= diff;
    }
}

// In this function, we're calculating
// the motion of the "npc unit" - very simple
void calculate_next_frame_of_our_program_by_shuffling_npc_around()
{
    // let's move the NPC unit around, in a circle.
    static float pos = 0.0f;
    pos += 0.1f;

    npcXPos += 0.01*sin( 0.001*pos );
    npcYPos += 0.01*cos( 0.001*pos );
}

void draw()
{
    HDC hdcTotalArtist = GetDC( hwnd ); // get the "handle to device context"
                                        // for our Window.

    // The "device context" is like a "total artist"
    // that has pens, brushes, and a canvas to draw to.

    // I explain that more below, but for now, let's
    // draw the player:
    Ellipse( hdcTotalArtist, (int)playerXPos, (int)playerYPos, (int)(playerXPos + 10), (int)(playerYPos + 10) );

    ////////////
    // Changing the pen color:
    // Now the npc unit should be a different color.
    // so this is REALLY QUICKLY how you change
    // the color of the "pen" that you're using
    // to draw.

    // The "device context" of our window is
    // like a TOTAL ARTIST.
    // The "device context" contains:
        // canvas
        // pens
        // brushes
        // and more!

    // In order to change the style in which the
    // standard GDI functions (like Ellipse) are
    // drawing, we would have to CHANGE THE
    // PROPERTIES OF THE DEVICE CONTEXT TO WHICH
    // WE ARE DRAWING.

    // So what we do is, we:
        // 1.  CREATE A NEW redpen,
        //      WITH ALL THE PROPERTIES
        //      WE WANT IT TO HAVE (solid lines, can select width as well)
        // 
        // 2.  GIVE THAT redpen TO
        //     THE device context "total artist"
        //     by using the SelectObject() function.
        //     Note that the hdc total artist can
        //     only hold ONE PEN at a time.  So
        //     you'll see in the code that when we
        //     give the hdc total artist
        //     a new pen, WE MUST CATCH THE OLD ONE
        //     IN A VARIABLE!
        //
        // 3.  DRAW AGAIN using the Ellipse()
        //     gdi function.  Since the Ellipse()
        //     function WILL draw to an HDC,
        //     then we will see RED ELLIPSE, since
        //     we just gave the hdc total artist
        //     a red pen.

    // "select the pen into" the device context
    // by using the SelectObject() function.

    // This in a sense is like GIVING the device
    // context a new brush with which to draw,
    // or a new pen with which to draw.
    HPEN redpen = (HPEN)CreatePen( PS_SOLID, 2, RGB( 255, 0, 0 ) );
    HPEN oldpen = (HPEN)SelectObject( hdcTotalArtist, redpen );

    // draw with redpen
    Ellipse( hdcTotalArtist, (int)npcXPos, (int)npcYPos, (int)npcXPos + 10, (int)npcYPos + 10 );

    // select back the old pen, then delete the red pen
    redpen = (HPEN)SelectObject( hdcTotalArtist, oldpen );
    DeleteObject( redpen );

    ReleaseDC( hwnd, hdcTotalArtist );
}

////////////////////////////
// In a C++ Windows app, the starting point is WinMain().
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR szCmdLine,
                    int iCmdShow )
{
    #pragma region part 1 - STARTUP STUFF
    WNDCLASS wc;
    wc.cbClsExtra = 0; 
    wc.cbWndExtra = 0; 
    wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );         
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );     
    wc.hInstance = hInstance;         
    wc.lpfnWndProc = WndProc;         
    wc.lpszClassName = TEXT("Philip");
    wc.lpszMenuName = 0; 
    wc.style = CS_HREDRAW | CS_VREDRAW;

    // B.  Register the WNDCLASS with Windows, THEN
    //     create the window.
    RegisterClass( &wc );
    hwnd = CreateWindow(
        TEXT("Philip"),
        TEXT("Fast windows app!!"),
        WS_OVERLAPPEDWINDOW,
        10, 10,
        400, 400,
        NULL, NULL,
        hInstance, NULL );      

    // Next, SHOW and PAINT the window!
    // You won't see the window if you DO NOT
    // call ShowWindow();
    ShowWindow(hwnd, iCmdShow );
	UpdateWindow(hwnd);
    #pragma endregion

    // first, we create the MSG structure.
    MSG msg;

    #pragma region OLD GETMESSAGE LOOP -- NORMALLY YOU SHOULD JUST DELETE THIS PART
    // -- commented out, but I STRONGLY ENCOURAGE
    // you to try commenting it back in to see
    // how it behaves and why we CAN'T be using
    // GetMessage() for our games.

    // IT DOES WORK, but its INCREDIBLY, INCREDIBLY
    // slow.

    // And on top of the extreme SLOW, you have to constantly
    // be inputting "messages" into the window in order
    // for anything to happen! (for the npc to move!)

    /*
    SetWindowText( hwnd, TEXT("SLOW MODE!!! - Press ESC for fast mode") );
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg ); 

        check_for_user_input_through_keyboard();
        calculate_next_frame_of_our_program_by_shuffling_npc_around();
        draw();
    }
    */
    #pragma endregion

    #pragma region part 2 - ENTER A LOOP TO CONTINUALLY KEEP CHECKING WITH WIN O/S FOR USER INTERACTION

    // Notice we changed the loop structure from the old
    // GetMessage() structure.  We're using PeekMessage() now.

    // It goes like this now:

    // while(FOREVER)
    // {
    //    if ( there's a message for our window )
    //    {
    //      translate and dispatch to WndProc for processing();
    //    }
    //  
    //    check_for_user_input_through_keyboard();
    //    calculate_next_frame_of_our_program_by_shuffling_npc_around();
    //    draw();
    // 
    // }
    SetWindowText( hwnd, TEXT("Fast windows app!!") );  // (in case you uncommented
                                                        // the GetMessage() above )
    while( 1 )  // LOOP FOREVER
    {
        // 1.  PROCESS MESSAGES FROM WINDOWS
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            // Now, this is DIFFERENT than GetMessage()!
            // PeekMessage is NOT a BLOCKING FUNCTION.
            // That is, PeekMessage() returns immediately
            // with either a TRUE (there was a message
            // for our window)
            // or a FALSE (there was no message for our window).

            // If there was a message for our window, then
            // we want to translate and dispatch that message.

            // otherwise, we want to be free to run
            // the next frame of our program.
            if( msg.message == WM_QUIT )
            {
                break;  // BREAK OUT OF INFINITE LOOP
                        // if user is trying to quit!
            }
            else
            {
              TranslateMessage( &msg );   // translates 
              DispatchMessage( &msg );    // this line RESULTS IN
              // a call to WndProc(), passing the message and
              // the HWND.

              // Note that in this program, all we're really using
              // the messaging system for is for
              // processing the QUIT message that occurs
              // when the user clicks the X on the window
              // to close it.

              // ALL OTHER application processing happens
              // in the 3 lines of code below
              // (the 'check_for_user ... ' stuff)
           }
        }
        else
        {
           check_for_user_input_through_keyboard();
           calculate_next_frame_of_our_program_by_shuffling_npc_around();
           draw();
        }
    }
    #pragma endregion

    return msg.wParam;
}

LRESULT CALLBACK WndProc(   HWND hwnd,      
                            UINT message,  
                            WPARAM wparam,  
                            LPARAM lparam ) 
{
    switch( message )
    {
    case WM_CREATE:
        // upon creation, let the speaker beep at 50Hz, for 10ms.
        Beep( 50, 10 );
        return 0;
        break;

    case WM_PAINT:
        {
            // we would place our Windows painting code here.
            HDC hdc;
            PAINTSTRUCT ps;
            hdc = BeginPaint( hwnd, &ps );

            // draw nothing here.
            // drawing happens in the 
            // draw() function that
            // we MANUALLY call ourselves.

            // We DO NOT rely on messages to
            // tell us when to draw when creating
            // game!  Instead, we draw when
            // we want to.

            // WHY?  Because the WM_PAINT message
            // only comes to your window WHEN
            // the Windows O/S THINKS your window
            // needs to be redrawn.  That includes
            // only events like "if your window
            // was blocked by another window,
            // then it needs to be repainted"

            // Our fast game NEEDS to be redrawn
            // 60 times a second AT LEAST, so we
            // totally BYPASS the normal windows
            // system, and draw in our draw()
            // function of our own accord.

            EndPaint( hwnd, &ps );
        }
        return 0;
        break;

    case WM_KEYDOWN:
        switch( wparam )
        {
        case VK_ESCAPE:
            PostQuitMessage( 0 );
            break;
        default:
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage( 0 ) ;
        return 0;
        break;
    }

    return DefWindowProc( hwnd, message, wparam, lparam );
}

///////////////////////
// Closing notes:
//
// IF you'll notice, when you run this
// program, if you look at your task
// manager, (CTRL+ALT+DEL), NOTICE your
// windows application (should show up
// in processes list as BasicWindowsProgramming_GameStructure.exe)
// is now a CPU HOG.  Its taking somewhere
// close to 40-50% of CPU time.

// THIS IS GOOD. YOUR APP IS TAKING
// CONTROL OF THE SYSTEM RESOURCES
// (CPU TIME).
//
// If its a game, THIS IS EXACTLY WHAT YOU
// WANT.  Games need to hog the system
// resources in order to run the best
// they possibly can given the user's
// hardware.

// Using this Windows program structure -
// a "greedy" structure if you will -
// we can create games that run as
// fast as they can.

/* 
     ____   __   __      __   __  ___
    / _  \ /  / /  /    /  /  \ \/  /
   / _/ / /  / /  /    /  /    \   /
  / _/ \ /  / /  /__  /  /__   /  /
 /_____//__/ /______//______/ /__/

*/