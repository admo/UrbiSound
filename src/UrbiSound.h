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

class UrbiSound : public urbi::UObject {
public:
    UrbiSound(const std::string&);
    virtual ~UrbiSound();
    
    bool play(const std::string&);
    void stop();
    //TODO: Pause function?
    bool isPlaying();
};

UStart(UrbiSound);

#endif	/* URBISOUND_H */

