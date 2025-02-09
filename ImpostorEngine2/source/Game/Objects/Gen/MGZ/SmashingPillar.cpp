// Object ID: 0x52
// Object Name: SmashingPillar.cpp

#include "SmashingPillar.h"

typedef IMath Math;

void SmashingPillar::Create() {
    Object::Create();
    Active = true;
    Priority = false;
    Solid = true;
    DoDeform = true;
    Scene->AddSelfToRegistry(this, "Solid");
    W = 62;
    H = 80;
    Sprite = LevelScene::LoadSpriteFromBin("MGZ/Smashing Pillar.bin", SaveGame::CurrentMode);
    CurrentAnimation = Sprite->FindAnimation("Smashing Pillar");
    OGY = Y - 5 + 5;
    Y += 10;
    YSpeed = -0xA0;
    Direction = true;
}

void SmashingPillar::Update() {
    if (Direction) {
        if (Y <= OGY) {
            YSpeed = 0;
            YSpeed = 0x300;
            Direction = false;
        }

    }
    else {
        YSpeed += 0x30;
        if (Y >= OGY + 80) {
            if (OnScreen) {
                Sound::Play(Sound::SFX_IMPACT4);
            }

            YSpeed = 0;
            Timer = 0;
            YSpeed = -0x110;
            Direction = true;
            Y = OGY + 80;
        }

    }
    Object::Update();
}

void SmashingPillar::Render(int CamX, int CamY) {
    if (!Visible) return;

    G->DrawSprite(Sprite, CurrentAnimation, Frame >> 8, X - CamX, Y - CamY, 0, IE_NOFLIP);
    if (App->viewObjectCollision) {
        G->SetDrawAlpha(0x80);
        G->DrawRectangle(X - (W / 2) - CamX, Y - (H / 2) - CamY, W, H, DrawCollisionsColor);
        G->SetDrawAlpha(0xFF);
    }

    }

int SmashingPillar::OnCollisionWithPlayer(int PlayerID, int HitFrom, int Data) {
    if (!Solid) return 0;

    if (HitFrom == CollideSide::BOTTOM && Visible && !Scene->Players[PlayerID]->Ground) {
        Scene->Players[PlayerID]->Hurt(X, true);
    }

    if (Scene->Players[PlayerID]->Ground && !Direction) {
        Scene->Players[PlayerID]->Die(false);
    }

    return 1;
}

