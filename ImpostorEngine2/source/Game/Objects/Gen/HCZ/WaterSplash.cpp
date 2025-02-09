// Object ID: 0x6D
// Object Name: WaterSplash.cpp

#include "WaterSplash.h"

typedef IMath Math;

void WaterSplash::Create() {
    Object::Create();
    this->Active = true;
    Sprite = LevelScene::LoadSpriteFromBin("HCZ/WaterGush.bin", SaveGame::CurrentMode);
    this->CurrentAnimation = 13;
    this->W = 48;
    this->H = 32;
    this->X += 4;
    this->Y -= 8;
}

void WaterSplash::Render(int CamX, int CamY) {
    Frame = (Scene->Frame >> 1) % 6;
    G->DrawSprite(Sprite, CurrentAnimation, Frame, X - CamX, Y - CamY, 0, IE_NOFLIP);
    if (App->viewObjectCollision) {
        G->SetDrawAlpha(0x80);
        G->DrawRectangle(X - (W / 2) - CamX, Y - (H / 2) - CamY, W, H, DrawCollisionsColor);
        G->SetDrawAlpha(0xFF);
    }

    }

