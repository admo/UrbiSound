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

class USound : public urbi::UObject {
public:
    USound(const std::string&);
    virtual ~USound();
    
    bool play(const std::string&);
    void stop();
    //TODO: Pause function?
    bool isPlaying();
};

UStart(USound);

#endif	/* URBISOUND_H */

