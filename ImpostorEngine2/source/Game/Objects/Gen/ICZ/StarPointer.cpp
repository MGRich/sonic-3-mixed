// Object ID: 0xAE
// Object Name: StarPointer.cpp

#include "StarPointer.h"

typedef IMath Math;

void StarPointer::Create() {
    Enemy::Create();
    Active = true;
    Priority = false;
    W = 32;
    H = 32;
    CurrentAnimation = Sprite->FindAnimation("Star Pointer");
    for (int i = 0; i < 4; i++)
{
        Orbs[i] = Scene->AddNewObject(Obj_StarPointerOrbs, i, X, Y, FlipX, false);
        Orbs[i]->CurrentAnimation = CurrentAnimation;
        OrbOffsets[i] = i * 90;
        IApp::Print(0, "%d %d", OrbOffsets[i], i);
    }
    RotAngle = 0;
    TurnSpeed = 0.01;
}

void StarPointer::Update() {
    for (int i = 0; i < 4; i++)
{
        RotAngle = 0;
        RotAngle += OrbOffsets[i];
        RotAngle += TurnSpeed;
        Orbs[i]->X = sin(RotAngle) * 20 + X;
        Orbs[i]->Y = cos(RotAngle) * 20 + Y;
        OrbOffsets[i] = RotAngle;
    }
    X -= 0.01;
    Object::Update();
}

void StarPointer::Render(int CamX, int CamY) {
    if (DrawCollisions) {
        G->DrawRectangle(X - CamX, Y - CamY, W, H, DrawCollisionsColor);
    }
    else {
        G->DrawSprite(this->Sprite, CurrentAnimation, Frame >> 8, this->X - CamX, this->Y - CamY, 0, this->FlipX ? IE_FLIPX : IE_NOFLIP);
    }
    }

