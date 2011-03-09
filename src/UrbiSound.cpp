/* 
 * File:   UrbiSound.cpp
 * Author: aoleksy
 * 
 * Created on 7 marzec 2011, 11:02
 */

#include "UrbiSound.h"

#include <SDL.h>
#include <SDL/SDL_audio.h>

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <vector>
#include <list>

using namespace urbi;
using namespace std;
using namespace boost;

void mixAudio(void *, Uint8 *stream, int len);

typedef shared_ptr<Uint8> Uint8Pointer;

class Sample {
private:
    Uint8Pointer mData;
    Uint32 mDataLen;
    Uint32 mDataPos;
    const UrbiSound* mOwner;
public:
    Sample(const UrbiSound* owner, Uint8Pointer data, Uint32 dataLen)
    : mOwner(owner), mData(data), mDataLen(dataLen) { }
    
    const UrbiSound* getOwner() const {
        return mOwner;
    }
    
    const Uint8* getData(int howMany) {
        Uint8* ret = &mData.get()[mDataPos];
        mDataPos += howMany;
        return ret;
    }
    
    int getDataLeft() const {
        return mDataLen - mDataPos;
    }
};
bool SampleIsEmptyPredicate(const Sample& sample) {
    return !sample.getDataLeft();
}
class SampleBelongsTo {
private:
    const UrbiSound* mUrbiSound;
public:
    SampleBelongsTo(const UrbiSound* urbiSound) : mUrbiSound(urbiSound) { }
    bool operator() (const Sample& sample) {
        return mUrbiSound == sample.getOwner();
    }
};

typedef list<Sample> SampleList;

class SDLSoundSingleton {
private:
    SDLSoundSingleton();
    SDLSoundSingleton(const SDLSoundSingleton&);
    SDLSoundSingleton& operator=(const SDLSoundSingleton&);
    
    SDL_AudioSpec fmt;
    bool isOpened_;
    
    SampleList sampleList;
    
public:
    static SDLSoundSingleton& getInstance();
   
    bool openDevice();
    void closeDevice();
    bool isOpened();
    bool stopSample(const UrbiSound* owner);
    
    SampleList &getSampleList();
    void addSample(const Sample& sample);
    bool play(const UrbiSound* owner, const string& file);
    void stop(const UrbiSound* owner);
};

// Implementations
void mixAudio(void *, Uint8 *stream, int len) {
    SampleList& sampleList = SDLSoundSingleton::getInstance().getSampleList();
    for(SampleList::iterator i = sampleList.begin(); i != sampleList.end(); ++i) {
        // Mozna sprawdzic czy jest zapauzowany:)
        Uint32 amount = i->getDataLeft();
        amount = amount < len ? amount : len;
        SDL_MixAudio(stream, i->getData(amount), amount, SDL_MIX_MAXVOLUME);

    }
    sampleList.remove_if(SampleIsEmptyPredicate);
    if(sampleList.empty())
        SDL_PauseAudio(1);
    
    cerr << "mixAudio " << sampleList.size() << endl;
}

//------------------------- SDLSoundSingleton ----------------------------//

SDLSoundSingleton::SDLSoundSingleton() : isOpened_(false) {
    cerr << "SDLSoundSingleton::SDLSoundSingleton()" << endl;

    /* Set 16-bit stereo audio at 22Khz */
    fmt.freq = 44100;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512; /* A good value for games */
    fmt.callback = mixAudio;
    fmt.userdata = NULL;
}

SDLSoundSingleton& SDLSoundSingleton::getInstance() {
    static SDLSoundSingleton instance;
    return instance;
}

bool SDLSoundSingleton::openDevice() {
    if (!isOpened()) {
        isOpened_ = !SDL_OpenAudio(&fmt, NULL);
    }
        
    return isOpened();
}

void SDLSoundSingleton::closeDevice() {
    if (isOpened()) {
        isOpened_ = false;
        SDL_CloseAudio();
    }
}

bool SDLSoundSingleton::isOpened() {
    return isOpened_;
}

bool SDLSoundSingleton::play(const UrbiSound* owner, const string& file) {
    if(!isOpened())
        return false;
    
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    if (SDL_LoadWAV(file.c_str(), &wave, &data, &dlen) == NULL) {
        cerr << "Couldn't load " << file << endl;
        return false;
    }

    SDL_AudioCVT cvt;
    SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
            AUDIO_S16, 2, 44100);
    cvt.buf = new Uint8[dlen * cvt.len_mult];
    memcpy(cvt.buf, data, dlen);
    cvt.len = dlen;
    SDL_ConvertAudio(&cvt);
    SDL_FreeWAV(data);
    
    SDL_LockAudio();
    Uint8Pointer sampleData(cvt.buf);
    sampleList.push_back(Sample(owner, sampleData, cvt.len_cvt));
    SDL_PauseAudio(0);
    SDL_UnlockAudio();
    
    return true;
}

void SDLSoundSingleton::stop(const UrbiSound* owner) {
    SDL_LockAudio();
    cerr << "SDLSoundSingleton::stop(const UrbiSound* owner)" << sampleList.size() << endl;
    sampleList.remove_if(SampleBelongsTo(owner));
    SDL_UnlockAudio();
}

SampleList &SDLSoundSingleton::getSampleList() {
    return sampleList;
}


//------------------------- UrbiSound ----------------------------//

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

bool UrbiSound::play(const std::string& file) {
    stop();
    return SDLSoundSingleton::getInstance().play(this, file);
}

void UrbiSound::stop() {
    SDLSoundSingleton::getInstance().stop(this);
    return;
}

bool UrbiSound::openDevice() {
    return SDLSoundSingleton::getInstance().openDevice();
}

void UrbiSound::closeDevice() {
    SDLSoundSingleton::getInstance().closeDevice();
}

bool UrbiSound::isDeviceOpened() {
    return true;
}

bool UrbiSound::isPlaying() {
    return true;
}