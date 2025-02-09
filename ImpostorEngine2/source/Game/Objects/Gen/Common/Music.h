#ifndef MUSIC_H
#define MUSIC_H

class Music;

#include <Utils/Standard.h>

#include <Engine/IApp.h>
#include <Engine/ISprite.h>

#include <Game/Object.h>
#include <Game/LevelScene.h>
#include <Game/Objects/Gen/ObjectListing.h>

class Music: public Object {
public:    
    
    char trackFile[128];
    char soundtestName[128];
    uint32_t trackID;
    uint32_t trackLoop;
    bool playOnLoad;
    bool loopTrack;
    
    void Create();
    void PlayMusic();
    void UpdateSubType();
};

#endif /* MUSIC_H */
