#if INTERFACE
#include <Game/LevelScene.h>

class Level_MGZ : public LevelScene {
public:
    int32_t FloorY = 0; //

    ISprite* MGZObjectsSprite = NULL;
};
#endif

#include <Game/Explosion.h>
#include <Game/LevelScene.h>

#include <Game/Objects/Gen/ObjectListing.h>

#include <Game/Levels/MGZ.h>
#include <Game/Levels/CNZ.h>
#include <Game/Levels/FBZ.h>

#define ADD_OBJECT() ObjectProp op; op.X = X; op.Y = Y; op.ID = ID; op.SubType = SubType; op.LoadFlag = PRIORITY; op.FlipX = FLIPX; op.FlipY = FLIPY; ObjectProps[ObjectPropCount++] = op; Object* obj = GetNewObjectFromID(ID); if (obj) { obj->G = G; obj->App = App; obj->Scene = this; obj->InitialX = X; obj->InitialY = Y; obj->FlipX = FLIPX == 1; obj->FlipY = FLIPY == 1; while (!SpriteMapIDs[ID]) ID--; obj->Sprite = SpriteMapIDs[ID]; obj->SubType = SubType; obj->Create(); Objects[ObjectCount++] = obj; }

PUBLIC Level_MGZ::Level_MGZ(IApp* app, IGraphics* g, int act) : LevelScene(app, g) {
    ZoneID = 3;
    VisualAct = Act = act;

    if (Act == 1) {
		Sound::SoundBank[0] = new ISound("Music/MGZ1.ogg", true);
		// Sound::Audio->LoopPoint[0] = 1007820;

        Str_TileConfigBin = "Stages/MGZ1/TileConfig.bin";
        Str_SceneBin = "Stages/MGZ1/Scene.bin";
        Str_TileSprite = "Stages/MGZ1/16x16Tiles.gif";
        Str_AnimatedSprites = "Stages/MGZ1/Animated Tiles.gif";
    }
    else {
        Sound::SoundBank[0] = new ISound("Music/MGZ2.ogg", true);
        Sound::Audio->LoopPoint[0] = 21609;
		// Sound::Audio->LoopPoint[0] = 1007820;

        Str_TileConfigBin = "Stages/MGZ2/TileConfig.bin";
        Str_SceneBin = "Stages/MGZ2/Scene.bin";
        Str_TileSprite = "Stages/MGZ2/16x16Tiles.gif";
        Str_AnimatedSprites = "Stages/MGZ2/Animated Tiles.gif";
    }

    sprintf(LevelName, "MARBLE GARDEN");
    sprintf(LevelNameDiscord, "Marble Garden");
}

PUBLIC void Level_MGZ::Init() {
    LevelScene::Init();
}

PUBLIC void Level_MGZ::RestartStage(bool doActTransition, bool drawBackground) {
    App->Audio->ClearMusic();
    App->Audio->PushMusic(Sound::SoundBank[0], true, Sound::Audio->LoopPoint[0]);

    LevelScene::RestartStage(doActTransition, drawBackground);

    // Data->layers[1].IsScrollingVertical = true;
    // Data->layers[2].IsScrollingVertical = true;

    if (Act == 1) {
        if (Checkpoint == -1)
            for (int p = 0; p < PlayerCount; p++)
                Players[p]->Action = ActionType::Peril;
    }
}

PUBLIC void Level_MGZ::FinishResults() {
    if (VisualAct == 1) {
        LevelScene::FinishResults();
    }
    else {
        FadeAction = FadeActionType::NEXT_ZONE;
        FadeTimerMax = 90;
        FadeMax = 0x140;
        G->FadeToWhite = false;
    }
}
PUBLIC void Level_MGZ::GoToNextAct() {
    if (VisualAct == 1) {
        Level_MGZ* NextAct = new Level_MGZ(App, G, 2);

        TransferCommonLevelData(NextAct);
        NextAct->MGZObjectsSprite = MGZObjectsSprite;
        // Enable Title Card with no fade-in
        NextAct->LevelCardTimer = 0.0;
        NextAct->FadeTimer = 0;
        NextAct->FadeAction = 0;
        NextAct->LevelCardHide = false;
        // Transfer over current frame
        NextAct->Frame = Frame;
        // Set player spawn position relative to their previous position
        NextAct->SpecialSpawnPositionX = Player->EZX - 0x2E00;
        NextAct->SpecialSpawnPositionY = Player->EZY - 0x600;
        NextAct->RoutineNumber = 0x00;

        App->NextScene = NextAct;
    }
    else {
        Level_CNZ* NextAct = new Level_CNZ(App, G, 1);
        TransferCommonLevelData(NextAct);
        App->NextScene = NextAct;
    }
}

PUBLIC void Level_MGZ::AssignSpriteMapIDs() {
    LevelScene::AssignSpriteMapIDs();

	SpriteMapIDs[0x0D] = MGZObjectsSprite;
    SpriteMapIDs[0x0F] = MGZObjectsSprite;
	SpriteMapIDs[0x2F] = MGZObjectsSprite;
	SpriteMapIDs[0x51] = MGZObjectsSprite;
}

