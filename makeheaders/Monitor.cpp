// Object ID: 0x01
// Object Name: Monitor.cpp

#include "Monitor.h"

typedef Math IMath;

void Monitor::Create() {
    Object::Create();
    W = 26;
    H = 32;
    Active = true;
    Priority = false;
    Frame = 0;
    YSpeed = 0;
    SubY = Y << 8;
    Solid = true;
    Scene->AddSelfToRegistry(this, "Solid");
    BreakableByRoll = true;
    BreakableByJump = true;
    BreakableByGlide = true;
    Scene->AddSelfToRegistry(this, "Breakable");
    PlayerAggressor = 0;
    CanFall = false;
    GhostY = 0.0;
    SubTypeFrame = 0;
    switch (SubType) {
        case ItemTypes::TYPE_STATIC:
        SubTypeFrame = 14;
        break;
        case ItemTypes::TYPE_1UP:
        SubTypeFrame = 7 + Scene->Player->Character;
        break;
        case ItemTypes::TYPE_ROBOTNIK:
        SubTypeFrame = 10;
        break;
        case ItemTypes::TYPE_RINGS:
        SubTypeFrame = 0;
        break;
        case ItemTypes::TYPE_SPEED_SHOES:
        SubTypeFrame = 6;
        break;
        case ItemTypes::TYPE_SHIELD_FIRE:
        SubTypeFrame = 3;
        break;
        case ItemTypes::TYPE_SHIELD_LIGHTNING:
        SubTypeFrame = 4;
        break;
        case ItemTypes::TYPE_SHIELD_BUBBLE:
        SubTypeFrame = 2;
        break;
        case ItemTypes::TYPE_INVINCIBILITY:
        SubTypeFrame = 5;
        break;
        case ItemTypes::TYPE_SUPER:
        SubTypeFrame = 12;
        break;
        case ItemTypes::TYPE_HYPER_RING:
        SubTypeFrame = 11;
        break;
    }

    CurrentAnimation = 4;
}

