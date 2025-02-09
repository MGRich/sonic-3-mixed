// Object ID: 0xB6
// Object Name: IceCube.cpp

#include "IceCube.h"

typedef IMath Math;

void IceCube::Create() {
    Object::Create();
    Active = true;
    Priority = false;
    Solid = true;
    VisualLayer = 1;
    H = 32;
    W = 48;
    Scene->AddSelfToRegistry(this, "Solid");
    BreakableByJump = CollideSide::TOP;
    Scene->AddSelfToRegistry(this, "Breakable");
    Sprite = LevelScene::LoadSpriteFromBin("ICZ/Ice Cube.bin", SaveGame::CurrentMode);
    CurrentAnimation = Sprite->FindAnimation("Ice Cube");
}

void IceCube::Render(int CamX, int CamY) {
    if (DrawCollisions) {
        G->SetDrawAlpha(0x80);
        G->DrawRectangle(X - CamX, Y - CamY, W, H, DrawCollisionsColor);
        G->SetDrawAlpha(0x80);
    }

    G->DrawSprite(Sprite, CurrentAnimation, Frame >> 8, X - CamX, Y - CamY, 0, IE_NOFLIP);
    }

void IceCube::Break() {
    Scene->AddMovingSprite(Sprite, X + 12, Y + 4, CurrentAnimation + 1, 0 + (4 & 3), false, false, 0x1C0, -0x1C0, 0xA0);
    Scene->AddMovingSprite(Sprite, X - 12, Y + 4, CurrentAnimation + 1, 0 + (3 & 3), false, false, -0x1B0, -0x1C0, 0xA0);
    Scene->AddMovingSprite(Sprite, X + 12, Y - 4, CurrentAnimation + 1, 0 + (2 & 3), false, false, 0x100, -0x1E0, 0xA0);
    Scene->AddMovingSprite(Sprite, X - 4, Y - 4, CurrentAnimation + 1, 0 + (1 & 3), false, false, -0x100, -0x200, 0xA0);
}

int IceCube::OnBreakVertical(int PlayerID, int HitFrom) {
    if (HitFrom != CollideSide::TOP) return 0;

    Solid = false;
    Visible = false;
    Active = false;
    Sound::Play(Sound::SFX_COLLAPSE);
    BreakableByJump = CollideSide::NONE;
    Break();
    return 1;
}

int IceCube::OnCollisionWithPlayer(int PlayerID, int HitFrom, int Data) {
    if (!Solid) return 0;

    if (Scene->Players[PlayerID]->Shield != ShieldType::Fire) return 0;

    if (Scene->Players[PlayerID]->Shield == ShieldType::Fire) {
        Solid = false;
        Visible = false;
        Active = false;
        BreakableByJump = CollideSide::NONE;
        Break();
        return 1;
    }

    return 0;
}