PUBLIC void Level_MGZ::LoadZoneSpecificSprites() {
	if (!MGZObjectsSprite) {
		MGZObjectsSprite = new ISprite("Sprites/MGZ/Objects.gif", App);
        MGZObjectsSprite->Print = true;
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Collapsing Bridge.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Dash Trigger.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Floating Platform.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Head Trigger.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Pulley.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Breakable Wall.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Trigger Platform.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Swinging Platform.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Moving Spike Platform.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Smashing Pillar.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Swinging Spike Ball.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Top Platform.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Miniboss.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/End Boss.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Non Animated Sprites.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Bubbles Badnik.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Spiker.bin");
        MGZObjectsSprite->LoadAnimation("Sprites/MGZ/Mantis.bin");
		// printf("\n");
	}

    if (!KnuxSprite[0]) {
        KnuxSprite[0] = new ISprite("Player/Knux1.gif", App);
        KnuxSprite[1] = new ISprite("Player/Knux2.gif", App);
        KnuxSprite[2] = new ISprite("Player/Knux3.gif", App);
        KnuxSprite[3] = new ISprite("Player/KnuxCutsceneAIZ.gif", App);
        KnuxSprite[4] = new ISprite("Player/KnuxCutsceneHPZ.gif", App);

        KnuxSprite[0]->LoadAnimation("Player/Knux.bin");
        KnuxSprite[1]->LinkAnimation(KnuxSprite[0]->Animations);
        KnuxSprite[2]->LinkAnimation(KnuxSprite[0]->Animations);
        KnuxSprite[3]->LinkAnimation(KnuxSprite[0]->Animations);
        KnuxSprite[4]->LinkAnimation(KnuxSprite[0]->Animations);
    }
}

PUBLIC void Level_MGZ::RenderAboveBackground() {

}
PUBLIC void Level_MGZ::RenderAboveForeground() {

}

PUBLIC void Level_MGZ::RenderEverything() {
    LevelScene::RenderEverything();
}

PUBLIC void Level_MGZ::EarlyUpdate() {
    // bool Thremixed = true;

    LevelScene::EarlyUpdate();
}
PUBLIC void Level_MGZ::Subupdate() {
    LevelScene::Subupdate();
}

PUBLIC void Level_MGZ::HandleCamera() {
    // int offX = App->WIDTH / 2 - 0xA0;
    // int offY = App->HEIGHT / 2 - 0x70;

    // Layer Flags:
    // 0x1: Collidable
    // 0x2: Don't Repeat X
    // 0x3: Don't Repeat Y

    if (Act == 1) {
        Data->layers[1].IsScrollingVertical = true;
        Data->layers[2].IsScrollingVertical = true;
    }
    else if (Act == 2) {
        Data->layers[1].Flags |= 1 | 0 | 4;
        Data->layers[1].OffsetX = 0x3480;
        if (RoutineNumber == 0) {
            Data->layers[1].OffsetY = 0x0B00;

            if (Player->EZX >= 0x36C0 &&
                Player->EZY >= 0xAD0) {
                RoutineNumber = 1;
            }
        }

        if (RoutineNumber == 1) {
            if (!(Frame & 0xF)) {
                Sound::Play(Sound::SFX_RUMBLE);
            }
            if ((FloorY >> 16) < 0x1C0) {
                FloorY += 0x00006000;
                ShakeTimer = -1;
            }
            if ((FloorY >> 16) >= 0x1C0) {
                ShakeTimer = 20;
                Sound::Stop(Sound::SFX_RUMBLE);
                Sound::Play(Sound::SFX_IMPACT4);
                RoutineNumber = 2;
            }
        }
        Data->layers[1].OffsetY = 0x0B00 - (FloorY >> 16);

        if (Player->EZX >= 0x3000 &&
            Player->EZY >= 0x0680 &&
            Player->EZX <  0x3660 &&
            Player->EZY <  0x07F0) {
            Data->layers[0].RelativeY = 0x0100;
            Data->layers[0].OffsetX = 0x3580;
            Data->layers[0].OffsetY = -0x8F0 + (FloorY >> 16);

            BackgroundColor = TileSprite->GetPalette(0x3A);
        }
        else if (Player->EZX >= 0x3000 &&
            Player->EZY >= 0x07F0 &&
            Player->EZX <  0x3E80 &&
            Player->EZY <  0x0C00) {
            Data->layers[0].RelativeY = 0x0100;
            Data->layers[0].OffsetX = 0x3580;
            Data->layers[0].OffsetY = -0x8F0 + (FloorY >> 16);

            BackgroundColor = TileSprite->GetPalette(0x3A);
        }
        else if (Player->EZX > 0x3920 && Player->EZY < 0x7E8) {
            Data->layers[0].RelativeY = 0x0030;
            Data->layers[0].OffsetX = 0;
            Data->layers[0].OffsetY = -0x00F0;

            BackgroundColor = TileSprite->GetPalette(0x31);
        }
        else {
            Data->layers[0].RelativeY = 0x0030;
            Data->layers[0].OffsetY = 0;
            BackgroundColor = TileSprite->GetPalette(0x3A);
        }


    }

    LevelScene::HandleCamera();
}
