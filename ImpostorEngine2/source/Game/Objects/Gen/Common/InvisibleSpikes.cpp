// Object ID: 0x6A
// Object Name: InvisibleSpikes.cpp

#include "InvisibleSpikes.h"

typedef IMath Math;

void InvisibleSpikes::Create() {
    Object::Create();
    Active = true;
    Priority = false;
    Painful = true;
    Deadly = false;
    if (!Deadly) Solid = true;

    Scene->AddSelfToRegistry(this, "Solid");
    Rotation = 0;
    Size = 1 + (SubType & 0xF0) / 0x10;
    W = (SubType & 0xF0) + 0x10;
    H = ((SubType & 0x0F) << 4) + 0x10;
    if (Deadly) {
        W -= 2;
        H -= 2;
    }

}

int InvisibleSpikes::OnCollisionWithPlayer(int PlayerID, int HitFrom, int Data) {
    if (Painful) {
        if (HitFrom == CollideSide::TOP && Rotation == 0 && Scene->Players[PlayerID]->YSpeed >= 0) {
            Scene->Players[PlayerID]->Hurt(X, true);
        }
        else if (HitFrom == CollideSide::BOTTOM && Rotation == 180 && Scene->Players[PlayerID]->YSpeed <= 0) {
            Scene->Players[PlayerID]->Hurt(X, true);
        }
        else if ((Rotation == 90 && HitFrom == CollideSide::LEFT) || (Rotation == 270 && HitFrom == CollideSide::RIGHT)) {
            if (Scene->Players[PlayerID]->Y + 20 - 4 - 4 <= Y - 16) return 0;

            Scene->Players[PlayerID]->Hurt(X, true);
        }

    }
    else if (Deadly) {
        if (Scene->Players[PlayerID]->Y - 20 > Y + H / 2) return 0;

        Scene->Players[PlayerID]->Die(false);
    }

    return 1;
}

