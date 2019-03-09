#if INTERFACE
#include <Game/LevelScene.h>

class Level_CGZ : public LevelScene {
public:
    int  CutsceneRoutineNumber = 0x00;
};
#endif

#include <Game/Explosion.h>
#include <Game/LevelScene.h>

#include <Game/Objects/Gen/ObjectListing.h>

#include <Game/Levels/CGZ.h>

PUBLIC Level_CGZ::Level_CGZ(IApp* app, IGraphics* g, int act) : LevelScene(app, g) {
    ZoneID = 7;
    Act = act;
	sprintf(ZoneLetters, "CGZ");
	PlayMusic(act, 0, SaveGame::CurrentMode);

    if (Act == 1) {
        Str_TileConfigBin = "Stages/CGZ/TileConfig1.bin";
        Str_SceneBin = "Stages/CGZ/Scene1.bin";
        Str_TileSprite = "Stages/CGZ/16x16Tiles1.gif";
        //Str_AnimatedSprites = "Sprites/CGZ/AnimatedTiles";
        Str_ObjectsList = "Stages/CGZ/Objects1.bin";
        Str_RingsList = "Stages/CGZ/Rings1.bin";
    }

    sprintf(LevelName, "CHROME GADGET");
    sprintf(LevelNameDiscord, "Chrome Gadget");

    if (Act == 1) {
        // PlayerStartX = 0x00D8;
        // PlayerStartY = 0x0500;
    }
    else {

    }
}

PUBLIC void Level_CGZ::LoadZoneSpecificSprites() {
	if (!KnuxSprite[0]) {
        KnuxSprite[0] = new ISprite("Sprites/Players/Knux1.gif", App);
        KnuxSprite[1] = new ISprite("Sprites/Players/Knux2.gif", App);
        KnuxSprite[2] = new ISprite("Sprites/Players/Knux3.gif", App);
        KnuxSprite[3] = new ISprite("Sprites/Players/KnuxCutsceneAIZ.gif", App);
        KnuxSprite[4] = new ISprite("Sprites/Players/KnuxCutsceneHPZ.gif", App);

        KnuxSprite[0]->LoadAnimation("Sprites/Players/Knux.bin");
        KnuxSprite[1]->LinkAnimation(KnuxSprite[0]->Animations);
        KnuxSprite[2]->LinkAnimation(KnuxSprite[0]->Animations);
        KnuxSprite[3]->LinkAnimation(KnuxSprite[0]->Animations);
        KnuxSprite[4]->LinkAnimation(KnuxSprite[0]->Animations);
    }
}

PUBLIC void Level_CGZ::RestartStage(bool doActTransition, bool drawBackground) {
    LevelScene::RestartStage(doActTransition, drawBackground);

    if (Act == 1) {

    }
}

PUBLIC void Level_CGZ::EarlyUpdate() {
    if (Act == 1) {

    }
    else if (Act == 2) {

    }

    LevelScene::EarlyUpdate();
}
PUBLIC void Level_CGZ::Subupdate() {
    LevelScene::Subupdate();
}

PUBLIC void Level_CGZ::HandleCamera() {
    if (Act == 1) {
        //int off = App->WIDTH / 2 - 0xA0;
        //int ToS3X = CameraX + off;
        if (false) { // SKAlone
            CameraMinX = 0x680;
            if (CameraX < 0x680) {
                CameraX = 0x680;
            }
        }
        else {
            if (Player->EZY < 0x580) {
                CameraMinX = 0xC0;
                if (CameraX < 0xC0) {
                    CameraX = 0xC0;
                }
            }
        }

        /*int MaxY = 0x0;
        MaxY += 0xE0;
        MaxY -= App->HEIGHT;
        if (CameraY > MaxY) {
            CameraY = MaxY;
        }*/
    }

    LevelScene::HandleCamera();
}
