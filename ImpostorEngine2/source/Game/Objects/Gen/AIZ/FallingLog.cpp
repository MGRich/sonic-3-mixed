// Object ID: 0x2D
// Object Name: FallingLog.cpp

#include "FallingLog.h"

typedef IMath Math;

CONSTRUCTER FallingLog::FallingLog() {
}

void FallingLog::Create() {
    Object::Create();
    Active = true;
    Priority = false;
    W = 48;
    H = 14;
    if (Scene->Act == 1) {
        Sprite = LevelScene::LoadSpriteFromBin("AIZ/Falling Log (Act 1).bin", SaveGame::CurrentMode);
    }
    else {
        Sprite = LevelScene::LoadSpriteFromBin("AIZ/Falling Log (Act 2).bin", SaveGame::CurrentMode);
    }
    SolidTop = true;
    Scene->AddSelfToRegistry(this, "Solid");
    Length = 0;
    LastY = Y;
    if (Scene->Act == 1) {
        CurrentAnimation = Sprite->FindAnimation("Falling Log (Act 1)");
    }
    else {
        CurrentAnimation = Sprite->FindAnimation("Falling Log (Act 2)");
    }
}

void FallingLog::Update() {
    Priority = true;
    LastY = Y;
    Length = Scene->WaterLevel - InitialY;
    Y = InitialY + Math::min(Frame, Length);
    Length = (Length / 0x80) * 0x80 + 0x80;
    Frame = (Frame + 1) % (Length);
    if (Math::abs(Scene->Player->EZX - X) > 0x200) {
        Priority = false;
        Y = InitialY;
    }

    Object::Update();
}

void FallingLog::Render(int CamX, int CamY) {
    Length = Scene->WaterLevel - InitialY;
    if (Frame < Length || Frame % 8 < 4) G->DrawSprite(Sprite, CurrentAnimation, 0, X - CamX, LastY - CamY, 0, IE_NOFLIP);

    G->DrawSprite(Sprite, CurrentAnimation + 2, (Frame & 0xF) / 4, X - CamX, LastY - CamY, 0, IE_NOFLIP);
    }

