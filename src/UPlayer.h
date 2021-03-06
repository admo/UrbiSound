/* 
 * File:   UrbiSound.h
 * Author: aoleksy
 *
 * Created on 7 marzec 2011, 11:02
 */

#ifndef URBISOUND_H
#define	URBISOUND_H

#include "urbi/uobject.hh"

#include <boost/thread/mutex.hpp>

#include <string>

class UPlayer : public urbi::UObject {
public:
    UPlayer(const std::string&);
    virtual ~UPlayer();
    
    bool play(const std::string&);
    void stop();
    //TODO: Pause function?
    bool isPlaying();
    
    void lockPlay(bool lock) const;
private:
    mutable boost::mutex mLockPlay;
};

UStart(UPlayer);

#endif	/* URBISOUND_H */

