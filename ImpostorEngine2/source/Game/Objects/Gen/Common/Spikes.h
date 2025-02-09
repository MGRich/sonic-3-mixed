#ifndef SPIKES_H
#define SPIKES_H

class Spikes;

#include <Utils/Standard.h>

#include <Engine/IApp.h>
#include <Engine/ISprite.h>

#include <Game/Object.h>
#include <Game/LevelScene.h>
#include <Game/Objects/Gen/ObjectListing.h>

class Spikes: public Object {
public:    
    
    int Rotation;
    int Count;
    int Vertical;
    bool In;
    bool Moving;
    int Type;
    int TimerMax;
    int Stagger;
    int PlaneFilter;
    
    void Create();
    void Update();
    void Render(int CamX, int CamY);
    int OnCollisionWithPlayer(int PlayerID, int HitFrom, int Data);
};

#endif /* SPIKES_H */
