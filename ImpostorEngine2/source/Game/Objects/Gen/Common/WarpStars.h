#ifndef WARPSTARS_H
#define WARPSTARS_H

class WarpStars;

#include <Utils/Standard.h>

#include <Engine/IApp.h>
#include <Engine/ISprite.h>

#include <Game/Object.h>
#include <Game/LevelScene.h>
#include <Game/Objects/Gen/ObjectListing.h>

class WarpStars: public Object {
public:    
    
    int GoTimer;
    int StoredX;
    int StoredY;
    
    void Create();
    void Update();
    void DrawStar(int Rock, int Ang, int Side, int CamX, int CamY);
    void Render(int CamX, int CamY);
    int OnCollisionWithPlayer(int PlayerID, int HitFrom, int Data);
};

#endif /* WARPSTARS_H */
