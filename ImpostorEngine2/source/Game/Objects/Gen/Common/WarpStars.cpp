// Object ID: 0xFFFFFFFF
// Object Name: WarpStars.cpp

#include "WarpStars.h"

typedef IMath Math;

void WarpStars::Create() {
    Object::Create();
    Active = false;
    Priority = true;
    DoDeform = false;
    Sprite = LevelScene::LoadSpriteFromBin("Global/StarPost.bin", SaveGame::CurrentMode);
    W = 16;
    H = 16;
    Rotation = 0;
    Frame = 0;
    CurrentAnimation = Sprite->FindAnimation("Stars 1");
    CleanupInactiveObject = false;
}

void WarpStars::Update() {
    if (Active) {
        if (Timer > 0) Timer--;
        else Timer = 0;
    }
    else {
        Timer = 64;
    }
    Object::Update();
}

void WarpStars::DrawStar(int Rock, int Ang, int Side, int CamX, int CamY) {
    int cosval = (Math::cosHex(Rock) * 16) >> 16;
    int xx = (cosval * Math::cosHex(Ang)) >> 16;
    int yy = (cosval * Math::sinHex(Ang)) >> 16;
    if (Math::sinHex(Rock) * Side >= 0) {
        G->DrawSprite(Sprite, Sprite->Animations[5].Frames[0].X, Sprite->Animations[5].Frames[0].Y, Sprite->Animations[5].Frames[0].W, Sprite->Animations[5].Frames[0].H, X - CamX + xx, Y - CamY + yy, 0, IE_NOFLIP, Sprite->Animations[5].Frames[0].OffX, Sprite->Animations[5].Frames[0].OffY);
    }

}

void WarpStars::Render(int CamX, int CamY) {
    if (Active) {
        G->DrawSprite(Sprite, CurrentAnimation, 0, this->X - CamX, (this->Y) - CamY, 0, IE_NOFLIP);
        if (App->viewObjectCollision) {
            G->SetDrawAlpha(0x80);
            G->DrawRectangle(X - (W / 2) - CamX, Y - (H / 2) - CamY, W, H, DrawCollisionsColor);
            G->SetDrawAlpha(0xFF);
        }

    }

    }

int WarpStars::OnCollisionWithPlayer(int PlayerID, int HitFrom, int Data) {
    if (PlayerID != 0) return 0;

    if (!Active) return 0;

    if (Timer != 0) return 0;

    if (Scene->Player->Rings > 20 && Scene->Player->Rings < 35) {
        Scene->NextBonusStage = 2;
    }
    else if (Scene->Player->Rings > 34 && Scene->Player->Rings < 50) {
        Scene->NextBonusStage = 1;
    }
    else if (Scene->Player->Rings >= 50) {
        Scene->NextBonusStage = 0;
    }
    else {
        return 0;
    }
    Scene->Player->ObjectControlled = 0xFF;
    Scene->Player->Hidden = true;
    Scene->StopTimer = true;
    GoTimer = 48;
    App->Audio->FadeMusic(1.0);
    Scene->SaveState();
    Scene->FadeAction = 5;
    Scene->FadeTimerMax = 48 + 48;
    Scene->FadeMax = 0x200;
    return 1;
}

