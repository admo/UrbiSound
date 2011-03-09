/* 
 * File:   UrbiSound.cpp
 * Author: aoleksy
 * 
 * Created on 7 marzec 2011, 11:02
 */

#include "UrbiSound.h"

#include <iostream>

using namespace urbi;
using namespace std;

void mixAudio(void *unused,  Uint8 *stream, int len) {
    
}

UrbiSound::UrbiSound(const std::string& name) : UObject(name) {
    // Configure SDL audio
    fmt.freq = 44100;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;        /* A good value for games */
    fmt.callback = mixAudio;
    fmt.userdata = NULL;
    
    //Bind the functions
    UBindFunction(UrbiSound, play);
    UBindFunction(UrbiSound, stop);
    UBindFunction(UrbiSound, openDevice);
    UBindFunction(UrbiSound, closeDevice);
    UBindFunction(UrbiSound, isDeviceOpened);
    UBindFunction(UrbiSound, isPlaying);
}

UrbiSound::~UrbiSound() {
}

bool UrbiSound::play(const std::string&) {
    return true;
}
void UrbiSound::stop() {
    return;
}
bool UrbiSound::openDevice() {
    cerr << "Opening device" << endl;
    if (!deviceOpened)
        deviceOpened = !SDL_OpenAudio(&fmt, NULL);
    else
        return false;
    return deviceOpened;
}
void UrbiSound::closeDevice() {
    if (deviceOpened)
        SDL_CloseAudio();
}
bool UrbiSound::isDeviceOpened() {
    return true;
}
bool UrbiSound::isPlaying() {
    return true;
}
