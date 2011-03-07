/* 
 * File:   UrbiSound.cpp
 * Author: aoleksy
 * 
 * Created on 7 marzec 2011, 11:02
 */

#include "UrbiSound.h"

using namespace urbi;

UrbiSound::UrbiSound(const std::string& name) : UObject(name) {
    
}

UrbiSound::~UrbiSound() {
}

bool UrbiSound::play() {
    return true;
}
bool UrbiSound::play(const std::string&) {
    return true;
}
void UrbiSound::pause() {
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

