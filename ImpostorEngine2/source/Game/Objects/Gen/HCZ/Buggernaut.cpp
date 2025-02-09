// Object ID: 0x95
// Object Name: Buggernaut.cpp

#include "Buggernaut.h"

typedef IMath Math;

void Buggernaut::Create() {
    Enemy::Create();
    Active = true;
    Priority = false;
    W = 24;
    H = 24;
    Sprite = LevelScene::LoadSpriteFromBin("HCZ/Buggernaut.bin", SaveGame::CurrentMode);
    CurrentAnimation = Sprite->FindAnimation("Body");
    Flip = 1;
    if (FlipX) Flip = -1;

    HitCount = 1;
    Routine = 2;
    TimerRoutine = 2;
    Timer = 0x3F;
    MaximumSpeed = 0x200;
    Acceleration = 0x10;
    SubX = X << 16;
    SubY = Y << 16;
    ChildSubX = (X + 0x20) << 16;
    ChildSubY = Y << 16;
    ChildXSpeed = 0;
    ChildYSpeed = 0;
    ChildFlip = -1;
    VisualLayer = 1;
}

int Buggernaut::OnHit() {
    return OnDeath();
}

void Buggernaut::Update() {
    if (!isHeldDebugObject) {
        if (Routine == 4) {
            if (Scene->Player->EZX < X) {
                if (XSpeed > -MaximumSpeed) XSpeed -= Acceleration;

                Flip = -1;
            }
            else {
                if (XSpeed < MaximumSpeed) XSpeed += Acceleration;

                Flip = 1;
            }
            if (Scene->Player->EZY < Y) {
                if (YSpeed > -MaximumSpeed) YSpeed -= Acceleration;

            }
            else {
                if (YSpeed < MaximumSpeed) YSpeed += Acceleration;

            }
            if (Y > Scene->WaterLevel - 8) YSpeed = -0x200;

            SubX += XSpeed << 8;
            SubY += YSpeed << 8;
        }

        if (SubX < ChildSubX) {
            if (ChildXSpeed > -MaximumSpeed) ChildXSpeed -= Acceleration << 1;

            ChildFlip = -1;
        }
        else {
            if (ChildXSpeed < MaximumSpeed) ChildXSpeed += Acceleration << 1;

            ChildFlip = 1;
        }
        if (SubY < ChildSubY) {
            if (ChildYSpeed > -MaximumSpeed) ChildYSpeed -= Acceleration << 1;

        }
        else {
            if (ChildYSpeed < MaximumSpeed) ChildYSpeed += Acceleration << 1;

        }
        if ((ChildSubY >> 16) > Scene->WaterLevel - 8) ChildYSpeed = -0x200;

        ChildSubX += ChildXSpeed << 8;
        ChildSubY += ChildYSpeed << 8;
        if (Timer > 0) {
            Timer -= 1;
        }
        else {
            if (TimerRoutine == 0) {
                Routine = 2;
                TimerRoutine = 2;
                Timer = 0x3F;
            }
            else if (TimerRoutine == 2) {
                Routine = 4;
                TimerRoutine = 0;
                Timer = 0x7F;
            }

        }
        if (Sprite->Animations.size() > CurrentAnimation) {
            if (Sprite->Animations[CurrentAnimation].AnimationSpeed > 2) Frame += Sprite->Animations[CurrentAnimation].AnimationSpeed;
            else if (Sprite->Animations[CurrentAnimation].Frames[Frame >> 8].Duration != 0) Frame += 0x100 / Sprite->Animations[CurrentAnimation].Frames[Frame >> 8].Duration;

            if (Frame >= Sprite->Animations[CurrentAnimation].FrameCount << 8) {
                Frame = Sprite->Animations[CurrentAnimation].FrameToLoop << 8;
            }

        }

        X = SubX >> 16;
        Y = SubY >> 16;
    }

    Object::Update();
}

void Buggernaut::Render(int CamX, int CamY) {
    G->DrawSprite(Sprite, 5, Frame >> 8, (ChildSubX >> 16) - CamX, (ChildSubY >> 16) - CamY, 0, Flip < 0 ? IE_NOFLIP : IE_FLIPX);
    G->DrawSprite(Sprite, 3, 0, (ChildSubX >> 16) - CamX, (ChildSubY >> 16) - CamY, 0, Flip < 0 ? IE_NOFLIP : IE_FLIPX);
    G->DrawSprite(Sprite, CurrentAnimation, Frame >> 8, X - CamX, Y - CamY, 0, Flip < 0 ? IE_NOFLIP : IE_FLIPX);
    G->DrawSprite(Sprite, 2, 0, X - CamX, Y - CamY, 0, Flip < 0 ? IE_NOFLIP : IE_FLIPX);
    if (App->viewObjectCollision) {
        G->SetDrawAlpha(0x80);
        G->DrawRectangle(X - (W / 2) - CamX, Y - (H / 2) - CamY, W, H, DrawCollisionsColor);
        G->SetDrawAlpha(0xFF);
    }

    }

