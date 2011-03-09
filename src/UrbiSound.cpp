/* 
 * File:   UrbiSound.cpp
 * Author: aoleksy
 * 
 * Created on 7 marzec 2011, 11:02
 */

#include "UrbiSound.h"

#include <SDL.h>
#include <SDL/SDL_audio.h>

#include <iostream>
#include <vector>
#include <list>

using namespace urbi;
using namespace std;

void mixAudio(void *unused, Uint8 *stream, int len);

class Sample {
private:
    typedef vector<Uint8> DataUint8;
    DataUint8 mData;
    Uint32 dpos;
    const UrbiSound* mOwner;
public:
    Sample(const UrbiSound* owner, const Uint8 *data, Uint32 dlen)
    : mOwner(owner), mData(data, data+dlen), dpos(0) { }
    
    const UrbiSound* getOwner() const {
        return mOwner;
    }
};

class SDLSoundSingleton {
private:
    SDLSoundSingleton();
    SDLSoundSingleton(const SDLSoundSingleton&);
    SDLSoundSingleton& operator=(const SDLSoundSingleton&);
    
    SDL_AudioSpec fmt;
    bool isOpened_;
    
    typedef list<Sample> SampleList;
    SampleList sampleList;
    
public:
    static SDLSoundSingleton& getInstance();
   
    bool openDevice();
    void closeDevice();
    bool isOpened();
    bool stopSample(const UrbiSound* owner);
};

// Implementations
void mixAudio(void *unused, Uint8 *stream, int len) {
    
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

static SDLSoundSingleton& SDLSoundSingleton::getInstance() {
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
    //dodanie do listy odtwarzanych plikow
    delete[] cvt.buf;
    SDL_UnlockAudio();

    return true;
}

void UrbiSound::stop() {
    //Znajdz swoj wpis w liscie
    //zablokuj urzadzenie
    //usun
    return;
}

bool UrbiSound::openDevice() {
    cerr << "Opening device" << endl;
    return true;
}

void UrbiSound::closeDevice() {
    return;
}

bool UrbiSound::isDeviceOpened() {
    return true;
}

bool UrbiSound::isPlaying() {
    return true;
}