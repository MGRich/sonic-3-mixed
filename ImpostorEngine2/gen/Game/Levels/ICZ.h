#ifndef LEVEL_ICZ_H
#define LEVEL_ICZ_H

#define PUBLIC
#define PRIVATE
#define PROTECTED
#define STATIC
#define VIRTUAL

class ISprite;

#include <Game/LevelScene.h>

class Level_ICZ : public LevelScene {
public:
    ISprite* ICZObjectSprite = NULL;

    Level_ICZ(IApp* app, IGraphics* g, int act);
    void RestartStage(bool doActTransition, bool drawBackground);
    void AssignSpriteMapIDs();
    void LoadZoneSpecificSprites();
    void EarlyUpdate();
    void Subupdate();
    void HandleCamera();
};

#endif /* LEVEL_ICZ_H */