void Monitor::Update() {
    if (!CanFall && (YSpeed != 0)) {
        CanFall = true;
    }

    if (CanFall) {
        if (YSpeed < 0 && Scene->CollisionAt(X, (SubY >> 8) - 17)) {
            YSpeed = 0;
        }

        YSpeed += 0x38;
        SubY += YSpeed;
        Y = SubY >> 8;
        while (Scene->CollisionAt(X, Y + 16 + 1)) {
            Y--;
            YSpeed = 0;
            Priority = false;
            CanFall = false;
        }

        if (!CanFall) Y += 2;

    }

    if (YSpeed != 0) Priority = true;

    if (Timer >= 0 && Timer < 64 + 24) Timer++;
    else if (!CanFall) Priority = false;

    if (Timer == 32) {
        switch (SubType) {
            case ItemTypes::TYPE_STATIC:
            Scene->Players[PlayerAggressor]->Hurt(X, false);
            break;
            case ItemTypes::TYPE_1UP:
            Scene->Players[PlayerAggressor]->GiveLife(1);
            break;
            case ItemTypes::TYPE_ROBOTNIK:
            Scene->Players[PlayerAggressor]->Hurt(X, false);
            break;
            case ItemTypes::TYPE_RINGS:
            Scene->Players[PlayerAggressor]->GiveRing(10);
            Sound->Play(Sound->SFX_RING);
            break;
            case ItemTypes::TYPE_SPEED_SHOES:
            Scene->Players[PlayerAggressor]->SpeedSneakersActive = true;
            Scene->Players[PlayerAggressor]->SpeedSneakersTimer = 1200;
            App->Audio->PushMusic(Sound->SoundBank[0xFE], false, 0);
            break;
            case ItemTypes::TYPE_SHIELD_FIRE:
            Scene->Players[PlayerAggressor]->Shield = ShieldType::Fire;
            Scene->Players[PlayerAggressor]->ShieldUsable = true;
            Scene->Players[PlayerAggressor]->ShieldAction = false;
            Sound->Play(Sound->SFX_SHIELD_FIRE);
            break;
            case ItemTypes::TYPE_SHIELD_LIGHTNING:
            Scene->Players[PlayerAggressor]->Shield = ShieldType::Electric;
            Scene->Players[PlayerAggressor]->ShieldUsable = true;
            Scene->Players[PlayerAggressor]->ShieldAction = false;
            Sound->Play(Sound->SFX_SHIELD_ELECTRIC);
            break;
            case ItemTypes::TYPE_SHIELD_BUBBLE:
            Scene->Players[PlayerAggressor]->Shield = ShieldType::Bubble;
            Scene->Players[PlayerAggressor]->ShieldUsable = true;
            Scene->Players[PlayerAggressor]->ShieldAction = false;
            Sound->Play(Sound->SFX_SHIELD_BUBBLE);
            App->Audio->RemoveMusic(Sound->SoundBank[0xFD]);
            break;
            case ItemTypes::TYPE_INVINCIBILITY:
            if (!Scene->Players[PlayerAggressor]->SuperForm && !Scene->Players[PlayerAggressor]->HyperForm) {
                Scene->Players[PlayerAggressor]->Invincibility = InvincibilityType::Full;
                Scene->Players[PlayerAggressor]->InvincibilityTimer = 1200;
                App->Audio->PushMusic(Sound->SoundBank[0xFF], true, 139623);
            }

            break;
            case ItemTypes::TYPE_SUPER:
            Scene->Players[PlayerAggressor]->SuperForm = true;
            Scene->Players[PlayerAggressor]->GiveRing(100);
            break;
            case ItemTypes::TYPE_HYPER_RING:
            Scene->Players[PlayerAggressor]->HyperRings = true;
            Scene->Players[PlayerAggressor]->GiveRing(10);
            Sound->Play(Sound->SFX_RING);
            break;
        }

    }

    if (!BreakableByRoll) {
        GhostY += (-35.0 - GhostY) / 10.0;
        Priority = true;
        if (Math::floor(GhostY) == -35.0) {
            GhostY = -35.0;
        }

    }

    if (Sprite->Animations[CurrentAnimation].AnimationSpeed > 2) Frame += Sprite->Animations[CurrentAnimation].AnimationSpeed;
    else if (Sprite->Animations[CurrentAnimation].Frames[Frame / 0x100]->Duration != 0) Frame += 0x100 / Sprite->Animations[CurrentAnimation].Frames[Frame / 0x100]->Duration;

    if (Frame / 0x100 >= Sprite->Animations[CurrentAnimation].FrameCount - 1) {
        Frame = Sprite->Animations[CurrentAnimation].FrameToLoop * 0x100;
    }

    Object::Update();
}

void Monitor::Render(int CamX, int CamY) {
    if (CurrentAnimation == 4) {
        G->DrawSprite(Sprite, 0, 0, X - CamX, Y - CamY, 0, IE_NOFLIP);
        G->DrawModeOverlay = true;
        G->DrawSprite(Sprite, 3, Scene->Frame >> 1 & 1, X - CamX, Y - CamY - 2, 0, IE_NOFLIP);
        G->DrawModeOverlay = false;
    }
    else {
        G->DrawSprite(Sprite, CurrentAnimation, Frame >> 8, X - CamX, Y - CamY, 0, IE_NOFLIP);
    }
    if (CurrentAnimation == 1 && (Timer >= 64 && Timer % 3 != 0)) return;

    G->DrawSprite(Sprite, 2, SubTypeFrame, X - CamX, Y + (int)(-CamY - 5 + GhostY), 0, IE_NOFLIP);
    if (CurrentAnimation == 4) {
        G->DrawModeOverlay = true;
        G->DrawAlpha = 0x80;
        G->DrawSprite(Sprite, CurrentAnimation, Frame >> 8, X - CamX, Y - CamY - 2, 0, IE_NOFLIP);
        G->DrawModeOverlay = false;
        G->DrawAlpha = 0xFF;
    }

    Object::Render(CamX, CamY);
}

int Monitor::OnBreakHorizontal(int PlayerID, int HitFrom) {
    BreakableByRoll = false;
    BreakableByJump = false;
    BreakableByGlide = false;
    Solid = false;
    Timer = 0;
    Priority = true;
    Sound->Play(Sound->SFX_DESTROY);
    ChangeAnimation(1);
    Scene->AddExplosion(3, false, X, Y - 12);
    return 1;
}

int Monitor::OnBreakVertical(int PlayerID, int HitFrom) {
    return OnBreakHorizontal(PlayerID, HitFrom);
}

