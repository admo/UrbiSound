/*******************************************
*
*	UPlayer version 1.0
*
*   Author: Adam Oleksy
*   date: 16.03.2011
*
********************************************/


Description:
//////////////////////////////////////////
Module to play wav files. It uses SDL library.

Install:
//////////////////////////////////////////
1. Download SDL library from http://www.libsdl.org/ and
install them in the system (ex. C:\Program Files\SDL-1.2.14).

2. Under Windows platform add to system Enviroment Variables:
 - to PATH add sdl directory  (ex. C:\Program Files\SDL-1.2.14)

3. For compile sources you have to download and install CMake also. 
CMake website is http://www.cmake.org  
a. Set system Enviroment Variables:
 - SDLDIR (ex. C:\Program Files\SDL-1.2.14);
 - URBI_ROOT (ex. C:\Program Files\Gostai Engine Runtime\2.7\).
b. use CMake to generate and configure the project.


Module functions:
//////////////////////////////////////////
UPlayer.play("sound.wav"); - play the sound, you can run this
function in background;
UPlayer.stop; - stop the sound;
UPlayer.isPlaying; - returns true (1) if sound is playing.

Urbiscript example:
//////////////////////////////////////////

loadModule("UPlayer");
var player=UPlayer.new;
player.play("music.wav"),



