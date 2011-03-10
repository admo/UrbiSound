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
#include <boost/noncopyable.hpp>

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

class SDLSoundSingleton : private boost::noncopyable {
private:
    SDLSoundSingleton();
    
    SDL_AudioSpec mFMT;
    bool mIsOpened;
    
    SampleList mSampleList;
    
public:
    ~SDLSoundSingleton();
    
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
    SampleList::iterator i = sampleList.begin();
    while(i != sampleList.end()) {
        Uint32 amount = i->getDataLeft();
        amount = amount < len ? amount : len;
        SDL_MixAudio(stream, i->getData(amount), amount, SDL_MIX_MAXVOLUME);
        
        //Iteracja pętli oraz sprawdzenie czy w sample cos jeszcze zostalo
        SampleList::iterator previous = i++;
        if(!previous->getDataLeft())
            sampleList.erase(previous);
    }
    
    if(sampleList.empty())
        SDL_PauseAudio(1);
}

//------------------------- SDLSoundSingleton ----------------------------//

SDLSoundSingleton::SDLSoundSingleton() : mIsOpened(false) {
    cerr << "SDLSoundSingleton::SDLSoundSingleton()" << endl;

    /* Set 16-bit stereo audio at 22Khz */
    mFMT.freq = 44100;
    mFMT.format = AUDIO_S16;
    mFMT.channels = 2;
    mFMT.samples = 512; /* A good value for games */
    mFMT.callback = mixAudio;
    mFMT.userdata = NULL;
}

SDLSoundSingleton::~SDLSoundSingleton() {
    cerr << "SDLSoundSingleton::~SDLSoundSingleton()" << endl;
    SDL_CloseAudio();
}

SDLSoundSingleton& SDLSoundSingleton::getInstance() {
    static SDLSoundSingleton instance;
    return instance;
}

inline bool SDLSoundSingleton::openDevice() {
    cerr << "SDLSoundSingleton::openDevice()" << endl;
    if (!isOpened()) {
        if(SDL_OpenAudio(&mFMT, NULL) < 0) {
            cerr << "Could'nt open audio: " <<SDL_GetError() << endl;
            return false;
        }
    }
        
    return (mIsOpened = true);
}

inline void SDLSoundSingleton::closeDevice() {
    cerr << "SDLSoundSingleton::closeDevice()" << endl;
    if (isOpened()) {
        mIsOpened = false;
        SDL_CloseAudio();
    }
}

inline bool SDLSoundSingleton::isOpened() {
    cerr << "SDLSoundSingleton::isOpened()" << endl;
    return mIsOpened;
}

bool SDLSoundSingleton::play(const UrbiSound* owner, const string& file) {
    cerr << "SDLSoundSingleton::play(" << owner << ", " << file << ")" << endl;
    if(!openDevice())
        return false;
    
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    if (SDL_LoadWAV(file.c_str(), &wave, &data, &dlen) == NULL) {
        cerr << "Couldn't load " << file << ": " << SDL_GetError() << endl;
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
    
    stop(owner);
    SDL_LockAudio();
    Uint8Pointer sampleData(cvt.buf);
    mSampleList.push_back(Sample(owner, sampleData, cvt.len_cvt));
    SDL_PauseAudio(0);
    SDL_UnlockAudio();
    
    return true;
}

inline void SDLSoundSingleton::stop(const UrbiSound* owner) {
    cerr << "SDLSoundSingleton::stop(" << owner << ")" << endl;
    SDL_LockAudio();
    mSampleList.remove_if(SampleBelongsTo(owner));
    SDL_UnlockAudio();
}

inline SampleList &SDLSoundSingleton::getSampleList() {
    return mSampleList;
}


//------------------------- UrbiSound ----------------------------//

UrbiSound::UrbiSound(const std::string& name) : UObject(name) {
    //Bind the functions
    UBindFunction(UrbiSound, play);
    UBindFunction(UrbiSound, stop);
    UBindFunction(UrbiSound, isDeviceOpened);
    UBindFunction(UrbiSound, isPlaying);
}

UrbiSound::~UrbiSound() {
}

bool UrbiSound::play(const std::string& file) {
    //stop();
    return SDLSoundSingleton::getInstance().play(this, file);
}

void UrbiSound::stop() {
    SDLSoundSingleton::getInstance().stop(this);
    return;
}

bool UrbiSound::isDeviceOpened() {
    return true;
}

bool UrbiSound::isPlaying() {
    return true;
}