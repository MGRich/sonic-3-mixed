#if INTERFACE
#include <Game/LevelScene.h>

class Level_LRZ : public LevelScene {
public:

};
#endif

#include <Game/Explosion.h>
#include <Game/LevelScene.h>

#include <Game/Objects/Gen/ObjectListing.h>

#include <Game/Levels/LRZ.h>

PUBLIC Level_LRZ::Level_LRZ(IApp* app, IGraphics* g, int act) : LevelScene(app, g) {
    ZoneID = 10;
    VisualAct = Act = act;
	sprintf(ZoneLetters, "LRZ");

	if (SaveGame::CurrentMode >= 1)
	{
		if (Act == 1) {
			Str_TileConfigBin = "Mixed/Stages/LRZ1/TileConfig.bin";
			Str_SceneBin = "Mixed/Stages/LRZ1/Scene.bin";
			Str_TileSprite = "Mixed/Stages/LRZ1/16x16Tiles.gif";
			Str_AnimatedSprites = "Mixed/Stages/LRZ1/Animated Tiles.gif";
			Str_StageBin = "Mixed/Stages/LRZ1/Stageconfig.bin";
		}
		else {
			Str_TileConfigBin = "Mixed/Stages/LRZ2/TileConfig.bin";
			Str_SceneBin = "Mixed/Stages/LRZ2/Scene.bin";
			Str_TileSprite = "Mixed/Stages/LRZ2/16x16Tiles.gif";
			Str_AnimatedSprites = "Mixed/Stages/LRZ2/Animated Tiles.gif";
			Str_StageBin = "Mixed/Stages/LRZ2/Stageconfig.bin";
		}
	}
	else
	{
		if (Act == 1) {
			Str_TileConfigBin = "Classic/Stages/LRZ1/TileConfig.bin";
			Str_SceneBin = "Classic/Stages/LRZ1/Scene.bin";
			Str_TileSprite = "Classic/Stages/LRZ1/16x16Tiles.gif";
			Str_AnimatedSprites = "Classic/Stages/LRZ1/Animated Tiles.gif";
			Str_StageBin = "Classic/Stages/LRZ1/Stageconfig.bin";
		}
		else {
			Str_TileConfigBin = "Classic/Stages/LRZ2/TileConfig.bin";
			Str_SceneBin = "Classic/Stages/LRZ2/Scene.bin";
			Str_TileSprite = "Classic/Stages/LRZ2/16x16Tiles.gif";
			Str_AnimatedSprites = "Classic/Stages/LRZ2/Animated Tiles.gif";
			Str_StageBin = "Classic/Stages/LRZ2/Stageconfig.bin";
		}
	}

    sprintf(LevelName, "LAVA REEF");
    sprintf(LevelNameDiscord, "Lava Reef");

    if (Act == 1) {

    }
    else if (Act == 2) {

    }
}

PUBLIC void Level_LRZ::Init() {
	LevelScene::Init();
}


PUBLIC void Level_LRZ::RestartStage(bool doActTransition, bool drawBackground) {
	App->Audio->ClearMusic();
	PlayMusic(Act, 0, SaveGame::CurrentMode);

    LevelScene::RestartStage(doActTransition, drawBackground);
}

PUBLIC void Level_LRZ::LoadZoneSpecificSprites() {
	if (!KnuxSprite[0]) {
		if (SaveGame::CurrentMode >= 1)
		{
			KnuxSprite[0] = new ISprite("Mixed/Sprites/Players/Knux1.gif", App);
			KnuxSprite[1] = new ISprite("Mixed/Sprites/Players/Knux2.gif", App);
			KnuxSprite[2] = new ISprite("Mixed/Sprites/Players/Knux3.gif", App);
			KnuxSprite[3] = new ISprite("Mixed/Sprites/Players/KnuxCutsceneAIZ.gif", App);
			KnuxSprite[4] = new ISprite("Mixed/Sprites/Players/KnuxCutsceneHPZ.gif", App);

			KnuxSprite[0]->LoadAnimation("Mixed/Sprites/Players/Knux.bin");
		}
		else
		{
			KnuxSprite[0] = new ISprite("Classic/Sprites/Players/Knux1.gif", App);
			KnuxSprite[1] = new ISprite("Classic/Sprites/Players/Knux2.gif", App);
			KnuxSprite[2] = new ISprite("Classic/Sprites/Players/Knux3.gif", App);
			KnuxSprite[3] = new ISprite("Classic/Sprites/Players/KnuxCutsceneAIZ.gif", App);
			KnuxSprite[4] = new ISprite("Classic/Sprites/Players/KnuxCutsceneHPZ.gif", App);

			KnuxSprite[0]->LoadAnimation("Classic/Sprites/Players/Knux.bin");
		}
		KnuxSprite[1]->LinkAnimation(KnuxSprite[0]->Animations);
		KnuxSprite[2]->LinkAnimation(KnuxSprite[0]->Animations);
		KnuxSprite[3]->LinkAnimation(KnuxSprite[0]->Animations);
		KnuxSprite[4]->LinkAnimation(KnuxSprite[0]->Animations);
	}
}

PUBLIC void Level_LRZ::EarlyUpdate() {
    if (Act == 1) {

    }
    else if (Act == 2) {

    }
    LevelScene::EarlyUpdate();
}
PUBLIC void Level_LRZ::Subupdate() {
    LevelScene::Subupdate();
}

PUBLIC void Level_LRZ::HandleCamera() {
    if (Act == 1) {

    }
    else if (Act == 2) {

    }
    LevelScene::HandleCamera();
}
