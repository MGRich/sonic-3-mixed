// Object ID: 0x96
// Object Name: TurboSpiker.cpp

#include "TurboSpiker.h"

typedef IMath Math;

void TurboSpiker::Create() {
    Enemy::Create();
    Active = true;
    Priority = false;
    W = 32;
    H = 16;
    Y += 7;
    Sprite = LevelScene::LoadSpriteFromBin("HCZ/TurboSpiker.bin", SaveGame::CurrentMode);
    CurrentAnimation = Sprite->FindAnimation("Idle");
    SplashAnim = CurrentAnimation + 5;
    ShellAnim = CurrentAnimation + 2;
    Flip = 1;
    if (FlipX) Flip = -1;

    HitCount = 1;
    Fired = false;
    VisualLayer = -1;
    FireTimer = -30;
    MoveTimer = 0;
    KeptFlip = 0;
    TurnTimer = 0;
    Turning = false;
}

int TurboSpiker::OnHit() {
    return OnDeath();
}

void TurboSpiker::Update() {
    if (!isHeldDebugObject) {
        if (Math::pydistance(X, Y, Scene->Players[0]->X, Scene->Players[0]->Y) <= 0xA0 && VisualLayer == -1) {
            VisualLayer = 1;
            Sound::Play(Sound::SFX_SPLASH);
            FireTimer = 20;
        }

        FireTimer = FireTimer == -30 ? -30 : FireTimer - 1;
        TurnTimer = TurnTimer == 0 ? 0 : (TurnTimer < 0 ? TurnTimer + 1 : TurnTimer - 1);
        if (FireTimer < 0 && !Fired && Math::pydistance(X, Y, Scene->Players[0]->X, Scene->Players[0]->Y) <= 0x60) {
            Fired = true;
            KeptFlip = Flip;
            Scene->AddNewObject(Obj_TurboSpikerSpike, 0, X, Y, Flip < 0 ? false : true, false);
            Sound::Play(Sound::SFX_SLIDE);
            Gravity = 0x40;
        }

        if (Turning && TurnTimer == 0) {
            KeptFlip *= -1;
            Turning = false;
        }

        if (TurnTimer <= 0) XSpeed = 0x250 * Flip;
        else XSpeed = 0;
        if (FireTimer == -30 && Fired) VisualLayer = 0;

        if (Fired) CurrentAnimation = Sprite->FindAnimation("Body");

        if (TurnTimer > 0) CurrentAnimation = Sprite->FindAnimation("Turn");

        Flip = (Scene->Players[0]->X >= X && !Fired) ? -1 : (KeptFlip != 0 ? KeptFlip : 1);
        if (Sprite->Animations.size() > CurrentAnimation) {
            if (Sprite->Animations[CurrentAnimation].AnimationSpeed > 2) Frame += Sprite->Animations[CurrentAnimation].AnimationSpeed;
            else if (Sprite->Animations[CurrentAnimation].Frames[Frame >> 8].Duration != 0) Frame += 0x100 / Sprite->Animations[CurrentAnimation].Frames[Frame >> 8].Duration;

            if (Frame >= Sprite->Animations[CurrentAnimation].FrameCount << 8) {
                Frame = Sprite->Animations[CurrentAnimation].FrameToLoop << 8;
            }

        }

        if (Fired && !OnScreen) {
            Active = false;
            X = -32;
            Y = -32;
        }

    }

    Object::Update();
}

void TurboSpiker::MoveSprite() {
    if (!Fired) return;

    YSpeed += Gravity;
    SubX += XSpeed << 8;
    SubY += YSpeed << 8;
    for (int i = Y - 16; i < Y + 16; i++)
{
        if (Scene->CollisionAt(X, i)) {
            Y = i - 8;
            YSpeed = 0;
            break;
        }

    }
    if (XSpeed != 0) {
        if (((Scene->CollisionAt(X + (W / 2), Y - (H / 2)) && Flip == 1) || (Scene->CollisionAt(X - (W / 2), Y - (H / 2)) && Flip == -1)) && TurnTimer == 0) {
            TurnTimer = 10;
            Turning = true;
            XSpeed = 0;
        }

    }

}

void TurboSpiker::Render(int CamX, int CamY) {
    if (20 - FireTimer <= 5) G->DrawSprite(Sprite, SplashAnim, 20 - FireTimer, X - CamX, Y - CamY - 8, 0, Flip < 0 ? IE_NOFLIP : IE_FLIPX);

    if (!Fired) G->DrawSprite(Sprite, ShellAnim, 0, X - CamX, Y - CamY, 0, Flip < 0 ? IE_NOFLIP : IE_FLIPX);

    G->DrawSprite(Sprite, CurrentAnimation, Frame >> 8, X - CamX, Y - CamY, 0, Flip < 0 ? IE_NOFLIP : IE_FLIPX);
    if (DrawCollisions) {
        G->SetDrawAlpha(0x80);
        G->DrawRectangle(X - (W / 2) - CamX, Y - (H / 2) - CamY, W, H, DrawCollisionsColor);
        G->SetDrawAlpha(0xFF);
    }

    }

