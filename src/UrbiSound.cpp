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

UrbiSound::UrbiSound(const std::string& name) : UObject(name) {
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
    return true;
}
bool UrbiSound::closeDevice() {
    return true;
}
bool UrbiSound::isDeviceOpened() {
    return true;
}
bool UrbiSound::isPlaying() {
    return true;
}

