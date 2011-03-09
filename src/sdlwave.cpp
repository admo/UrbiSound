/* 
 * File:   sdlwave.c
 * Author: aoleksy
 *
 * Created on 6 marzec 2011, 20:31
 */

#include "UrbiSound.h"

#include <iostream>

#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
    UrbiSound urbiSound("aaa");
    
    cout << "urbiSound.openDevice(): " << urbiSound.openDevice() << endl;
    cout << "urbiSound.play(\"/tmp/sound.wav\"): " << urbiSound.play("/tmp/sound.wav") << endl;
    sleep(10);
    cout << "urbiSound.play(\"/usr/share/sounds/purple/logout.wav\"): " << urbiSound.play("/tmp/sound.wav") << endl;
    sleep(20);
    return 0;
}