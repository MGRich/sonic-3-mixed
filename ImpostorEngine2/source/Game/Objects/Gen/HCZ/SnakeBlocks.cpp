// Object ID: 0x67
// Object Name: SnakeBlocks.cpp

#include "SnakeBlocks.h"

typedef IMath Math;

void SnakeBlocks::Create() {
    Object::Create();
    Active = true;
    Priority = true;
    Solid = true;
    Scene->AddSelfToRegistry(this, "Solid");
    W = 32;
    H = 32;
    Sprite = LevelScene::LoadSpriteFromBin("HCZ/Platform.bin", SaveGame::CurrentMode);
    CurrentAnimation = Sprite->FindAnimation("Main");
    Timer = SubType + Scene->Frame;
    Offset = (SubType & 0x7F);
    Reverse = false;
    if (SubType & 0x80) {
        Reverse = true;
    }

    DirX = 1;
    DirY = 1;
}

void SnakeBlocks::Update() {
    Object::Update();
}

void SnakeBlocks::Render(int CamX, int CamY) {
    G->DrawSprite(Sprite, CurrentAnimation, 2, X - CamX, Y - CamY, 0, IE_NOFLIP);
    if (App->viewObjectCollision) {
        G->SetDrawAlpha(0x80);
        G->DrawRectangle(X - (W / 2) - CamX, Y - (H / 2) - CamY, W, H, DrawCollisionsColor);
        G->SetDrawAlpha(0xFF);
    }

    }

