// Object ID: 0x2A
// Object Name: CorkFloor.cpp

#include "CorkFloor.h"

typedef IMath Math;

void CorkFloor::Create() {
    Object::Create();
    Active = true;
    Priority = false;
    Solid = true;
    Scene->AddSelfToRegistry(this, "Solid");
    BreakableFromTop = false;
    if (SubType == 1) BreakableFromTop = true;

    W = 32;
    H = 80;
    int i = 0;
    switch (Scene->ZoneID) {
        case 1:
        if (Scene->Act == 2) H = 88;

        VisualLayer = 1;
        i = 0;
        PieceVels[i++] = 0xFF00;
        PieceVels[i++] = 0xFE00;
        PieceVels[i++] = 0x0100;
        PieceVels[i++] = 0xFE00;
        PieceVels[i++] = 0xFF20;
        PieceVels[i++] = 0xFE40;
        PieceVels[i++] = 0x00E0;
        PieceVels[i++] = 0xFE40;
        PieceVels[i++] = 0xFF40;
        PieceVels[i++] = 0xFE80;
        PieceVels[i++] = 0x00C0;
        PieceVels[i++] = 0xFE80;
        PieceVels[i++] = 0xFF60;
        PieceVels[i++] = 0xFEC0;
        PieceVels[i++] = 0x00A0;
        PieceVels[i++] = 0xFEC0;
        PieceVels[i++] = 0xFF80;
        PieceVels[i++] = 0xFF00;
        PieceVels[i++] = 0x0080;
        PieceVels[i++] = 0xFF00;
        PieceVels[i++] = 0xFFA0;
        PieceVels[i++] = 0xFF40;
        PieceVels[i++] = 0x0060;
        PieceVels[i++] = 0xFF40;
        if (Scene->Act <= 1) {
            Sprite = LevelScene::LoadSpriteFromBin("AIZ/Cork Floor.bin", SaveGame::CurrentMode);
            CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        }
        else {
            Sprite = LevelScene::LoadSpriteFromBin("AIZ/Cork Floor 2.bin", SaveGame::CurrentMode);
            CurrentAnimation = Sprite->FindAnimation("Cork Floor 2");
        }
        break;
        case 4:
        W = 64;
        H = 64;
        Sprite = LevelScene::LoadSpriteFromBin("CNZ/Cork Floor.bin", SaveGame::CurrentMode);
        CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        break;
        case 5:
        W = 32;
        H = 64;
        Sprite = LevelScene::LoadSpriteFromBin("ICZ/Cork Floor.bin", SaveGame::CurrentMode);
        CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        BreakableFromTop = true;
        break;
        case 6:
        W = 64;
        H = 64;
        Sprite = LevelScene::LoadSpriteFromBin("LBZ/Cork Floor.bin", SaveGame::CurrentMode);
        CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        break;
        case 7:
        W = 32;
        H = 32;
        Sprite = LevelScene::LoadSpriteFromBin("MHZ/Cork Floor.bin", SaveGame::CurrentMode);
        CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        break;
        case 8:
        W = 32;
        H = 32;
        Sprite = LevelScene::LoadSpriteFromBin("FBZ/Cork Floor.bin", SaveGame::CurrentMode);
        CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        break;
        case 9:
        W = 32;
        H = 32;
        Sprite = LevelScene::LoadSpriteFromBin("SOZ/Cork Floor.bin", SaveGame::CurrentMode);
        CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        break;
        case 10:
        W = 32;
        H = 32;
        Sprite = LevelScene::LoadSpriteFromBin("LRZ/Cork Floor.bin", SaveGame::CurrentMode);
        CurrentAnimation = Sprite->FindAnimation("Cork Floor");
        case 11:
        case 12:
        case 13:
        case 14:
        break;
    }

    BreakableByJump = CollideSide::BOTTOM;
    if (BreakableFromTop) BreakableByJump = CollideSide::TOP;
    else BreakableBySpring = CollideSide::BOTTOM;
    BreakableByRoll = BreakableByJump;
    Scene->AddSelfToRegistry(this, "Breakable");
}

void CorkFloor::BreakAIZ(int HitSide) {
    if (Scene->ZoneID != 1) return;

    int w = 2;
    int h = 6;
    int x = X - w * 8;
    int y = Y + H / 2 - h * 16;
    int left;
    int top;
    int top2;
    if (CurrentAnimation < Sprite->Animations.size()) {
        left = Sprite->Animations[CurrentAnimation].Frames[Frame].X;
        top = Sprite->Animations[CurrentAnimation].Frames[Frame].Y;
        top2 = Sprite->Animations[CurrentAnimation].Frames[Frame].Y;
    }

    int toph = 16;
    for (int i = 0; i < w; i++)
{
        for (int j = 0; j < h; j++)
{
            toph = 16;
            top2 = top + 88 - h * 16 + j * 16;
            if (top2 < top) {
                toph = top - top2;
                top2 = top;
            }

            if (FlipX) {
                Scene->AddMovingSprite(Sprite, x + i * 16, y + j * 16 + 16 - toph, left + (w - 1 - i) * 16, top2, 16, toph, 0, 0, FlipX, false, PieceVels[(i + j * w) * 2], PieceVels[(i + j * w) * 2 + 1], 0x18, 0);
            }
            else {
                Scene->AddMovingSprite(Sprite, x + i * 16, y + j * 16 + 16 - toph, left + i * 16, top2, 16, toph, 0, 0, FlipX, false, PieceVels[(i + j * w) * 2], PieceVels[(i + j * w) * 2 + 1], 0x18, 0);
            }
        }
    }
}

void CorkFloor::Render(int CamX, int CamY) {
    if (!Solid) return;

    if (CurrentAnimation >= 0) G->DrawSprite(Sprite, CurrentAnimation, 0, X - CamX, Y - CamY, 0, IE_NOFLIP);
    else G->DrawRectangle(X - W / 2 - CamX, Y - H / 2 - CamY, W, H, 0xFF0000);
    }

int CorkFloor::OnBreakVertical(int PlayerID, int HitFrom) {
    if (PlayerID != 0) return 0;

    if (Scene->Players[PlayerID]->EZX > X + W / 2 - 2) return 0;

    if (Scene->Players[PlayerID]->EZX < X - W / 2 + 2) return 0;

    if (BreakableFromTop && HitFrom == CollideSide::BOTTOM) return 0;

    if (!BreakableFromTop && HitFrom == CollideSide::TOP) return 0;

    Sound::Play(Sound::SFX_COLLAPSE);
    BreakableByRoll = CollideSide::NONE;
    BreakableByJump = CollideSide::NONE;
    BreakableByGlide = CollideSide::NONE;
    BreakableBySpring = CollideSide::NONE;
    BreakableByKnuckles = CollideSide::NONE;
    Solid = false;
    if (Scene->ZoneID == 1) BreakAIZ(HitFrom);

    return 1;
}

