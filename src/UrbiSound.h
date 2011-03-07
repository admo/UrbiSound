/* 
 * File:   UrbiSound.h
 * Author: aoleksy
 *
 * Created on 7 marzec 2011, 11:02
 */

#ifndef URBISOUND_H
#define	URBISOUND_H

#include "urbi/uobject.hh"

#include <string>

class UrbiSound {
public:
    UrbiSound();
    UrbiSound(const UrbiSound& orig);
    virtual ~UrbiSound();
    
    bool play();
    bool play(const std::string&);
    void pause();
    bool openDevice();
    bool closeDevice();
    bool isDeviceOpened();
    
private:
    bool deviceOpened;
};

#endif	/* URBISOUND_H */

