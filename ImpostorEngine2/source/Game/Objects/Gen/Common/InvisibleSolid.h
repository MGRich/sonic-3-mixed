#ifndef INVISIBLESOLID_H
#define INVISIBLESOLID_H

class InvisibleSolid;

#include <Utils/Standard.h>

#include <Engine/IApp.h>
#include <Engine/ISprite.h>

#include <Game/Object.h>
#include <Game/LevelScene.h>
#include <Game/Objects/Gen/ObjectListing.h>

class InvisibleSolid: public Object {
public:    
    
    bool Painful;
    bool Deadly;
    int Rotation;
    int Size;
    
    void Create();
    void Render(int CamX, int CamY);
    int OnCollisionWithPlayer(int PlayerID, int HitFrom, int Data);
};

#endif /* INVISIBLESOLID_H */
