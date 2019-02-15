#if INTERFACE
#include <Utils/Standard.h>

#include <Engine/IScene.h>
#include <Engine/IResources.h>
#include <Engine/IStreamer.h>
#include <Engine/IGraphics.h>
#include <Engine/IModel.h>
#include <Engine/ISound.h>
#include <Engine/IAudio.h>

#include <Game/LevelSceneExt.h>
#include <Game/Object.h>
#include <Game/Player.h>
#include <Game/ObjectNames.h>

#include <Game/Explosion.h>

class LevelScene : public IScene {
public:
    SceneData*  Data = NULL;
    ISprite*    TileSprite = NULL;
    ISprite*    AnimTileSprite = NULL;
    int         FGLayer = 0;

    int         RingAnimationFrame = 0;
    int         WaterAnimationFrame = 0;

    int32_t     CameraX = 0;
    int32_t     CameraY = 0;
    int32_t     CameraMinX = 0;
    int32_t     CameraMinY = 0;
    int32_t     CameraMaxX = 0xFFFF;
    int32_t     CameraMaxY = 0xFFFF;
    int32_t     CameraDeltaX = 0;
    int32_t     CameraDeltaY = 0;
    int         CameraAutoScrollX = 0;
    int         CameraAutoScrollY = 0;
    int         Frame = 0;
    int         Timer = 0;
    int         Score = 0;

    int         WaterLevel = 0xFFFF;
    int         VisualWaterLevel = 0xFFFF;

    int         PauseAnim[8];
    int         PauseSelectedMenuItem = 0;
    bool        Paused = false;
    bool        PauseFinished = false;

    ISprite*    PauseSprite = NULL;
    ISprite*    GlobalDisplaySprite = NULL;
    ISprite*    MobileButtonsSprite = NULL;
    ISprite*    ItemsSprite = NULL;
    ISprite*    AnimalsSprite = NULL;
    ISprite*    ObjectsSprite = NULL;
    ISprite*    Objects2Sprite = NULL;
    ISprite*    Objects3Sprite = NULL;
    ISprite*    RobotnikSprite = NULL;
    ISprite*    ExplosionSprite = NULL;
    ISprite*    WaterSprite = NULL;
    ISprite*    KnuxSprite[6];

    IPlayer*    Player = NULL;
    IPlayer*    Players[7];
    int         PlayerCount = 1;
    PlaneSwitch*PlaneSwitchers = NULL;
    int         PlaneSwitchCount = 0;

    int         PlayerStartX = -1;
    int         PlayerStartY = -1;

    int         RoutineNumber = 0;
    int         LevelTriggerFlag = 0;

    int         FadeAction = 0;
    enum        FadeActionType {
        FADEIN = 1,
        RESTART = 2,
        EXIT = 3,
        DIED = 4,
        TO_BONUS_STAGE1 = 5,
        TO_BONUS_STAGE2 = 6,
        TO_SPECIAL_STAGE = 7,
        NEXT_ZONE = 8,
        CUSTOM_FADE_ACTION = 9,
    };

    int         maxLayer = 1;
    bool        Thremixed = false;

    Object**    Objects;
    int         ObjectCount = 0;
    int         ObjectNewCount = 0;

    Object**    ObjectsSolid;
    int         ObjectSolidCount = 0;

    Object**    ObjectsSpring;
    int         ObjectSpringCount = 0;

    Enemy**     ObjectsEnemies;
    int         ObjectEnemiesCount = 0;

    Object**    ObjectsBreakable;
    int         ObjectBreakableCount = 0;

    Object**    ObjectsPathSwitcher;
    int         ObjectPathSwitcherCount = 0;

	unordered_map<string, ISprite*> SpriteMap;
    ISprite**   SpriteMapIDs;

    int         ZoneID = 0;
    int         Act = 1;
    int         VisualAct = 1;
    bool        CanWaterRun = true;

    int         DEBUG_MOUSE_X = -1;
    int         DEBUG_MOUSE_Y = -1;

    vector<Object*> Explosions;

    struct ObjectProp {
        uint16_t X = 0x00;
        int16_t Y = 0x00;
        uint8_t ID = 0x00;
        uint8_t SubType = 0x00;
        bool LoadFlag = false;
        bool FlipX = false;
        bool FlipY = false;
    };

    ObjectProp* ObjectProps = NULL;
    int         ObjectPropCount = 0;

    ObjectProp* RingProps = NULL;
    int         RingPropCount = 0;

    ObjectProp* AnimatedSprite0Props = NULL;
    int         AnimatedSprite0PropCount = 0;

    ObjectProp* AnimatedSprite1Props = NULL;
    int         AnimatedSprite1PropCount = 0;

    double      LevelCardTimer = 0.0;
    bool        LevelCardHide = false;

    char        LevelName[20];
    char        LevelNameDiscord[30];
    bool        SKAlone = false;

    bool        HUDVisible = true;
    int         HUDAnim = 0x00;
    bool        ControlsVisible = true;
    int         ControlsAnim = 0x00;

    bool        ManiaLevel = false;
    bool        SonicKnucklesLevel = false;

    const char* Str_TileConfigBin = NULL;
    const char* Str_SceneBin = NULL;
    const char* Str_TileSprite = NULL;
    const char* Str_StageBin = NULL;
    const char* Str_AnimatedSprites = NULL;
    const char* Str_ObjectsList = NULL;
    const char* Str_RingsList = NULL;

    uint8_t     CharacterFlag = 0;

    int         BackgroundRepeatTileX = 0;
    int         BackgroundRepeatTileWidth = 0;

    IModel*     GiantRingModel = NULL;

    int         Checkpoint = -1;
    int         SavedPositionX = -1;
    int         SavedPositionY = -1;
	uint32_t*   SavedPalette = NULL;

    int         WaterEnteredCounter = 0;

    ISound**    SoundBank = NULL;

    uint8_t*    PerspectiveMap;

    int         ShakeTimer = 0;

    bool        ResetTimer = true;
    bool        StopTimer = false;
    bool        ShowResults = false;
    bool        DoneSpinning = false;
    int         ResultsTimer = 0;
    int         TimerTotal = 0;
    int         TotalToAdd = 0;

    int         SpecialSpawnPositionX = -1;
    int         SpecialSpawnPositionY = -1;

    bool        CollisionCheckForClimbable = false;
    bool        CollisionCheckForAlternate = false;
    bool        CollisionCheckForAlternate2 = false;

    uint32_t    BackgroundColor = 0x000000;
    bool        SepThread = false;
    uint16_t    Signal[8]; //
};
#endif

#include <Engine/IInput.h>

#include <Game/Ring.h>
#include <Game/FallingTile.h>
#include <Game/MovingSprite.h>
#include <Game/ScoreGhost.h>
#include <Game/LevelScene.h>
#include <Game/Animal.h>
#include <Game/Sound.h>

#include <Game/Objects/Gen/ObjectListing.h>
#include <Game/Object.h>

#include <Game/Scenes/DataSelect.h>
#include <Game/Scenes/LevelSelect.h>

#include <Game/SaveGame.h>

#define ADD_OBJECT() ObjectProp op; op.X = X; op.Y = Y; op.ID = ID; op.SubType = SubType; op.LoadFlag = PRIORITY; op.FlipX = FLIPX; op.FlipY = FLIPY; ObjectProps[ObjectPropCount++] = op; Object* obj = GetNewObjectFromID(ID); if (obj) { obj->G = G; obj->App = App; obj->Scene = this; obj->InitialX = X; obj->InitialY = Y; obj->FlipX = FLIPX == 1; obj->FlipY = FLIPY == 1; obj->ID = ID; while (!SpriteMapIDs[ID]) ID--; obj->Sprite = SpriteMapIDs[ID]; obj->SubType = SubType; Objects[ObjectCount++] = obj; }

bool ViewPalettes = false;
bool ViewPathswitchers = false;
bool ViewPlayerStats = false;
bool ViewPlayerUpdateStats = false;
bool ViewTileInfo = false;
bool ViewTileCollision = false;
const char* ObjectName[347];

int BlankTile = 0;

PUBLIC LevelScene::LevelScene(IApp* app, IGraphics* g) {
    App = app;
    G = g;

    App->Settings->GetBool("dev", "viewPalettes", &ViewPalettes);
    App->Settings->GetBool("dev", "viewPathswitchers", &ViewPathswitchers);
    App->Settings->GetBool("dev", "viewPlayerStats", &ViewPlayerStats);
	App->Settings->GetBool("dev", "viewPlayerUpdateStats", &ViewPlayerUpdateStats);
    App->Settings->GetBool("dev", "viewTileInfo", &ViewTileInfo);
    App->Settings->GetBool("dev", "viewTileCollision", &ViewTileCollision);

    uint64_t startTime = SDL_GetTicks();

    Sound::Audio = App->Audio;
    Sound::Init();
	SaveGame::Init();

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Sound::Init()", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

    memset(Signal, 0, sizeof(Signal));

    Objects = (Object**)calloc(2000, sizeof(Object*));
    ObjectsSolid = (Object**)calloc(1000, sizeof(Object*));
    ObjectsSpring = (Object**)calloc(100, sizeof(Object*));
    ObjectsEnemies = (Enemy**)calloc(300, sizeof(Enemy*));
    ObjectsBreakable = (Object**)calloc(100, sizeof(Object*));
    ObjectsPathSwitcher = (Object**)calloc(100, sizeof(Object*));

    ObjectProps = (ObjectProp*)calloc(0x400, sizeof(ObjectProp));
    RingProps = (ObjectProp*)calloc(0x400, sizeof(ObjectProp));
    AnimatedSprite0Props = (ObjectProp*)calloc(0x100, sizeof(ObjectProp));
    AnimatedSprite1Props = (ObjectProp*)calloc(0x100, sizeof(ObjectProp));

	SavedPalette = (uint32_t*)calloc(0x100, sizeof(uint32_t));

    SoundBank = (ISound**)calloc(0x100, sizeof(ISound*));

    SpriteMapIDs = (ISprite**)calloc(0x400, sizeof(ISprite*));

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Memory Allocation", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

    /*
    GlobalDisplaySprite = new ISprite("Sprites/Global/Display.gif", App);
    GlobalDisplaySprite->LoadAnimation("Sprites/Global/HUD.bin");
    GlobalDisplaySprite->LoadAnimation("Sprites/Global/TitleCard.bin");
    GlobalDisplaySprite->LoadAnimation("Sprites/Global/ScoreBonus.bin");

    MobileButtonsSprite = new ISprite("UI/Mobile Buttons.gif", App);
    ISprite::Animation an;
    an.Name = NULL;
    an.FrameCount = 8;
    an.Frames = (ISprite::AnimFrame*)calloc(8, sizeof(ISprite::AnimFrame));
    for (int i = 0; i < 8; i++) {
        ISprite::AnimFrame ts_af;
        ts_af.X = i * 64;
        ts_af.Y = 0;
        ts_af.W = ts_af.H = 64;
        ts_af.OffX = ts_af.OffY = -32;
        an.Frames[i] = ts_af;
        G->MakeFrameBufferID(MobileButtonsSprite, an.Frames + i);
    }
    MobileButtonsSprite->Animations.push_back(an);
    MobileButtonsSprite->SetTransparentColorIndex(0x05);
    MobileButtonsSprite->UpdatePalette();

    PauseSprite = new ISprite("UI/PauseEN.gif", App);
    PauseSprite->LoadAnimation("UI/TextEN.bin");
    //*/

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Creating GlobalDisplaySprite...", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

    memset(PauseAnim, 0, sizeof(PauseAnim));

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Creating PauseSprite...", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

    memset(KnuxSprite, 0, sizeof(KnuxSprite));

    VisualAct = Act;
}

PUBLIC VIRTUAL void LevelScene::AssignSpriteMapIDs() {
	SpriteMapIDs[0x01] = ItemsSprite;
    SpriteMapIDs[0x07] = ObjectsSprite;
    SpriteMapIDs[0x08] = ObjectsSprite;
    SpriteMapIDs[0x33] = ObjectsSprite;
    SpriteMapIDs[0x34] = ObjectsSprite;
    SpriteMapIDs[0x81] = ObjectsSprite;
}

PUBLIC VIRTUAL void LevelScene::LoadZoneSpecificSprites() {

}

ISprite* GlobalDisplaySpriteS3K = NULL;
Object* LastObjectUpdated = NULL;

PUBLIC VIRTUAL void LevelScene::LoadData() {
    /// Init
    bool AlreadyLoaded = true;
    uint64_t startTime = SDL_GetTicks();
    if (!Data) {
        AlreadyLoaded = false;
        memset(SpriteMapIDs, 0, 0x100);

        FadeAction = FadeActionType::FADEIN;
        FadeTimerMax = 90;
        FadeMax = 0x140;
        G->FadeToWhite = false;

        startTime = SDL_GetTicks();

        // Test Loading Models
        if (!GiantRingModel)
            GiantRingModel = new IModel("Meshes/SpecialRing.bin", G);

        IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "GiantRingModel", (SDL_GetTicks() - startTime) / 1000.0);
        startTime = SDL_GetTicks();

        if (!PauseSprite) {
            PauseSprite = new ISprite("UI/PauseEN.gif", App);
            PauseSprite->LoadAnimation("UI/TextEN.bin");
        }
		if (!GlobalDisplaySprite) {
			GlobalDisplaySprite = new ISprite("Sprites/Global/Display.gif", App);
			GlobalDisplaySprite->LoadAnimation("Sprites/Global/HUD.bin");
			GlobalDisplaySprite->LoadAnimation("Sprites/Global/TitleCard.bin");
		}
		if (!GlobalDisplaySpriteS3K) {
			GlobalDisplaySpriteS3K = new ISprite("Sprites/GlobalS3K/Display.gif", App);
			GlobalDisplaySpriteS3K->LoadAnimation("Sprites/GlobalS3K/HUD.bin");
		}
        if (!MobileButtonsSprite) {
            MobileButtonsSprite = new ISprite("UI/Mobile Buttons.gif", App);
            ISprite::Animation an;
            an.Name = NULL;
            an.FrameCount = 8;
            an.Frames = (ISprite::AnimFrame*)calloc(8, sizeof(ISprite::AnimFrame));
            for (int i = 0; i < 8; i++) {
                ISprite::AnimFrame ts_af;
                ts_af.X = i * 64;
                ts_af.Y = 0;
                ts_af.W = ts_af.H = 64;
                ts_af.OffX = ts_af.OffY = -32;
                an.Frames[i] = ts_af;
                G->MakeFrameBufferID(MobileButtonsSprite, an.Frames + i);
            }
            MobileButtonsSprite->Animations.push_back(an);
            MobileButtonsSprite->SetTransparentColorIndex(0x05);
            MobileButtonsSprite->UpdatePalette();
        }
        if (!ItemsSprite) {
			if (Thremixed) {
				ItemsSprite = new ISprite("Sprites/Global/Items.gif", App);
				ItemsSprite->LoadAnimation("Sprites/Global/ItemBox.bin");
				ItemsSprite->LoadAnimation("Sprites/Global/Ring.bin");
			}
			else {
				ItemsSprite = new ISprite("Sprites/GlobalS3K/Items.gif", App);
				ItemsSprite->LoadAnimation("Sprites/GlobalS3K/ItemBox.bin");
				ItemsSprite->LoadAnimation("Sprites/GlobalS3K/Ring.bin");
			}
        }
        if (!AnimalsSprite) {
            AnimalsSprite = new ISprite("Sprites/Global/Animals.gif", App);
            AnimalsSprite->LoadAnimation("Sprites/Global/Animals.bin");
        }
        if (!ObjectsSprite) {
			if (Thremixed) {
				ObjectsSprite = new ISprite("Sprites/Global/Objects.gif", App);
				ObjectsSprite->LoadAnimation("Sprites/Global/Springs.bin");
				ObjectsSprite->LoadAnimation("Sprites/Global/Spikes.bin");
				ObjectsSprite->LoadAnimation("Sprites/Global/StarPost.bin");
				ObjectsSprite->LoadAnimation("Sprites/Global/ScoreBonus.bin");
			}
			else {
				ObjectsSprite = new ISprite("Sprites/GlobalS3K/Objects.gif", App);
				ObjectsSprite->LoadAnimation("Sprites/GlobalS3K/Springs.bin");
				ObjectsSprite->LoadAnimation("Sprites/GlobalS3K/Spikes.bin");
				ObjectsSprite->LoadAnimation("Sprites/GlobalS3K/StarPost.bin");
				ObjectsSprite->LoadAnimation("Sprites/GlobalS3K/ScoreBonus.bin");

				ObjectsSprite->LoadAnimation("Sprites/GlobalS3K/Gray Button.bin");
				ObjectsSprite->LoadAnimation("Sprites/GlobalS3K/EggPrison.bin");
			}
            // printf("\n");
        }
        if (!Objects2Sprite) {
            Objects2Sprite = new ISprite("Sprites/Global/Objects2.gif", App);
            Objects2Sprite->LoadAnimation("Sprites/Global/SignPost.bin");
            // printf("\n");
        }
        if (!Objects3Sprite) {
            Objects3Sprite = new ISprite("Sprites/Global/Objects3.gif", App);
            Objects3Sprite->LoadAnimation("Sprites/Global/SpecialRing.bin");
            Objects3Sprite->LoadAnimation("Sprites/Global/SuperSparkle.bin");
            Objects3Sprite->LoadAnimation("Sprites/Global/Shields.bin");
            // printf("\n");
        }
        if (!RobotnikSprite) {
            RobotnikSprite = new ISprite("Sprites/GlobalS3K/Robotnik.gif", App);
            RobotnikSprite->LoadAnimation("Sprites/GlobalS3K/EggMobile.bin");
            RobotnikSprite->LoadAnimation("Sprites/GlobalS3K/Crane.bin");
            // printf("\n");
        }
        if (!ExplosionSprite) {
			if (Thremixed) {
				ExplosionSprite = new ISprite("Sprites/Global/Explosions.gif", App);
				ExplosionSprite->LoadAnimation("Sprites/Global/Dust.bin");
				ExplosionSprite->LoadAnimation("Sprites/Global/Explosions.bin");
			}
			else {
				ExplosionSprite = new ISprite("Sprites/GlobalS3K/Explosions.gif", App);
				ExplosionSprite->LoadAnimation("Sprites/GlobalS3K/Dust.bin");
				ExplosionSprite->LoadAnimation("Sprites/GlobalS3K/Explosions.bin");
			}
            // printf("\n");
        }
        if (!WaterSprite) {
            WaterSprite = new ISprite("Sprites/Global/Water.gif", App);
            WaterSprite->Print = true;
            WaterSprite->LoadAnimation("Sprites/Global/Water.bin");
        }

        IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Common Sprites", (SDL_GetTicks() - startTime) / 1000.0);
        startTime = SDL_GetTicks();

        LoadZoneSpecificSprites();
        IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "LoadZoneSpecificSprites", (SDL_GetTicks() - startTime) / 1000.0);
        startTime = SDL_GetTicks();

        AssignSpriteMapIDs();
        IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "AssignSpriteMapIDs", (SDL_GetTicks() - startTime) / 1000.0);
        startTime = SDL_GetTicks();

        // Instead of filling it on a per by per basis, Fill the whole thing and
        // have the replacements fill the spots needed.
        for (int i = 0x0; i <= 346; i++) {
            ObjectName[i] = "Unused";
        }

        // If Sonic 3 level...
        if (ZoneID < 7) {
            ObjectName[0x00] = "Obj_Ring";
            ObjectName[0x01] = "Obj_Monitor";
            ObjectName[0x02] = "Obj_PathSwap";
            ObjectName[0x03] = "Obj_HollowTree";
            ObjectName[0x04] = "Obj_CollapsingPlatform";
            ObjectName[0x05] = "Obj_Rock";
            ObjectName[0x06] = "Obj_RideVine";
            ObjectName[0x07] = "Obj_Spring";
            ObjectName[0x08] = "Obj_Spikes";
            ObjectName[0x09] = "Obj_AIZTreeBark";
            ObjectName[0x0A] = "Obj_RopePeg";
            ObjectName[0x0B] = "Obj_Ring";
            ObjectName[0x0C] = "Obj_SwingVine";
            ObjectName[0x0D] = "Obj_BreakableWall";
            ObjectName[0x0E] = "Obj_Ridge";
            ObjectName[0x0F] = "Obj_CollapsingBridge";
            ObjectName[0x10] = "Obj_LBZ_TubeElevator";
            ObjectName[0x11] = "Obj_LBZ_MovingPlatform";
            ObjectName[0x12] = "Obj_LBZ_FlagPlatform"; // Unused
            ObjectName[0x13] = "Obj_LBZ_ExplodingTrigger";
            ObjectName[0x14] = "Obj_LBZ_TriggerBridge";
            ObjectName[0x15] = "Obj_LBZ_PlayerLauncher";
            ObjectName[0x16] = "Obj_LBZ_FlameThrower";
            ObjectName[0x17] = "Obj_LBZ_RideGrapple";
            ObjectName[0x18] = "Obj_LBZ_CupElevator";
            ObjectName[0x19] = "Obj_LBZ_CupElevatorPole";
            ObjectName[0x1A] = "Obj_P2RotateObject"; // Purpose unknown.
            ObjectName[0x1B] = "Obj_LBZ_PipePlug";
            ObjectName[0x1C] = "Obj_Ring";
            ObjectName[0x1D] = "Obj_LBZ_HangPlatform";
            ObjectName[0x1E] = "Obj_LBZ_SpinLauncher";
            ObjectName[0x1F] = "Obj_LBZ_LoweringGrapple";
            ObjectName[0x20] = "Obj_MGZ_LBZSmashingPillar";
            ObjectName[0x21] = "Obj_LBZ_GateLaser";
            ObjectName[0x22] = "Obj_LBZ_Siren";
            ObjectName[0x23] = "Obj_ContactFirceFallAnimator"; // Unused
            ObjectName[0x24] = "Obj_AutomaticTunnel";
            ObjectName[0x25] = "Obj_Ring";
            ObjectName[0x26] = "Obj_RollEnforcer";
            ObjectName[0x27] = "Obj_LavaCollisionMarker"; // Unused
            ObjectName[0x28] = "Obj_InvisibleBarrier";
            ObjectName[0x29] = "Obj_AIZ_DisappearingFloor";
            ObjectName[0x2A] = "Obj_CorkFloor";
            ObjectName[0x2B] = "Obj_AIZ_FlippingBridge";
            ObjectName[0x2C] = "Obj_AIZ_CollapsingLogBridge";
            ObjectName[0x2D] = "Obj_AIZ_FallingLog";
            ObjectName[0x2E] = "Obj_AIZ_SpikedLog";
            ObjectName[0x2F] = "Obj_StillSprite";
            ObjectName[0x30] = "Obj_AnimatedStillSprite";
            ObjectName[0x31] = "Obj_LRZ_RotatingCylinders";
            ObjectName[0x32] = "Obj_AIZ_DrawBridge";
            ObjectName[0x33] = "Obj_Button";
            ObjectName[0x34] = "Obj_StarPost";
            ObjectName[0x35] = "Obj_AIZ_ForegroundPlant";
            ObjectName[0x36] = "Obj_BreakableBar";
            ObjectName[0x37] = "Obj_HCZ_VWaterStream";
            ObjectName[0x38] = "Obj_HCZ_CGZ_Fan";
            ObjectName[0x39] = "Obj_HCZ_LargeFan";
            ObjectName[0x3A] = "Obj_HCZ_HandLauncher";
            ObjectName[0x3B] = "Obj_HCZ_WaterWall";
            ObjectName[0x3C] = "Obj_Door";
            ObjectName[0x3D] = "Obj_RetractingSpring";
            ObjectName[0x3E] = "Obj_HCZ_ConveyorBelt";
            ObjectName[0x3F] = "Obj_HCZ_ConveyorSpike";
            ObjectName[0x40] = "Obj_HCZ_Block";
            ObjectName[0x41] = "Obj_CNZ_Balloon";
            ObjectName[0x42] = "Obj_CNZ_Cannon";
            ObjectName[0x43] = "Obj_CNZ_RisingPlatform";
            ObjectName[0x44] = "Obj_CNZ_TrapDoor";
            ObjectName[0x45] = "Obj_CNZ_LightBulb";
            ObjectName[0x46] = "Obj_CNZ_HoverFan";
            ObjectName[0x47] = "Obj_CNZ_RotatingBarrel";
            ObjectName[0x48] = "Obj_VacuumTube";
            ObjectName[0x49] = "Obj_CNZ_RotatingWheel";
            ObjectName[0x4A] = "Obj_CNZ_Bumper";
            ObjectName[0x4B] = "Obj_CNZ_BouncepadSprings";
            ObjectName[0x4C] = "Obj_CNZ_Corkscrew";
            ObjectName[0x4D] = "Obj_CNZ_BarberPoleSprite";
            ObjectName[0x4E] = "Obj_CNZ_RotatingMesh";
            ObjectName[0x4F] = "Obj_MGZ_QuickMud";
            ObjectName[0x50] = "Obj_MGZ_RunAroundPole";
            ObjectName[0x51] = "Obj_FloatingPlatform";
            ObjectName[0x52] = "Obj_MGZ_LBZSmashingPillar";
            ObjectName[0x53] = "Obj_MGZ_SwingingPlatform";
            ObjectName[0x54] = "Obj_Bubbler";
            ObjectName[0x55] = "Obj_MGZ_HeadTrigger";
            ObjectName[0x56] = "Obj_MGZ_MovingSpikePlatform";
            ObjectName[0x57] = "Obj_MGZ_TriggerPlatform";
            ObjectName[0x58] = "Obj_MGZ_SwingingSpikeBall";
            ObjectName[0x59] = "Obj_MGZ_DashTrigger";
            ObjectName[0x5A] = "Obj_MGZ_Pulley";
            ObjectName[0x5B] = "Obj_MGZ_TopPlatform";
            ObjectName[0x5C] = "Obj_MGZ_TopLauncher";
            ObjectName[0x5D] = "Obj_CGZ_TriangularSprings";
            ObjectName[0x5E] = "Obj_CGZ_BladePlatform";
            ObjectName[0x5F] = "Obj_RetractingSpring2P";
            ObjectName[0x60] = "Obj_BPZ_ElephantBlock";
            ObjectName[0x61] = "Obj_BPZ_Balloon";
            ObjectName[0x62] = "Obj_DPZ_DisolvingSandBar";
            ObjectName[0x63] = "Obj_DPZ_Button";
            ObjectName[0x64] = "Obj_Item2P";
            ObjectName[0x65] = "Obj_GoalMarker2P";
            ObjectName[0x66] = "Obj_EMZ_Dripper";
            ObjectName[0x67] = "Obj_HCZ_SnakeBlocks";
            ObjectName[0x68] = "Obj_HCZ_SpinningColumn";
            ObjectName[0x69] = "Obj_HCZ_Slide";
            ObjectName[0x6A] = "Obj_InvisibleHurtBlockHorizontal";
            ObjectName[0x6B] = "Obj_InvisibleHurtBlockVertical";
            ObjectName[0x6C] = "Obj_TensionBridge";
            ObjectName[0x6D] = "Obj_HCZ_WaterSplash";
            ObjectName[0x6E] = "Obj_WaterDrop";
            ObjectName[0x6F] = "Obj_FBZ_RotatingMesh";
            ObjectName[0x70] = "Obj_FBZ_TwistingCylinders";
            ObjectName[0x71] = "Obj_FBZ_FloatingPlatform";
            ObjectName[0x72] = "Obj_FBZ_ChainLink";
            ObjectName[0x73] = "Obj_FBZ_MagneticSpikeBall";
            ObjectName[0x74] = "Obj_FBZ_MagneticPlatform";
            ObjectName[0x75] = "Obj_FBZ_SnakePlatform";
            ObjectName[0x76] = "Obj_FBZ_BentPipe";
            ObjectName[0x77] = "Obj_FBZ_RotatingPlatform";
            ObjectName[0x78] = "Obj_FBZ_DEZPlayerLauncher";
            ObjectName[0x79] = "Obj_FBZ_DisappearingPlatform";
            ObjectName[0x7A] = "Obj_FBZ_ScrewDoor";
            ObjectName[0x7B] = "Obj_FBZ_FanPole";
            ObjectName[0x7C] = "Obj_FBZ_Propeller";
            ObjectName[0x7D] = "Obj_FBZ_Piston";
            ObjectName[0x7E] = "Obj_FBZ_PlatformBlocks";
            ObjectName[0x7F] = "Obj_FBZ_MissileLauncher";
            ObjectName[0x80] = "Obj_HiddenMonitor";
            ObjectName[0x81] = "Obj_EggCapsule";
            ObjectName[0x82] = "Obj_CutsceneKnuckles";
            ObjectName[0x83] = "Obj_CutsceneButton";
            ObjectName[0x84] = "Obj_AIZ_CutsceneSonic";
            ObjectName[0x85] = "Obj_SpecialRing";
            ObjectName[0x86] = "Obj_SS_GumballLevel";
            ObjectName[0x87] = "Obj_SS_GumbalSideSpring";
            ObjectName[0x88] = "Obj_CNZ_BreakableWaterRisingFloor";
            ObjectName[0x89] = "Obj_CNZ_LightSwitch";
            ObjectName[0x8A] = "Obj_FBZ2_CutsceneObjects";
            ObjectName[0x8B] = "Obj_SpriteMasker";
            ObjectName[0x8C] = "Obj_Tulipon";
            ObjectName[0x8D] = "Obj_RhinoBot";
            ObjectName[0x8E] = "Obj_MonkeyDude";
            ObjectName[0x8F] = "Obj_CaterKillerJr";
            ObjectName[0x90] = "Obj_AIZ_Miniboss_Cutscene";
            ObjectName[0x91] = "Obj_AIZ_Miniboss";
            ObjectName[0x92] = "Obj_AIZ_EndBoss";
            ObjectName[0x93] = "Obj_Jawz";
            ObjectName[0x94] = "Obj_Blastoid";
            ObjectName[0x95] = "Obj_Buggernaut";
            ObjectName[0x96] = "Obj_TurboSpiker";
            ObjectName[0x97] = "Obj_MegaChomper";
            ObjectName[0x98] = "Obj_Poindexter";
            ObjectName[0x99] = "Obj_HCZ_Miniboss";
            ObjectName[0x9A] = "Obj_HCZ_EndBoss";
            ObjectName[0x9B] = "Obj_BubblesBadnik";
            ObjectName[0x9C] = "Obj_Spiker";
            ObjectName[0x9D] = "Obj_Mantis";
            ObjectName[0x9E] = "Obj_MGZ_CutsceneMiniBoss";
            ObjectName[0x9F] = "Obj_MGZ_MiniBoss";
            ObjectName[0xA0] = "Obj_MGZ2_DrillingEggman";
            ObjectName[0xA1] = "Obj_MGZ_Boss";
            ObjectName[0xA2] = "Obj_MGZ_KnucklesBoss";
            ObjectName[0xA3] = "Obj_Clamer";
            ObjectName[0xA4] = "Obj_CNZ_Blastoid";
            ObjectName[0xA5] = "Obj_Batbright";
            ObjectName[0xA6] = "Obj_CNZ_Miniboss";
            ObjectName[0xA7] = "Obj_CNZ_EndBoss";
            ObjectName[0xA8] = "Obj_Blaster";
            ObjectName[0xA9] = "Obj_TechnoSqueek";
            ObjectName[0xAA] = "Obj_FBZ1_Boss";
            ObjectName[0xAB] = "Obj_FBZ_Laser_Boss";
            ObjectName[0xAC] = "Obj_FBZ_EndBoss";
            ObjectName[0xAD] = "Obj_Penguinator";
            ObjectName[0xAE] = "Obj_StarPointer";
            ObjectName[0xAF] = "Obj_ICZ_CrushingColumn";
            ObjectName[0xB0] = "Obj_ICZ_PathFollowPlatform";
            ObjectName[0xB1] = "Obj_ICZ_BreakableWall";
            ObjectName[0xB2] = "Obj_ICZ_Freezer";
            ObjectName[0xB3] = "Obj_ICZ_SegmentColumn";
            ObjectName[0xB4] = "Obj_ICZ_SwingingPlatform";
            ObjectName[0xB5] = "Obj_ICZ_Stalagtite";
            ObjectName[0xB6] = "Obj_ICZ_IceCube";
            ObjectName[0xB7] = "Obj_ICZ_IceSpikes";
            ObjectName[0xB8] = "Obj_ICZ_HarmfulIce";
            ObjectName[0xB9] = "Obj_ICZ_SnowPile";
            ObjectName[0xBA] = "Obj_ICZ_TensionPlatform";
            ObjectName[0xBB] = "Obj_ICZ_RightTrampolineSupport";
            ObjectName[0xBC] = "Obj_ICZ_Miniboss";
            ObjectName[0xBD] = "Obj_ICZ_EndBoss";
            ObjectName[0xBE] = "Obj_SnaleBlaster";
            ObjectName[0xBF] = "Obj_Ribot";
            ObjectName[0xC0] = "Obj_Orbinaut";
            ObjectName[0xC1] = "Obj_Corkey";
            ObjectName[0xC2] = "Obj_Flybot767";
            ObjectName[0xC3] = "Obj_LRZ1_Eggman";
            ObjectName[0xC4] = "Obj_LRZ1_MiniBoss";
            ObjectName[0xC5] = "Obj_LRZ1_KnucklesMiniBossBox";
            ObjectName[0xC6] = "Obj_LRZ2_EggMobile";
            ObjectName[0xC7] = "Obj_Ring";
            ObjectName[0xC8] = "Obj_DeathEgg_LaunchTowers";
            ObjectName[0xC9] = "Obj_LRZ_Miniboss";
            ObjectName[0xCA] = "Obj_LRZFinalBoss1";
            ObjectName[0xCB] = "Obj_LRZEndBoss";
            ObjectName[0xCC] = "Obj_LRZFinalBoss2";
            ObjectName[0xCD] = "Obj_LRZ2_TallBoss";
            ObjectName[0xCE] = "Obj_FBZ_ExitDoor";
            ObjectName[0xCF] = "Obj_FBZ_EggPrison";
            ObjectName[0xD0] = "Obj_FBZ_SpringPlunger";
            ObjectName[0xD1] = "Obj_Ring";
            ObjectName[0xD2] = "Obj_Ring";
            ObjectName[0xD3] = "Obj_Ring";
            ObjectName[0xD4] = "Obj_Ring";
            ObjectName[0xD5] = "Obj_Ring";
            ObjectName[0xD6] = "Obj_Ring";
            ObjectName[0xD7] = "Obj_Ring";
            ObjectName[0xD8] = "Obj_Ring";
            ObjectName[0xD9] = "Obj_Ring";
            ObjectName[0xDA] = "Obj_Ring";
            ObjectName[0xDB] = "Obj_Ring";
            ObjectName[0xDC] = "Obj_Ring";
            ObjectName[0xDD] = "Obj_Ring";
            ObjectName[0xDE] = "Obj_Ring";
            ObjectName[0xDF] = "Obj_Ring";
            ObjectName[0xE0] = "Obj_FBZ_WallMissile";
            ObjectName[0xE1] = "Obj_FBZ_Mine";
            ObjectName[0xE2] = "Obj_FBZ_Elevator";
            ObjectName[0xE3] = "Obj_FBZ_TrapSpring";
            ObjectName[0xE4] = "Obj_FBZ_Flamethrower";
            ObjectName[0xE5] = "Obj_FBZ_SpiderCrane";
            ObjectName[0xE6] = "Obj_Pachinko_TriangleBumper";
            ObjectName[0xE7] = "Obj_Pachinko_Flipper";
            ObjectName[0xE8] = "Obj_Pachinko_EnergyTrap";
            ObjectName[0xE9] = "Obj_Pachinko_InvisibleUnknown";
            ObjectName[0xEA] = "Obj_Pachinko_Platform";
            ObjectName[0xEB] = "Obj_GumballItem";
            ObjectName[0xEC] = "Obj_MagneticOrb";
            ObjectName[0xED] = "Obj_Pachinko_ItemOrb";
            ObjectName[0xEE] = "Obj_Ring";
            ObjectName[0xEF] = "Obj_Ring";
            ObjectName[0xF0] = "Obj_Ring";
            ObjectName[0xF1] = "Obj_Ring";
            ObjectName[0xF2] = "Obj_Ring";
            ObjectName[0xF3] = "Obj_Ring";
            ObjectName[0xF4] = "Obj_Ring";
            ObjectName[0xF5] = "Obj_Ring";
            ObjectName[0xF6] = "Obj_Ring";
            ObjectName[0xF7] = "Obj_Ring";
            ObjectName[0xF8] = "Obj_Ring";
            ObjectName[0xF9] = "Obj_Ring";
            ObjectName[0xFA] = "Obj_Ring";
            ObjectName[0xFB] = "Obj_Ring";
            ObjectName[0xFC] = "Obj_Ring";
            ObjectName[0xFD] = "Obj_Ring";
            ObjectName[0xFE] = "Obj_Ring";
			ObjectName[0xFF] = "Obj_FBZ_MagneticPendulum";
        }
        // If Sonic & Knuckles level...
        else if (ZoneID < 13) {
            ObjectName[0x00] = "Obj_Ring";
            ObjectName[0x01] = "Obj_Monitor";
            ObjectName[0x02] = "Obj_PathSwap";
            ObjectName[0x03] = "Obj_MGZ_CTwistVine";
            ObjectName[0x04] = "Obj_CollapsingPlatform";
            ObjectName[0x05] = "Obj_AIZ_LRZ_EMZ_Rock";
            ObjectName[0x06] = "Obj_MHZ_PulleyLift";
            ObjectName[0x07] = "Obj_Spring";
            ObjectName[0x08] = "Obj_Spikes";
            ObjectName[0x09] = "Obj_MHZ_CurledVine";
            ObjectName[0x0A] = "Obj_MHZ_StickyVine";
            ObjectName[0x0B] = "Obj_MHZ_SwingBarHorizontal";
            ObjectName[0x0C] = "Obj_MHZ_SwingBarVertical";
            ObjectName[0x0D] = "Obj_BreakableWall";
            ObjectName[0x0E] = "Obj_Ridge";
            ObjectName[0x0F] = "Obj_CollapsingBridge";
            ObjectName[0x10] = "Obj_MHZ_SwingVine";
            ObjectName[0x11] = "Obj_MHZ_MushroomPlatform";
            ObjectName[0x12] = "Obj_MHZ_MushroomParachute";
            ObjectName[0x13] = "Obj_MHZ_MushroomCatapult";
            ObjectName[0x14] = "Obj_MHZ_WindTunnel";
            ObjectName[0x15] = "Obj_LRZ_VerticalCylinder";
            ObjectName[0x16] = "Obj_LRZ_TwirlTunnel";
            ObjectName[0x17] = "Obj_LRZ_SinkingRock";
            ObjectName[0x18] = "Obj_LRZ_FallingSpike";
            ObjectName[0x19] = "Obj_LRZ_Door";
            ObjectName[0x1A] = "Obj_LRZ_BigDoor";
            ObjectName[0x1B] = "Obj_LRZ_FireballLauncher";
            ObjectName[0x1C] = "Obj_LRZ_ButtonHorizontal";
            ObjectName[0x1D] = "Obj_LRZ_ShootingTrigger";
            ObjectName[0x1E] = "Obj_LRZ_DashElevator";
            ObjectName[0x1F] = "Obj_LRZ_LavaFall";
            ObjectName[0x20] = "Obj_LRZ_SwingingSpikeBall";
            ObjectName[0x21] = "Obj_LRZ_SmashingSpikePlatform";
            ObjectName[0x22] = "Obj_LRZ_SpikeBall";
            ObjectName[0x23] = "Obj_MHZ_MushroomCap";
            ObjectName[0x24] = "Obj_AutomaticTunnel";
            ObjectName[0x25] = "Obj_LRZ_ChainedPlatforms";
            ObjectName[0x26] = "Obj_RollEnforcer";
            ObjectName[0x27] = "Obj_LavaCollisionMarker";
            ObjectName[0x28] = "Obj_InvisibleBarrier";
            ObjectName[0x29] = "Obj_LRZ_FlameThrower";
            ObjectName[0x2A] = "Obj_CorkFloor";
            ObjectName[0x2B] = "Obj_LRZ_OrbitingSpikeBallHorizontal";
            ObjectName[0x2C] = "Obj_LRZ_OrbitingSpikeBallVertical";
            ObjectName[0x2D] = "Obj_LRZ_SolidMovingPlatforms";
            ObjectName[0x2E] = "Obj_LRZ_SolidRock";
            ObjectName[0x2F] = "Obj_StillSprite";
            ObjectName[0x30] = "Obj_AnimatedStillSprite";
            ObjectName[0x31] = "Obj_LRZ_CollapsingBridge";
            ObjectName[0x32] = "Obj_LRZ_TurbineSprites";
            ObjectName[0x33] = "Obj_Button";
            ObjectName[0x34] = "Obj_StarPost";
            ObjectName[0x35] = "Obj_AIZ_ForegroundPlant";
            ObjectName[0x36] = "Obj_BreakableBar";
            ObjectName[0x37] = "Obj_LRZ_SpikeBallLauncher";
            ObjectName[0x38] = "Obj_SOZ_SandFalls";
            ObjectName[0x39] = "Obj_SOZ_SpawningSandBlocks";
            ObjectName[0x3A] = "Obj_SOZ_DoubleLoopPathSwapper";
            ObjectName[0x3B] = "Obj_SOZ_SpringLoopPathSwapper";
            ObjectName[0x3C] = "Obj_Door";
            ObjectName[0x3D] = "Obj_RetractingSpring";
            ObjectName[0x3E] = "Obj_SOZ_PushableRock";
            ObjectName[0x3F] = "Obj_SOZ_SpringVine";
            ObjectName[0x40] = "Obj_SOZ_RisingSandWall";
            ObjectName[0x41] = "Obj_SOZ_LightSwitch";
            ObjectName[0x42] = "Obj_SOZ_FloatingPillar";
            ObjectName[0x43] = "Obj_SOZ_SwingingPlatform";
            ObjectName[0x44] = "Obj_SOZ_BreakableSandRock";
            ObjectName[0x45] = "Obj_SOZ_PushSwitch";
            ObjectName[0x46] = "Obj_SOZ_Door";
            ObjectName[0x47] = "Obj_SOZ_SandCork";
            ObjectName[0x48] = "Obj_SOZ_RapelWire";
            ObjectName[0x49] = "Obj_SOZ_SolidSprites";
            ObjectName[0x4A] = "Obj_DEZ_FloatingPlatform";
            ObjectName[0x4B] = "Obj_TiltingBridge";
            ObjectName[0x4C] = "Obj_DEZ_HangCarrier";
            ObjectName[0x4D] = "Obj_DEZ_TorpedoLauncher";
            ObjectName[0x4E] = "Obj_DEZ_LiftPad";
            ObjectName[0x4F] = "Obj_DEZ_Staircase";
            ObjectName[0x50] = "Obj_DEZ_ConveyorBelt";
            ObjectName[0x51] = "Obj_FloatingPlatform";
            ObjectName[0x52] = "Obj_DEZ_Lightning";
            ObjectName[0x53] = "Obj_DEZ_ConveyorPad";
            ObjectName[0x54] = "Obj_Bubbler";
            ObjectName[0x55] = "Obj_DEZ_EnergyBridge";
            ObjectName[0x56] = "Obj_DEZ_CurvedEnergyBridge";
            ObjectName[0x57] = "Obj_DEZ_TunnelLauncher";
            ObjectName[0x58] = "Obj_DEZ_GravitySwitch";
            ObjectName[0x59] = "Obj_DEZ_ReverseGravityTunnel";
            ObjectName[0x5A] = "Obj_DEZ_TwirlTunnel";
            ObjectName[0x5B] = "Obj_DEZ_GravityReverser";
            ObjectName[0x5C] = "Obj_DEZ_TunnelJunction";
            ObjectName[0x5D] = "Obj_DEZ_RetractingSpring";
            ObjectName[0x5E] = "Obj_DEZ_HoverMachine";
            ObjectName[0x5F] = "Obj_DEZ_SpecialEngineController";
            ObjectName[0x60] = "Obj_DEZ_BumperWall";
            ObjectName[0x61] = "Obj_DEZ_GravityPuzzle";
            ObjectName[0x62] = "Obj_Ring";
            ObjectName[0x63] = "Obj_Ring";
            ObjectName[0x64] = "Obj_Ring";
            ObjectName[0x65] = "Obj_Ring";
            ObjectName[0x66] = "Obj_Ring";
            ObjectName[0x67] = "Obj_Ring";
            ObjectName[0x68] = "Obj_Ring";
            ObjectName[0x69] = "Obj_Ring";
            ObjectName[0x6A] = "Obj_InvisibleHurtBlockHorizontal";
            ObjectName[0x6B] = "Obj_InvisibleHurtBlockVertical";
            ObjectName[0x6C] = "Obj_TensionBridge";
            ObjectName[0x6D] = "Obj_ElectricInvisibleBlock";
            ObjectName[0x6E] = "Obj_FlamingInvisibleBlock";
            ObjectName[0x6F] = "Obj_Ring";
            ObjectName[0x70] = "Obj_Ring";
            ObjectName[0x71] = "Obj_Ring";
            ObjectName[0x72] = "Obj_Ring";
            ObjectName[0x73] = "Obj_Ring";
            ObjectName[0x74] = "Obj_SSZ_RetractingSpring";
            ObjectName[0x75] = "Obj_SSZ_SwingingCarrier";
            ObjectName[0x76] = "Obj_SSZ_RotatingPlatform";
            ObjectName[0x77] = "Obj_SSZ_RetractingBridge";
            ObjectName[0x78] = "Obj_FBZ_DEZPlayerLauncher";
            ObjectName[0x79] = "Obj_SSZ_HPZTeleporter";
            ObjectName[0x7A] = "Obj_SSZ_ElevatorBar";
            ObjectName[0x7B] = "Obj_SSZ_CollapsingBridgeDiagonal";
            ObjectName[0x7C] = "Obj_SSZ_CollapsingBridge";
            ObjectName[0x7D] = "Obj_SSZ_BouncyCloud";
            ObjectName[0x7E] = "Obj_SSZ_CollapsingColumn";
            ObjectName[0x7F] = "Obj_SSZ_FloatingPlatform";
            ObjectName[0x80] = "Obj_HiddenMonitor";
            ObjectName[0x81] = "Obj_EggCapsule";
            ObjectName[0x82] = "Obj_CutsceneKnuckles";
            ObjectName[0x83] = "Obj_CutsceneButton";
            ObjectName[0x84] = "Obj_AIZ_CutsceneSonic";
            ObjectName[0x85] = "Obj_SS_EntryRing";
            ObjectName[0x86] = "Obj_SS_GumballLevel";
            ObjectName[0x87] = "Obj_SS_GumballSideSpring";
            ObjectName[0x88] = "Obj_CNZ_BreakableWaterRisingFloor";
            ObjectName[0x89] = "Obj_CNZ_LightSwitch";
            ObjectName[0x8A] = "Obj_FBZ2_CutsceneObjects";
            ObjectName[0x8B] = "Obj_SpriteMasker";
            ObjectName[0x8C] = "Obj_Madmole";
            ObjectName[0x8D] = "Obj_Mushmeanie";
            ObjectName[0x8E] = "Obj_Dragonfly";
            ObjectName[0x8F] = "Obj_Butterdroid";
            ObjectName[0x90] = "Obj_Cluckoid";
            ObjectName[0x91] = "Obj_MHZ_ChopTree";
            ObjectName[0x92] = "Obj_MHZ_Miniboss";
            ObjectName[0x93] = "Obj_MHZ_EndBoss";
            ObjectName[0x94] = "Obj_Skorp";
            ObjectName[0x95] = "Obj_Sandworm";
            ObjectName[0x96] = "Obj_Rockn";
            ObjectName[0x97] = "Obj_SOZ_Miniboss";
            ObjectName[0x98] = "Obj_SOZ_EndBoss";
            ObjectName[0x99] = "Obj_Fireworm";
            ObjectName[0x9A] = "Obj_LRZ_ExplodingRock";
            ObjectName[0x9B] = "Obj_Toxomister";
            ObjectName[0x9C] = "Obj_LRZ_RockCrusher";
            ObjectName[0x9D] = "Obj_LRZ_Miniboss";
            ObjectName[0x9E] = "Obj_LRZ_TorpedoLauncher";
            ObjectName[0x9F] = "Obj_Ring";
            ObjectName[0xA0] = "Obj_EggRobo";
            ObjectName[0xA1] = "Obj_SSZ_GHZBoss";
            ObjectName[0xA2] = "Obj_SSZ_MTZBoss";
            ObjectName[0xA3] = "Obj_SSZ_MechaSonicBoss";
            ObjectName[0xA4] = "Obj_Spikebonker";
            ObjectName[0xA5] = "Obj_Chainspike";
            ObjectName[0xA6] = "Obj_DEZ_Miniboss";
            ObjectName[0xA7] = "Obj_DEZ_EndBoss";
            ObjectName[0xA8] = "Obj_MHZ_ForceCrouch";
            ObjectName[0xA9] = "Obj_MHZ_KnucklesButton";
            ObjectName[0xAA] = "Obj_SOZ_Ghosts";
            ObjectName[0xAB] = "Obj_SOZ2_GhostCapsuleLoader";
            ObjectName[0xAC] = "Obj_SOZ2_GhostCapsule";
            ObjectName[0xAD] = "Obj_LRZ3_Platforms";
            ObjectName[0xAE] = "Obj_LRZ_KnucklesEndHelper";
            ObjectName[0xAF] = "Obj_SSZ_KnucklesButton";
            ObjectName[0xB0] = "Obj_HPZ_MasterEmerald";
            ObjectName[0xB1] = "Obj_HPZ_MidwayPalleteChanger";
            ObjectName[0xB2] = "Obj_SSZ_EggRobo";
            ObjectName[0xB3] = "Obj_ContactLevelChanger";
            ObjectName[0xB4] = "Obj_HPZ_SuperEmeralds";
            ObjectName[0xB5] = "Obj_HPZ_SpecialStageCreater";
            ObjectName[0xB6] = "Obj_DDZ_BossShip";
            ObjectName[0xB7] = "Obj_DDZ_Asteroid";
            ObjectName[0xB8] = "Obj_DDZ_Missile";
        }
        // If 3'Mixed level...
        else {
            ObjectName[ThremixedIDS::Ring] = "Obj_Ring";
            ObjectName[ThremixedIDS::Monitor] = "Obj_Monitor";
            ObjectName[ThremixedIDS::PathSwap] = "Obj_PathSwap";
            ObjectName[ThremixedIDS::CollapsingPlatform] = "Obj_CollapsingPlatform";
            ObjectName[ThremixedIDS::Spring] = "Obj_Spring";
            ObjectName[ThremixedIDS::Spikes] = "Obj_Spikes";
            ObjectName[ThremixedIDS::BreakableWall] = "Obj_BreakableWall";
            ObjectName[ThremixedIDS::Ridge] = "Obj_Ridge";
            ObjectName[ThremixedIDS::CollapsingBridge] = "Obj_CollapsingBridge";
            ObjectName[ThremixedIDS::ContactFirceFallAnimator] = "Obj_ContactFirceFallAnimator"; // Unused
            ObjectName[ThremixedIDS::AutomaticTunnel] = "Obj_AutomaticTunnel";
            ObjectName[ThremixedIDS::RollEnforcer] = "Obj_RollEnforcer";
            ObjectName[ThremixedIDS::LavaCollisionMarker] = "Obj_LavaCollisionMarker"; // Unused
            ObjectName[ThremixedIDS::InvisibleBarrier] = "Obj_InvisibleBarrier";
            ObjectName[ThremixedIDS::CorkFloor] = "Obj_CorkFloor";
            ObjectName[ThremixedIDS::StillSprite] = "Obj_StillSprite";
            ObjectName[ThremixedIDS::AnimatedStillSprite] = "Obj_AnimatedStillSprite";
            ObjectName[ThremixedIDS::Button] = "Obj_Button";
            ObjectName[ThremixedIDS::StarPost] = "Obj_StarPost";
            ObjectName[ThremixedIDS::BreakableBar] = "Obj_BreakableBar";
            ObjectName[ThremixedIDS::Door] = "Obj_Door";
            ObjectName[ThremixedIDS::RetractingSpring] = "Obj_RetractingSpring";
            ObjectName[ThremixedIDS::FloatingPlatform] = "Obj_FloatingPlatform";
            ObjectName[ThremixedIDS::Bubbler] = "Obj_Bubbler";
            ObjectName[ThremixedIDS::InvisibleHurtBlockHorizontal] = "Obj_InvisibleHurtBlockHorizontal";
            ObjectName[ThremixedIDS::InvisibleHurtBlockVertical] = "Obj_InvisibleHurtBlockVertical";
            ObjectName[ThremixedIDS::TensionBridge] = "Obj_TensionBridge";
            ObjectName[ThremixedIDS::WaterDrop] = "Obj_WaterDrop";
            ObjectName[ThremixedIDS::HiddenMonitor] = "Obj_HiddenMonitor";
            ObjectName[ThremixedIDS::EggCapsule] = "Obj_EggCapsule";
            ObjectName[ThremixedIDS::CutsceneKnuckles] = "Obj_CutsceneKnuckles";
            ObjectName[ThremixedIDS::CutsceneButton] = "Obj_CutsceneButton";
            ObjectName[ThremixedIDS::SpriteMasker] = "Obj_SpriteMasker";
            ObjectName[ThremixedIDS::Tulipon] = "Obj_Tulipon";
            ObjectName[ThremixedIDS::RhinoBot] = "Obj_RhinoBot";
            ObjectName[ThremixedIDS::MonkeyDude] = "Obj_MonkeyDude";
            ObjectName[ThremixedIDS::CaterKillerJr] = "Obj_CaterKillerJr";
            ObjectName[ThremixedIDS::Jawz] = "Obj_Jawz";
            ObjectName[ThremixedIDS::Blastoid] = "Obj_Blastoid";
            ObjectName[ThremixedIDS::Buggernaut] = "Obj_Buggernaut";
            ObjectName[ThremixedIDS::TurboSpiker] = "Obj_TurboSpiker";
            ObjectName[ThremixedIDS::MegaChomper] = "Obj_MegaChomper";
            ObjectName[ThremixedIDS::Poindexter] = "Obj_Poindexter";
            ObjectName[ThremixedIDS::BubblesBadnik] = "Obj_BubblesBadnik";
            ObjectName[ThremixedIDS::Spiker] = "Obj_Spiker";
            ObjectName[ThremixedIDS::Mantis] = "Obj_Mantis";
            ObjectName[ThremixedIDS::Clamer] = "Obj_Clamer";
            ObjectName[ThremixedIDS::Batbright] = "Obj_Batbright";
            ObjectName[ThremixedIDS::Blaster] = "Obj_Blaster";
            ObjectName[ThremixedIDS::TechnoSqueek] = "Obj_TechnoSqueek";
            ObjectName[ThremixedIDS::Penguinator] = "Obj_Penguinator";
            ObjectName[ThremixedIDS::StarPointer] = "Obj_StarPointer";
            ObjectName[ThremixedIDS::SnaleBlaster] = "Obj_SnaleBlaster";
            ObjectName[ThremixedIDS::Ribot] = "Obj_Ribot";
            ObjectName[ThremixedIDS::Orbinaut] = "Obj_Orbinaut";
            ObjectName[ThremixedIDS::Corkey] = "Obj_Corkey";
            ObjectName[ThremixedIDS::Flybot767] = "Obj_Flybot767";
            ObjectName[ThremixedIDS::GumballItem] = "Obj_GumballItem";
            ObjectName[ThremixedIDS::MagneticOrb] = "Obj_MagneticOrb";
            ObjectName[ThremixedIDS::TiltingBridge] = "Obj_TiltingBridge";
            ObjectName[ThremixedIDS::ElectricInvisibleBlock] = "Obj_ElectricInvisibleBlock";
            ObjectName[ThremixedIDS::FlamingInvisibleBlock] = "Obj_FlamingInvisibleBlock";
            ObjectName[ThremixedIDS::Madmole] = "Obj_Madmole";
            ObjectName[ThremixedIDS::Mushmeanie] = "Obj_Mushmeanie";
            ObjectName[ThremixedIDS::Dragonfly] = "Obj_Dragonfly";
            ObjectName[ThremixedIDS::Butterdroid] = "Obj_Butterdroid";
            ObjectName[ThremixedIDS::Cluckoid] = "Obj_Cluckoid";
            ObjectName[ThremixedIDS::Skorp] = "Obj_Skorp";
            ObjectName[ThremixedIDS::Sandworm] = "Obj_Sandworm";
            ObjectName[ThremixedIDS::Rockn] = "Obj_Rockn";
            ObjectName[ThremixedIDS::Fireworm] = "Obj_Fireworm";
            ObjectName[ThremixedIDS::Toxomister] = "Obj_Toxomister";
            ObjectName[ThremixedIDS::EggRobo] = "Obj_EggRobo";
            ObjectName[ThremixedIDS::Spikebonker] = "Obj_Spikebonker";
            ObjectName[ThremixedIDS::Chainspike] = "Obj_Chainspike";
            ObjectName[ThremixedIDS::ContactLevelChanger] = "Obj_ContactLevelChanger";

            ObjectName[ThremixedIDS::P2RotateObject] = "Obj_P2RotateObject"; // Purpose unknown.

            ObjectName[ThremixedIDS::RetractingSpring2P] = "Obj_RetractingSpring2P";
            ObjectName[ThremixedIDS::Item2P] = "Obj_Item2P";
            ObjectName[ThremixedIDS::GoalMarker2P] = "Obj_GoalMarker2P";

            ObjectName[ThremixedIDS::AIZ_HollowTree] = "Obj_AIZ_HollowTree";
            ObjectName[ThremixedIDS::AIZ_LRZ_EMZ_Rock] = "Obj_AIZ_LRZ_EMZ_Rock";
            ObjectName[ThremixedIDS::AIZ_RideVine] = "Obj_AIZ_RideVine";
            ObjectName[ThremixedIDS::AIZ_SwingVine] = "Obj_AIZ_SwingVine";
            ObjectName[ThremixedIDS::AIZ_DisappearingFloor] = "Obj_AIZ_DisappearingFloor";
            ObjectName[ThremixedIDS::AIZ_FlippingBridge] = "Obj_AIZ_FlippingBridge";
            ObjectName[ThremixedIDS::AIZ_CollapsingLogBridge] = "Obj_AIZ_CollapsingLogBridge";
            ObjectName[ThremixedIDS::AIZ_FallingLog] = "Obj_AIZ_FallingLog";
            ObjectName[ThremixedIDS::AIZ_SpikedLog] = "Obj_AIZ_SpikedLog";
            ObjectName[ThremixedIDS::AIZ_DrawBridge] = "Obj_AIZ_DrawBridge";
            ObjectName[ThremixedIDS::AIZ_ForegroundPlant] = "Obj_AIZ_ForegroundPlant";
            ObjectName[ThremixedIDS::AIZ_CutsceneSonic] = "Obj_AIZ_CutsceneSonic";
            ObjectName[ThremixedIDS::AIZ_Miniboss_Cutscene] = "Obj_AIZ_Miniboss_Cutscene";
            ObjectName[ThremixedIDS::AIZ_Miniboss] = "Obj_AIZ_Miniboss";
            ObjectName[ThremixedIDS::AIZ_EndBoss] = "Obj_AIZ_EndBoss";
            ObjectName[ThremixedIDS::AIZ1_TreeBark] = "Obj_AIZ1_TreeBark";
            ObjectName[ThremixedIDS::AIZ1_RopePeg] = "Obj_AIZ1_RopePeg";

            ObjectName[ThremixedIDS::HCZ_VWaterStream] = "Obj_HCZ_VWaterStream";
            ObjectName[ThremixedIDS::HCZ_CGZ_Fan] = "Obj_HCZ_CGZ_Fan";
            ObjectName[ThremixedIDS::HCZ_LargeFan] = "Obj_HCZ_LargeFan";
            ObjectName[ThremixedIDS::HCZ_HandLauncher] = "Obj_HCZ_HandLauncher";
            ObjectName[ThremixedIDS::HCZ_WaterWall] = "Obj_HCZ_WaterWall";
            ObjectName[ThremixedIDS::HCZ_ConveyorBelt] = "Obj_HCZ_ConveyorBelt";
            ObjectName[ThremixedIDS::HCZ_ConveyorSpike] = "Obj_HCZ_ConveyorSpike";
            ObjectName[ThremixedIDS::HCZ_Block] = "Obj_HCZ_Block";
            ObjectName[ThremixedIDS::HCZ_SnakeBlocks] = "Obj_HCZ_SnakeBlocks";
            ObjectName[ThremixedIDS::HCZ_SpinningColumn] = "Obj_HCZ_SpinningColumn";
            ObjectName[ThremixedIDS::HCZ_Slide] = "Obj_HCZ_Slide";
            ObjectName[ThremixedIDS::HCZ_WaterSplash] = "Obj_HCZ_WaterSplash";
            ObjectName[ThremixedIDS::HCZ_Miniboss] = "Obj_HCZ_Miniboss";
            ObjectName[ThremixedIDS::HCZ_EndBoss] = "Obj_HCZ_EndBoss";

            ObjectName[ThremixedIDS::MGZ_LBZSmashingPillar] = "Obj_MGZ_LBZSmashingPillar";
            ObjectName[ThremixedIDS::MGZ_QuickMud] = "Obj_MGZ_QuickMud";
            ObjectName[ThremixedIDS::MGZ_RunAroundPole] = "Obj_MGZ_RunAroundPole";
            ObjectName[ThremixedIDS::MGZ_SwingingPlatform] = "Obj_MGZ_SwingingPlatform";
            ObjectName[ThremixedIDS::MGZ_HeadTrigger] = "Obj_MGZ_HeadTrigger";
            ObjectName[ThremixedIDS::MGZ_MovingSpikePlatform] = "Obj_MGZ_MovingSpikePlatform";
            ObjectName[ThremixedIDS::MGZ_TriggerPlatform] = "Obj_MGZ_TriggerPlatform";
            ObjectName[ThremixedIDS::MGZ_SwingingSpikeBall] = "Obj_MGZ_SwingingSpikeBall";
            ObjectName[ThremixedIDS::MGZ_DashTrigger] = "Obj_MGZ_DashTrigger";
            ObjectName[ThremixedIDS::MGZ_Pulley] = "Obj_MGZ_Pulley";
            ObjectName[ThremixedIDS::MGZ_TopPlatform] = "Obj_MGZ_TopPlatform";
            ObjectName[ThremixedIDS::MGZ_TopLauncher] = "Obj_MGZ_TopLauncher";
            ObjectName[ThremixedIDS::MGZ_CutsceneMiniBoss] = "Obj_MGZ_CutsceneMiniBoss";
            ObjectName[ThremixedIDS::MGZ_CTwistVine] = "Obj_MGZ_CTwistVine";
            ObjectName[ThremixedIDS::MGZ_MiniBoss] = "Obj_MGZ_MiniBoss";
            ObjectName[ThremixedIDS::MGZ_Boss] = "Obj_MGZ_Boss";
            ObjectName[ThremixedIDS::MGZ_KnucklesBoss] = "Obj_MGZ_KnucklesBoss";
            ObjectName[ThremixedIDS::MGZ2_DrillingEggman] = "Obj_MGZ2_DrillingEggman";

            ObjectName[ThremixedIDS::CNZ_Balloon] = "Obj_CNZ_Balloon";
            ObjectName[ThremixedIDS::CNZ_Cannon] = "Obj_CNZ_Cannon";
            ObjectName[ThremixedIDS::CNZ_RisingPlatform] = "Obj_CNZ_RisingPlatform";
            ObjectName[ThremixedIDS::CNZ_TrapDoor] = "Obj_CNZ_TrapDoor";
            ObjectName[ThremixedIDS::CNZ_LightBulb] = "Obj_CNZ_LightBulb";
            ObjectName[ThremixedIDS::CNZ_HoverFan] = "Obj_CNZ_HoverFan";
            ObjectName[ThremixedIDS::CNZ_RotatingBarrels] = "Obj_CNZ_RotatingBarrels";
            ObjectName[ThremixedIDS::CNZ_VacuumTubes] = "Obj_CNZ_VacuumTubes";
            ObjectName[ThremixedIDS::CNZ_RotatingWheel] = "Obj_CNZ_RotatingWheel";
            ObjectName[ThremixedIDS::CNZ_Bumper] = "Obj_CNZ_Bumper";
            ObjectName[ThremixedIDS::CNZ_BouncepadSprings] = "Obj_CNZ_BouncepadSprings";
            ObjectName[ThremixedIDS::CNZ_Corkscrew] = "Obj_CNZ_Corkscrew";
            ObjectName[ThremixedIDS::CNZ_BarberPoleSprite] = "Obj_CNZ_BarberPoleSprite";
            ObjectName[ThremixedIDS::CNZ_RotatingMesh] = "Obj_CNZ_RotatingMesh";
            ObjectName[ThremixedIDS::CNZ_BreakableWaterRisingFloor] = "Obj_CNZ_BreakableWaterRisingFloor";
            ObjectName[ThremixedIDS::CNZ_LightSwitch] = "Obj_CNZ_LightSwitch";
            ObjectName[ThremixedIDS::CNZ_Blastoid] = "Obj_CNZ_Blastoid";
            ObjectName[ThremixedIDS::CNZ_Miniboss] = "Obj_CNZ_Miniboss";
            ObjectName[ThremixedIDS::CNZ_EndBoss] = "Obj_CNZ_EndBoss";

            ObjectName[ThremixedIDS::ICZ_CrushingColumn] = "Obj_ICZ_CrushingColumn";
            ObjectName[ThremixedIDS::ICZ_PathFollowPlatform] = "Obj_ICZ_PathFollowPlatform";
            ObjectName[ThremixedIDS::ICZ_BreakableWall] = "Obj_ICZ_BreakableWall";
            ObjectName[ThremixedIDS::ICZ_Freezer] = "Obj_ICZ_Freezer";
            ObjectName[ThremixedIDS::ICZ_SegmentColumn] = "Obj_ICZ_SegmentColumn";
            ObjectName[ThremixedIDS::ICZ_SwingingPlatform] = "Obj_ICZ_SwingingPlatform";
            ObjectName[ThremixedIDS::ICZ_Stalagtite] = "Obj_ICZ_Stalagtite";
            ObjectName[ThremixedIDS::ICZ_IceCube] = "Obj_ICZ_IceCube";
            ObjectName[ThremixedIDS::ICZ_IceSpikes] = "Obj_ICZ_IceSpikes";
            ObjectName[ThremixedIDS::ICZ_HarmfulIce] = "Obj_ICZ_HarmfulIce";
            ObjectName[ThremixedIDS::ICZ_SnowPile] = "Obj_ICZ_SnowPile";
            ObjectName[ThremixedIDS::ICZ_TensionPlatform] = "Obj_ICZ_TensionPlatform";
            ObjectName[ThremixedIDS::ICZ_RightTrampolineSupport] = "Obj_ICZ_RightTrampolineSupport";
            ObjectName[ThremixedIDS::ICZ_Miniboss] = "Obj_ICZ_Miniboss";
            ObjectName[ThremixedIDS::ICZ_EndBoss] = "Obj_ICZ_EndBoss";

            ObjectName[ThremixedIDS::MHZ_PulleyLift] = "Obj_MHZ_PulleyLift";
            ObjectName[ThremixedIDS::MHZ_CurledVine] = "Obj_MHZ_CurledVine";
            ObjectName[ThremixedIDS::MHZ_StickyVine] = "Obj_MHZ_StickyVine";
            ObjectName[ThremixedIDS::MHZ_SwingBarHorizontal] = "Obj_MHZ_SwingBarHorizontal";
            ObjectName[ThremixedIDS::MHZ_SwingBarVertical] = "Obj_MHZ_SwingBarVertical";
            ObjectName[ThremixedIDS::MHZ_SwingVine] = "Obj_MHZ_SwingVine";
            ObjectName[ThremixedIDS::MHZ_MushroomPlatform] = "Obj_MHZ_MushroomPlatform";
            ObjectName[ThremixedIDS::MHZ_MushroomParachute] = "Obj_MHZ_MushroomParachute";
            ObjectName[ThremixedIDS::MHZ_MushroomCatapult] = "Obj_MHZ_MushroomCatapult";
            ObjectName[ThremixedIDS::MHZ_MushroomCap] = "Obj_MHZ_MushroomCap";
            ObjectName[ThremixedIDS::MHZ_WindTunnel] = "Obj_MHZ_WindTunnel";
            ObjectName[ThremixedIDS::MHZ_ChopTree] = "Obj_MHZ_ChopTree";
            ObjectName[ThremixedIDS::MHZ_ForceCrouch] = "Obj_MHZ_ForceCrouch";
            ObjectName[ThremixedIDS::MHZ_KnucklesButton] = "Obj_MHZ_KnucklesButton";
            ObjectName[ThremixedIDS::MHZ_Miniboss] = "Obj_MHZ_Miniboss";
            ObjectName[ThremixedIDS::MHZ_EndBoss] = "Obj_MHZ_EndBoss";

            ObjectName[ThremixedIDS::FBZ_RotatingMesh] = "Obj_FBZ_RotatingMesh";
            ObjectName[ThremixedIDS::FBZ_TwistingCylinders] = "Obj_FBZ_TwistingCylinders";
            ObjectName[ThremixedIDS::FBZ_FloatingPlatform] = "Obj_FBZ_FloatingPlatform";
            ObjectName[ThremixedIDS::FBZ_ChainLink] = "Obj_FBZ_ChainLink";
            ObjectName[ThremixedIDS::FBZ_MagneticSpikeBall] = "Obj_FBZ_MagneticSpikeBall";
            ObjectName[ThremixedIDS::FBZ_MagneticPlatform] = "Obj_FBZ_MagneticPlatform";
            ObjectName[ThremixedIDS::FBZ_SnakePlatform] = "Obj_FBZ_SnakePlatform";
            ObjectName[ThremixedIDS::FBZ_BentPipe] = "Obj_FBZ_BentPipe";
            ObjectName[ThremixedIDS::FBZ_RotatingPlatform] = "Obj_FBZ_RotatingPlatform";
            ObjectName[ThremixedIDS::FBZ_DEZPlayerLauncher] = "Obj_FBZ_DEZPlayerLauncher";
            ObjectName[ThremixedIDS::FBZ_DisappearingPlatform] = "Obj_FBZ_DisappearingPlatform";
            ObjectName[ThremixedIDS::FBZ_ScrewDoor] = "Obj_FBZ_ScrewDoor";
            ObjectName[ThremixedIDS::FBZ_FanPole] = "Obj_FBZ_FanPole";
            ObjectName[ThremixedIDS::FBZ_Propeller] = "Obj_FBZ_Propeller";
            ObjectName[ThremixedIDS::FBZ_Piston] = "Obj_FBZ_Piston";
            ObjectName[ThremixedIDS::FBZ_PlatformBlocks] = "Obj_FBZ_PlatformBlocks";
            ObjectName[ThremixedIDS::FBZ_MissileLauncher] = "Obj_FBZ_MissileLauncher";
            ObjectName[ThremixedIDS::FBZ_ExitDoor] = "Obj_FBZ_ExitDoor";
            ObjectName[ThremixedIDS::FBZ_EggPrison] = "Obj_FBZ_EggPrison";
            ObjectName[ThremixedIDS::FBZ_SpringPlunger] = "Obj_FBZ_SpringPlunger";
            ObjectName[ThremixedIDS::FBZ_WallMissile] = "Obj_FBZ_WallMissile";
            ObjectName[ThremixedIDS::FBZ_Mine] = "Obj_FBZ_Mine";
            ObjectName[ThremixedIDS::FBZ_Elevator] = "Obj_FBZ_Elevator";
            ObjectName[ThremixedIDS::FBZ_TrapSpring] = "Obj_FBZ_TrapSpring";
            ObjectName[ThremixedIDS::FBZ_Flamethrower] = "Obj_FBZ_Flamethrower";
            ObjectName[ThremixedIDS::FBZ_SpiderCrane] = "Obj_FBZ_SpiderCrane";
            ObjectName[ThremixedIDS::FBZ_MagneticPendulum] = "Obj_FBZ_MagneticPendulum";
            ObjectName[ThremixedIDS::FBZ_Laser_Boss] = "Obj_FBZ_Laser_Boss";
            ObjectName[ThremixedIDS::FBZ_EndBoss] = "Obj_FBZ_EndBoss";
            ObjectName[ThremixedIDS::FBZ1_Boss] = "Obj_FBZ1_Boss";
            ObjectName[ThremixedIDS::FBZ2_CutsceneObjects] = "Obj_FBZ2_CutsceneObjects";

            ObjectName[ThremixedIDS::LRZ_TubeElevator] = "Obj_LRZ_TubeElevator";
            ObjectName[ThremixedIDS::LRZ_MovingPlatform] = "Obj_LRZ_MovingPlatform";
            ObjectName[ThremixedIDS::LRZ_FlagPlatform] = "Obj_LRZ_FlagPlatform"; // Unused
            ObjectName[ThremixedIDS::LRZ_Door] = "Obj_LRZ_Door";
            ObjectName[ThremixedIDS::LRZ_BigDoor] = "Obj_LRZ_BigDoor";
            ObjectName[ThremixedIDS::LRZ_ExplodingTrigger] = "Obj_LRZ_ExplodingTrigger";
            ObjectName[ThremixedIDS::LRZ_TriggerBridge] = "Obj_LRZ_TriggerBridge";
            ObjectName[ThremixedIDS::LRZ_PlayerLauncher] = "Obj_LRZ_PlayerLauncher";
            ObjectName[ThremixedIDS::LRZ_FlameThrower] = "Obj_LRZ_FlameThrower";
            ObjectName[ThremixedIDS::LRZ_RideGrapple] = "Obj_LRZ_RideGrapple";
            ObjectName[ThremixedIDS::LRZ_CupElevator] = "Obj_LRZ_CupElevator";
            ObjectName[ThremixedIDS::LRZ_CupElevatorPole] = "Obj_LRZ_CupElevatorPole";
            ObjectName[ThremixedIDS::LRZ_PipePlug] = "Obj_LRZ_PipePlug";
            ObjectName[ThremixedIDS::LRZ_HangPlatform] = "Obj_LRZ_HangPlatform";
            ObjectName[ThremixedIDS::LRZ_FireballLauncher] = "Obj_LRZ_FireballLauncher";
            ObjectName[ThremixedIDS::LRZ_ButtonHorizontal] = "Obj_LRZ_ButtonHorizontal";
            ObjectName[ThremixedIDS::LRZ_ShootingTrigger] = "Obj_LRZ_ShootingTrigger";
            ObjectName[ThremixedIDS::LRZ_DashElevator] = "Obj_LRZ_DashElevator";
            ObjectName[ThremixedIDS::LRZ_LavaFall] = "Obj_LRZ_LavaFall";
            ObjectName[ThremixedIDS::LRZ_SwingingSpikeBall] = "Obj_LRZ_SwingingSpikeBall";
            ObjectName[ThremixedIDS::LRZ_SmashingSpikePlatform] = "Obj_LRZ_SmashingSpikePlatform";
            ObjectName[ThremixedIDS::LRZ_SpikeBall] = "Obj_LRZ_SpikeBall";
            ObjectName[ThremixedIDS::LRZ_SpikeBallLauncher] = "Obj_LRZ_SpikeBallLauncher";
            ObjectName[ThremixedIDS::LRZ_OrbitingSpikeBallHorizontal] = "Obj_LRZ_OrbitingSpikeBallHorizontal";
            ObjectName[ThremixedIDS::LRZ_OrbitingSpikeBallVertical] = "Obj_LRZ_OrbitingSpikeBallVertical";
            ObjectName[ThremixedIDS::LRZ_SolidMovingPlatforms] = "Obj_LRZ_SolidMovingPlatforms";
            ObjectName[ThremixedIDS::LRZ_SolidRock] = "Obj_LRZ_SolidRock";
            ObjectName[ThremixedIDS::LRZ_SpinLauncher] = "Obj_LRZ_SpinLauncher";
            ObjectName[ThremixedIDS::LRZ_LoweringGrapple] = "Obj_LRZ_LoweringGrapple";
            ObjectName[ThremixedIDS::LRZ_GateLaser] = "Obj_LRZ_GateLaser";
            ObjectName[ThremixedIDS::LRZ_Siren] = "Obj_LRZ_Siren";
            ObjectName[ThremixedIDS::LRZ_ChainedPlatforms] = "Obj_LRZ_ChainedPlatforms";
            ObjectName[ThremixedIDS::LRZ_FallingSpike] = "Obj_LRZ_FallingSpike";
            ObjectName[ThremixedIDS::LRZ_SinkingRock] = "Obj_LRZ_SinkingRock";
            ObjectName[ThremixedIDS::LRZ_ExplodingRock] = "Obj_LRZ_ExplodingRock";
            ObjectName[ThremixedIDS::LRZ_RockCrusher] = "Obj_LRZ_RockCrusher";
            ObjectName[ThremixedIDS::LRZ_TorpedoLauncher] = "Obj_LRZ_TorpedoLauncher";
            ObjectName[ThremixedIDS::LRZ_RotatingCylinders] = "Obj_LRZ_RotatingCylinders";
            ObjectName[ThremixedIDS::LRZ_VerticalCylinder] = "Obj_LRZ_VerticalCylinder";
            ObjectName[ThremixedIDS::LRZ_CollapsingBridge] = "Obj_LRZ_CollapsingBridge";
            ObjectName[ThremixedIDS::LRZ_TurbineSprites] = "Obj_LRZ_TurbineSprites";
            ObjectName[ThremixedIDS::LRZ_TwirlTunnel] = "Obj_LRZ_TwirlTunnel";
            ObjectName[ThremixedIDS::LRZ_KnucklesEndHelper] = "Obj_LRZ_KnucklesEndHelper";
            ObjectName[ThremixedIDS::LRZ_Miniboss] = "Obj_LRZ_Miniboss";
            ObjectName[ThremixedIDS::LRZ_FinalBoss1] = "Obj_LRZ_FinalBoss1";
            ObjectName[ThremixedIDS::LRZ_FinalBoss2] = "Obj_LRZ_FinalBoss2";
            ObjectName[ThremixedIDS::LRZ_EndBoss] = "Obj_LRZ_EndBoss";
            ObjectName[ThremixedIDS::LRZ1_Eggman] = "Obj_LRZ1_Eggman";
            ObjectName[ThremixedIDS::LRZ1_MiniBoss] = "Obj_LRZ1_MiniBoss";
            ObjectName[ThremixedIDS::LRZ1_KnucklesMiniBossBox] = "Obj_LRZ1_KnucklesMiniBossBox";
            ObjectName[ThremixedIDS::LRZ2_EggMobile] = "Obj_LRZ2_EggMobile";
            ObjectName[ThremixedIDS::LRZ2_TallBoss] = "Obj_LRZ2_TallBoss";
            ObjectName[ThremixedIDS::LRZ3_Platforms] = "Obj_LRZ3_Platforms";

            ObjectName[ThremixedIDS::SOZ_SandFalls] = "Obj_SOZ_SandFalls";
            ObjectName[ThremixedIDS::SOZ_SpawningSandBlocks] = "Obj_SOZ_SpawningSandBlocks";
            ObjectName[ThremixedIDS::SOZ_DoubleLoopPathSwapper] = "Obj_SOZ_DoubleLoopPathSwapper";
            ObjectName[ThremixedIDS::SOZ_SpringLoopPathSwapper] = "Obj_SOZ_SpringLoopPathSwapper";
            ObjectName[ThremixedIDS::SOZ_PushableRock] = "Obj_SOZ_PushableRock";
            ObjectName[ThremixedIDS::SOZ_SpringVine] = "Obj_SOZ_SpringVine";
            ObjectName[ThremixedIDS::SOZ_RisingSandWall] = "Obj_SOZ_RisingSandWall";
            ObjectName[ThremixedIDS::SOZ_LightSwitch] = "Obj_SOZ_LightSwitch";
            ObjectName[ThremixedIDS::SOZ_FloatingPillar] = "Obj_SOZ_FloatingPillar";
            ObjectName[ThremixedIDS::SOZ_SwingingPlatform] = "Obj_SOZ_SwingingPlatform";
            ObjectName[ThremixedIDS::SOZ_BreakableSandRock] = "Obj_SOZ_BreakableSandRock";
            ObjectName[ThremixedIDS::SOZ_PushSwitch] = "Obj_SOZ_PushSwitch";
            ObjectName[ThremixedIDS::SOZ_Door] = "Obj_SOZ_Door";
            ObjectName[ThremixedIDS::SOZ_SandCork] = "Obj_SOZ_SandCork";
            ObjectName[ThremixedIDS::SOZ_RapelWire] = "Obj_SOZ_RapelWire";
            ObjectName[ThremixedIDS::SOZ_SolidSprites] = "Obj_SOZ_SolidSprites";
            ObjectName[ThremixedIDS::SOZ_Ghosts] = "Obj_SOZ_Ghosts";
            ObjectName[ThremixedIDS::SOZ_Miniboss] = "Obj_SOZ_Miniboss";
            ObjectName[ThremixedIDS::SOZ_EndBoss] = "Obj_SOZ_EndBoss";
            ObjectName[ThremixedIDS::SOZ2_GhostCapsuleLoader] = "Obj_SOZ2_GhostCapsuleLoader";
            ObjectName[ThremixedIDS::SOZ2_GhostCapsule] = "Obj_SOZ2_GhostCapsule";

            ObjectName[ThremixedIDS::HPZ_MasterEmerald] = "Obj_HPZ_MasterEmerald";
            ObjectName[ThremixedIDS::HPZ_MidwayPalleteChanger] = "Obj_HPZ_MidwayPalleteChanger";
            ObjectName[ThremixedIDS::HPZ_SuperEmeralds] = "Obj_HPZ_SuperEmeralds";
            ObjectName[ThremixedIDS::HPZ_SpecialStageCreater] = "Obj_HPZ_SpecialStageCreater";

            ObjectName[ThremixedIDS::SSZ_RetractingSpring] = "Obj_SSZ_RetractingSpring";
            ObjectName[ThremixedIDS::SSZ_SwingingCarrier] = "Obj_SSZ_SwingingCarrier";
            ObjectName[ThremixedIDS::SSZ_RotatingPlatform] = "Obj_SSZ_RotatingPlatform";
            ObjectName[ThremixedIDS::SSZ_RetractingBridge] = "Obj_SSZ_RetractingBridge";
            ObjectName[ThremixedIDS::SSZ_HPZTeleporter] = "Obj_SSZ_HPZTeleporter";
            ObjectName[ThremixedIDS::SSZ_ElevatorBar] = "Obj_SSZ_ElevatorBar";
            ObjectName[ThremixedIDS::SSZ_CollapsingBridgeDiagonal] = "Obj_SSZ_CollapsingBridgeDiagonal";
            ObjectName[ThremixedIDS::SSZ_CollapsingBridge] = "Obj_SSZ_CollapsingBridge";
            ObjectName[ThremixedIDS::SSZ_BouncyCloud] = "Obj_SSZ_BouncyCloud";
            ObjectName[ThremixedIDS::SSZ_CollapsingColumn] = "Obj_SSZ_CollapsingColumn";
            ObjectName[ThremixedIDS::SSZ_FloatingPlatform] = "Obj_SSZ_FloatingPlatform";
            ObjectName[ThremixedIDS::SSZ_KnucklesButton] = "Obj_SSZ_KnucklesButton";
            ObjectName[ThremixedIDS::SSZ_EggRobo] = "Obj_SSZ_EggRobo";
            ObjectName[ThremixedIDS::SSZ_GHZBoss] = "Obj_SSZ_GHZBoss";
            ObjectName[ThremixedIDS::SSZ_MTZBoss] = "Obj_SSZ_MTZBoss";
            ObjectName[ThremixedIDS::SSZ_MechaSonicBoss] = "Obj_SSZ_MechaSonicBoss";

            ObjectName[ThremixedIDS::DEZ_FloatingPlatform] = "Obj_DEZ_FloatingPlatform";
            ObjectName[ThremixedIDS::DEZ_HangCarrier] = "Obj_DEZ_HangCarrier";
            ObjectName[ThremixedIDS::DEZ_TorpedoLauncher] = "Obj_DEZ_TorpedoLauncher";
            ObjectName[ThremixedIDS::DEZ_LiftPad] = "Obj_DEZ_LiftPad";
            ObjectName[ThremixedIDS::DEZ_Staircase] = "Obj_DEZ_Staircase";
            ObjectName[ThremixedIDS::DEZ_ConveyorBelt] = "Obj_DEZ_ConveyorBelt";
            ObjectName[ThremixedIDS::DEZ_Lightning] = "Obj_DEZ_Lightning";
            ObjectName[ThremixedIDS::DEZ_ConveyorPad] = "Obj_DEZ_ConveyorPad";
            ObjectName[ThremixedIDS::DEZ_EnergyBridge] = "Obj_DEZ_EnergyBridge";
            ObjectName[ThremixedIDS::DEZ_CurvedEnergyBridge] = "Obj_DEZ_CurvedEnergyBridge";
            ObjectName[ThremixedIDS::DEZ_TunnelLauncher] = "Obj_DEZ_TunnelLauncher";
            ObjectName[ThremixedIDS::DEZ_GravitySwitch] = "Obj_DEZ_GravitySwitch";
            ObjectName[ThremixedIDS::DEZ_ReverseGravityTunnel] = "Obj_DEZ_ReverseGravityTunnel";
            ObjectName[ThremixedIDS::DEZ_TwirlTunnel] = "Obj_DEZ_TwirlTunnel";
            ObjectName[ThremixedIDS::DEZ_GravityReverser] = "Obj_DEZ_GravityReverser";
            ObjectName[ThremixedIDS::DEZ_TunnelJunction] = "Obj_DEZ_TunnelJunction";
            ObjectName[ThremixedIDS::DEZ_RetractingSpring] = "Obj_DEZ_RetractingSpring";
            ObjectName[ThremixedIDS::DEZ_HoverMachine] = "Obj_DEZ_HoverMachine";
            ObjectName[ThremixedIDS::DEZ_SpecialEngineController] = "Obj_DEZ_SpecialEngineController";
            ObjectName[ThremixedIDS::DEZ_BumperWall] = "Obj_DEZ_BumperWall";
            ObjectName[ThremixedIDS::DEZ_GravityPuzzle] = "Obj_DEZ_GravityPuzzle";
            ObjectName[ThremixedIDS::DEZ_Miniboss] = "Obj_DEZ_Miniboss";
            ObjectName[ThremixedIDS::DEZ_EndBoss] = "Obj_DEZ_EndBoss";
            ObjectName[ThremixedIDS::DeathEgg_LaunchTowers] = "Obj_DeathEgg_LaunchTowers";

            ObjectName[ThremixedIDS::DDZ_BossShip] = "Obj_DDZ_BossShip";
            ObjectName[ThremixedIDS::DDZ_Asteroid] = "Obj_DDZ_Asteroid";
            ObjectName[ThremixedIDS::DDZ_Missile] = "Obj_DDZ_Missile";

            ObjectName[ThremixedIDS::CGZ_TriangularSprings] = "Obj_CGZ_TriangularSprings";
            ObjectName[ThremixedIDS::CGZ_BladePlatform] = "Obj_CGZ_BladePlatform";

            ObjectName[ThremixedIDS::BPZ_ElephantBlock] = "Obj_BPZ_ElephantBlock";
            ObjectName[ThremixedIDS::BPZ_Balloon] = "Obj_BPZ_Balloon";

            ObjectName[ThremixedIDS::DPZ_DisolvingSandBar] = "Obj_DPZ_DisolvingSandBar";
            ObjectName[ThremixedIDS::DPZ_Button] = "Obj_DPZ_Button";

            ObjectName[ThremixedIDS::EMZ_Dripper] = "Obj_EMZ_Dripper";

            ObjectName[ThremixedIDS::SS_EntryRing] = "Obj_SS_EntryRing";
            ObjectName[ThremixedIDS::SS_GumballLevel] = "Obj_SS_GumballLevel";
            ObjectName[ThremixedIDS::SS_GumballSideSpring] = "Obj_SS_GumballSideSpring";

            ObjectName[ThremixedIDS::Pachinko_TriangleBumper] = "Obj_Pachinko_TriangleBumper";
            ObjectName[ThremixedIDS::Pachinko_Flipper] = "Obj_Pachinko_Flipper";
            ObjectName[ThremixedIDS::Pachinko_EnergyTrap] = "Obj_Pachinko_EnergyTrap";
            ObjectName[ThremixedIDS::Pachinko_InvisibleUnknown] = "Obj_Pachinko_InvisibleUnknown";
            ObjectName[ThremixedIDS::Pachinko_Platform] = "Obj_Pachinko_Platform";
            ObjectName[ThremixedIDS::Pachinko_ItemOrb] = "Obj_Pachinko_ItemOrb";
        }

        IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Object Name Assignments", (SDL_GetTicks() - startTime) / 1000.0);
        startTime = SDL_GetTicks();

        if (!Player) {
            Player = new IPlayer();
            Player->G = G;
            Player->App = App;
            Player->Scene = this;
			Player->Character = (CharacterType)(SaveGame::CurrentCharacterFlag & 0xF);
            Player->PlayerID = 0;
			Player->Thremixed = Thremixed;
            Player->Create();
            Player->Lives = SaveGame::GetLives();

            Players[0] = Player;

            PlayerCount = 1;

			if (SaveGame::CurrentCharacterFlag >> 4) {
				Players[1] = new IPlayer();
				Players[1]->G = G;
				Players[1]->App = App;
				Players[1]->Scene = this;
				Players[1]->Sidekick = true;
				Players[1]->Character = (CharacterType)(SaveGame::CurrentCharacterFlag >> 4);
				Players[1]->PlayerID = 1;
				Player->Thremixed = Thremixed;
				Players[1]->Create();

				PlayerCount = 2;
			}

            IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Player Creation", (SDL_GetTicks() - startTime) / 1000.0);
            startTime = SDL_GetTicks();
        }

        Data = new SceneData();
    }

    if (AlreadyLoaded) App->Print(2, "Already loaded %s Act %d! Skipping!!!", LevelNameDiscord, Act);

    if (AlreadyLoaded) return;

    startTime = SDL_GetTicks();

    TileSprite = new ISprite(Str_TileSprite, App);

	if (Str_AnimatedSprites) {
		AnimTileSprite = new ISprite(Str_AnimatedSprites, App);
		AnimTileSprite->LinkPalette(TileSprite);
	}

	if (!Thremixed) {
		ItemsSprite->LinkPalette(TileSprite);
		ExplosionSprite->LinkPalette(TileSprite);
		ObjectsSprite->LinkPalette(TileSprite);
	}

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "TileSprite loading", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

    ISprite::Animation an;
    an.Name = NULL;
    an.FrameCount = 0x400;
    an.Frames = (ISprite::AnimFrame*)malloc(0x400 * sizeof(ISprite::AnimFrame));
	if (TileSprite->Width > 16) {
		for (int i = 0; i < 0x400; i++) {
			ISprite::AnimFrame ts_af;
			ts_af.X = (i & 0x1F) << 4;
			ts_af.Y = (i >> 5) << 4;
			ts_af.W = ts_af.H = 16;
			ts_af.OffX = ts_af.OffY = -8;
			an.Frames[i] = ts_af;
			G->MakeFrameBufferID(TileSprite, an.Frames + i);
		}
	}
	else {
		for (int i = 0; i < 0x400; i++) {
			ISprite::AnimFrame ts_af;
			ts_af.X = 0;
			ts_af.Y = i << 4;
			ts_af.W = ts_af.H = 16;
			ts_af.OffX = ts_af.OffY = -8;
			an.Frames[i] = ts_af;
			G->MakeFrameBufferID(TileSprite, an.Frames + i);
		}
	}
    TileSprite->Animations.push_back(an);

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "TileSprite frame buffering", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

    // Loading TileConfig
    IResource* TileConfig = IResources::Load(Str_TileConfigBin); // Stages/MSZ/TileConfig.bin
    if (TileConfig) {
        IStreamer tileReader(TileConfig);
        free(tileReader.ReadByte4()); // Magic Word

        unsigned char* TileData = tileReader.ReadCompressed();
        IResources::Close(TileConfig);

		Data->tiles1 = (TileCfg*)malloc(0x400 * sizeof(TileCfg));
		Data->tiles2 = (TileCfg*)malloc(0x400 * sizeof(TileCfg));

        // Amount of bytes per Tile Definition
        int maxDataBytes = 0x26;
        // Collision Plane 1
        for (int i = 0; i < 0x400; i++) {
            int n = i * maxDataBytes;

            memcpy(Data->tiles1[i].Collision, TileData + n + 0x00, 0x10);
            memcpy(Data->tiles1[i].HasCollision, TileData + n + 0x10, 0x10);
            Data->tiles1[i].IsCeiling = TileData[n + 0x20];
            memcpy(Data->tiles1[i].Config, TileData + n + 0x21, 0x05);
        }
        // Collision Plane 2
        for (int i = 0; i < 0x400; i++) {
            int n = (i + 0x400) * maxDataBytes;

            memcpy(Data->tiles2[i].Collision, TileData + n + 0x00, 0x10);
            memcpy(Data->tiles2[i].HasCollision, TileData + n + 0x10, 0x10);
            Data->tiles2[i].IsCeiling = TileData[n + 0x20];
            memcpy(Data->tiles2[i].Config, TileData + n + 0x21, 0x05);
        }

        free(TileData);
    }
    else {
        App->Print(2, "TileConfig at '%s' could not be read.", Str_TileConfigBin);
        exit(1);
    }

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "TileConfig loading", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

	// Loading SceneBin
    IResource* SceneBin = IResources::Load(Str_SceneBin); // Stages/MSZ/Scene2.bin
    if (SceneBin) {
        IStreamer reader(SceneBin);
        uint32_t mag = reader.ReadUInt32BE(); // magic
        free(reader.ReadBytes(16));
    	free(reader.ReadRSDKString());

    	Data->cameraLayer = reader.ReadByte();
        Data->cameraLayer = -1;
        Data->layerCount = reader.ReadByte();
        for (int i = 0; i < Data->layerCount; i++) {
            reader.ReadByte(); // Ignored Byte
            char* Name = reader.ReadRSDKString();

			memset(Data->layers[i].Name, 0, 50);
            strcpy(Data->layers[i].Name, Name);
            free(Name);

            Data->layers[i].IsScrollingVertical = reader.ReadByte() == 1 ? true : false;
            Data->layers[i].Flags = reader.ReadByte();
			if (Data->layers[i].Flags & 0x10)
				Data->layers[i].Visible = false;

            Data->layers[i].Deform = (int8_t*)calloc(1, App->HEIGHT);

            int   Width = (int)reader.ReadUInt16();
            int   Height = (int)reader.ReadUInt16();

            Data->layers[i].RelativeY = reader.ReadUInt16();
            Data->layers[i].ConstantY = (short)reader.ReadUInt16();

            Data->layers[i].InfoCount = (int)reader.ReadUInt16();

			if (Data->layers[i].InfoCount)
				Data->layers[i].Info = (ScrollingInfo*)malloc(Data->layers[i].InfoCount * sizeof(ScrollingInfo));

			App->Print(3, "Layer %d (%s): Width (%d) Height (%d) Infos (%d) Vertical Scrolling (%d) UnknownFlags (%d) %s", i, Data->layers[i].Name, Width, Height, Data->layers[i].InfoCount, Data->layers[i].IsScrollingVertical, Data->layers[i].Flags, i == Data->cameraLayer ? " IS CAMERA LAYER" : "");

            for (int g = 0; g < Data->layers[i].InfoCount; g++) {
                Data->layers[i].Info[g].RelativeX = reader.ReadUInt16(); // actually is Scrolling Multiplier X
                Data->layers[i].Info[g].ConstantX = (short)reader.ReadUInt16(); // actually is Constant movement X

                Data->layers[i].Info[g].HeatWaveEnabled = reader.ReadByte();
                Data->layers[i].Info[g].Byte2 = reader.ReadByte();

                if (Data->cameraLayer == -1) {
                    if (Data->layers[i].RelativeY == 0x100 &&
                        Data->layers[i].ConstantY == 0x000 &&
                        Data->layers[i].Info[g].RelativeX == 0x100 &&
                        Data->layers[i].Info[g].ConstantX == 0x000 &&
                        Data->layers[i].Name[0] == 'F' &&
                        Data->layers[i].InfoCount == 1)
                    Data->cameraLayer = i;
                }

                //App->Print(0, " > ScrollInfo %d: RelX (%X) ConstX (%X) %d %d", g, Data->layers[i].Info[g].RelativeX, Data->layers[i].Info[g].ConstantX, Data->layers[i].Info[g].HeatWaveEnabled, Data->layers[i].Info[g].Byte2);
            }

			if ((mag >> 24) == 'S') {
				Data->layers[i].Flags = 0;
				if (Data->layers[i].RelativeY == 0x100 &&
					Data->layers[i].ConstantY == 0x000 &&
					Data->layers[i].Info[0].RelativeX == 0x100 &&
					Data->layers[i].Info[0].ConstantX == 0x000 &&
					Data->layers[i].Name[0] == 'F' &&
					Data->layers[i].InfoCount == 1)
					Data->layers[i].Flags = 1;
			}

            // FUN FACT:
            // With all compressed data, we already know the size of the data. (using Width, Height, etc.)

            unsigned char* ScrollIndexes = reader.ReadCompressed();

			int cnt = 0;
            if (Height > 0) { // Just in case
                int s_start = 0;
                int value = ScrollIndexes[0];
                int s = 0;
                for (; s < Height * 16; s++) {
                    int v = ScrollIndexes[s];
                    if (v != value) {
                        cnt++;
                        s_start = s;
                        value = v;
                    }
                }
                cnt++;
                Data->layers[i].ScrollIndexCount = cnt;
            }

            Data->layers[i].ScrollIndexes = (ScrollingIndex*)calloc(cnt, sizeof(ScrollingIndex));

            if (Height > 0) { // Just in case
                int sc = 0;
                int s_start = 0;
                int value = ScrollIndexes[0];
                int s = 0;
                for (; s < Height * 16; s++) {
					// ScrollingIndex
					int v = ScrollIndexes[s];
                    // If we find a different value, cap it off and start anew.
                    if (v != value) {
                        Data->layers[i].ScrollIndexes[sc].Index = value;
                        Data->layers[i].ScrollIndexes[sc].Size = s - s_start;
                        //App->Print(0, " - ScrollIndex %d: Index %d Size %d", sc, Data->layers[i].ScrollIndexes[sc].Index, Data->layers[i].ScrollIndexes[sc].Size);

                        sc++;
                        s_start = s;
                        value = v;
                    }
                }
                // Data->layers[i].ScrollIndexes[sc] = ScrollingIndex { value, s - s_start, NULL };
                Data->layers[i].ScrollIndexes[sc].Index = value;
                Data->layers[i].ScrollIndexes[sc].Size = s - s_start;
				//App->Print(0, " - ScrollIndex %d: Index %d Size %d", sc, Data->layers[i].ScrollIndexes[sc].Index, Data->layers[i].ScrollIndexes[sc].Size);
            }
            free(ScrollIndexes);

            Data->layers[i].Width = Width;
    		Data->layers[i].Height = Height;
            unsigned char* Tilesss = reader.ReadCompressed();

            Data->layers[i].Tiles = (short*)malloc(Width * Height * sizeof(short));
            Data->layers[i].TilesBackup = (short*)malloc(Width * Height * sizeof(short));
            Data->layers[i].TileOffsetY = (short*)calloc(sizeof(short), Width);

            IStreamer creader(Tilesss);
            for (int y = 0; y < Height; y++) {
                for (int x = 0; x < Width; x++) {
                    Data->layers[i].Tiles[x + y * Width] = creader.ReadUInt16();
                }
            }

            PatchLayer(i);

            memcpy(Data->layers[i].TilesBackup, Data->layers[i].Tiles, Width * Height * sizeof(short));
            free(Tilesss);
        }

        enum {
            OBJ_PLAYER = 9,
            OBJ_RING = 13,
            OBJ_MONITOR = 14,
            OBJ_SPRING = 18,
            OBJ_STARPOLE = 19,
            OBJ_SPIKES = 21,
            OBJ_PLANESWITCHER = 22,
            OBJ_SPECIALRING = 33,
        };

        PlaneSwitchers = (PlaneSwitch*)malloc(113 * sizeof(PlaneSwitch));

		Data->isAnims = (short*)malloc(0x400 * sizeof(short));
		memset(Data->isAnims, 0xFF, 0x400 * sizeof(short));

        // Mania-type Object Loading
        if ((mag >> 24) == 'S') {
			BlankTile = 0x3FF;

            unordered_map<string, const char*> ObjectHashes;
            for (int i = 0; i < 554; i++) {
                string hash = MD5I(string(ObjectNames(i)));
                ObjectHashes[hash] = ObjectNames(i);
            }

            Data->objectDefinitionCount = reader.ReadByte();
            for (int i = 0; i < Data->objectDefinitionCount; i++) {
                char str[16];
                char hashString[33];
                sprintf(hashString, "%s", "");

                for (int n = 0; n < 16; n++) {
                    str[n] = reader.ReadByte();
                    sprintf(hashString, "%s%02x", hashString, (unsigned char)str[n]);
                }

                int ArgumentCount = reader.ReadByte();

                int ArgumentTypes[0xFF];
                ArgumentTypes[0] = 0;

                for (int n = 1; n < ArgumentCount; n++) {
                    //App->Print(0, "Argument %d\n", n);

                    ///*

                    //App->Print(0, "IMPORTANT: Argument Hash: ");
                    char str2[17];
                    for (int n = 0; n < 16; n++) {
                        str2[n] = reader.ReadByte();
                        //printf("%02X ", (unsigned char)str2[n]);
                        fflush(stdout);
                    }
                    //// printf("\n");
                    fflush(stdout);
                    str2[16] = 0;
                    str2[5] = str2[5];

                    //*/


                    int ArgType = reader.ReadByte();
                    ArgumentTypes[n] = ArgType;
                }

                int EntityCount = reader.ReadUInt16();


				if (i == OBJ_SPRING) {
					App->Print(2, "Object #%d (%s) Hash: %s Count: %d", i, ObjectHashes[hashString], hashString, EntityCount);
					for (int n = 0; n < ArgumentCount; n++) {
						App->Print(0, "Argument %d type: %02X", n, ArgumentTypes[n]);
					}
				}
                if (i == OBJ_PLANESWITCHER)
                    PlaneSwitchCount = 0;

                for (int n = 0; n < EntityCount; n++) {
                    //int SlotID = reader.ReadUInt16();
                    reader.ReadUInt16();

                    unsigned int X1 = reader.ReadUInt16();
                    unsigned int X2 = reader.ReadUInt16();
                    unsigned int Y1 = reader.ReadUInt16();
                    unsigned int Y2 = reader.ReadUInt16();
                    //App->Print(0, "Entity %d (%d) %04X %04X, %04X %04X     %d   %d\n", n, SlotID, X1, X2, Y1, Y2, PlaneSwitchCount, i);

                    int* args = (int*)calloc(sizeof(int), ArgumentCount);
                    if (ArgumentCount > 1) {
                        for (int a = 1; a < ArgumentCount; a++) {
                            int value = 0;
                            switch (ArgumentTypes[a]) {
                                case 0x0: // Byte
                                case 0x3:
                                    value = (unsigned char)reader.ReadByte();//App->Print(0, "   Argument Value (Type: %02X): %02X\n", ArgumentTypes[a], (unsigned char)reader.ReadByte());
                                    break;
                                case 0x1:
                                case 0x4:
                                    value = (unsigned short)reader.ReadUInt16();//App->Print(0, "   Argument Value (Type: %02X): %04X\n", ArgumentTypes[a], (unsigned short)reader.ReadUInt16());
                                    break;
                                case 0x2:
                                case 0x5:
                                case 0x6: // Var
                                case 0x7: // Bool
                                case 0xB: // Color (Uint32)
                                    value = (unsigned int)reader.ReadUInt32();//App->Print(0, "   Argument Value (Type: %02X): %08X\n", ArgumentTypes[a], (unsigned int)reader.ReadUInt32());
                                    break;
                                case 0x8: {
                                    int WordCount = reader.ReadUInt16();
                                    //App->Print(0, "   Argument Value (Type: %02X): %04X\n", ArgumentTypes[a], WordCount);
                                    for (int wc = 0; wc < WordCount; wc++) {
                                        reader.ReadUInt16();//App->Print(0, "      Value (Type: %02X): %04X\n", ArgumentTypes[a], (unsigned short)reader.ReadUInt16());
                                    }
                                }
                                    break;
                                case 0x9:
                                    value = (unsigned int)reader.ReadUInt32();
            						reader.ReadUInt32();//App->Print(0, "   Argument Value (Type: %02X): %08X %08X\n", ArgumentTypes[a], (unsigned int)reader.ReadUInt32(), (unsigned int)reader.ReadUInt32());
                                    break;
                            }
                            args[a - 1] = value;
                        }
                    }

                    if (i == OBJ_PLANESWITCHER) {
                        PlaneSwitchers[PlaneSwitchCount].X = X2 + (X1 >> 16);
                        PlaneSwitchers[PlaneSwitchCount].Y = Y2 + (Y1 >> 16);

                        PlaneSwitchers[PlaneSwitchCount].Flags = args[0];
                        PlaneSwitchers[PlaneSwitchCount].Size = args[1];
                        PlaneSwitchers[PlaneSwitchCount].Angle = args[2];
                        PlaneSwitchers[PlaneSwitchCount].OnPath = args[3] == 1;
                        PlaneSwitchCount++;
                    }
                    else if (i == OBJ_PLAYER && args[0] == CharacterFlag) {
                        PlayerStartX = X2 + (X1 >> 16);
                        PlayerStartY = Y2 + (Y1 >> 16) - 4;
                    }
                    else if (i == OBJ_SPRING) {
                        int ID = 0x07;
                        int X = X2;
                        int Y = Y2;
                        int SubType = 0x00;
                        if ((args[0] & 0x1) == 0)
                            SubType |= 0x02;

						int ttty = args[0] >> 1 & 0x3;
                        bool FLIPX = (args[1] >> 0) & 1;
                        bool FLIPY = (args[1] >> 1) & 1;
                        bool PRIORITY = false;

						if (ttty == 0) {
							if (FLIPY)
								SubType |= 0x20;
							else
								SubType |= 0x0;
						}
						else if (ttty == 1) {
							SubType |= 0x10;
						}
						else if (ttty == 2) {
							if (!FLIPY)
								SubType |= 0x30;
							else
								SubType |= 0x40;
						}

                        ADD_OBJECT();
                    }
                    else if (i == OBJ_SPECIALRING) {
                        int ID = 0x85;
                        int X = X2;
                        int Y = Y2;
                        int SubType = args[0];
                        bool FLIPX = false;
                        bool FLIPY = false;
                        bool PRIORITY = false;

                        ADD_OBJECT();
                    }
                    else if (i == OBJ_SPIKES) {
                        int ID = 0x08;
                        int X = X2;
                        int Y = Y2;
                        int SubType = 0x00;
                        bool FLIPX = false;
                        bool FLIPY = false;
                        bool PRIORITY = false;

                        if (args[1] == 1)
                            SubType |= 0x02;
                        if (args[0] == 2) {
                            SubType |= 0x40;
                            FLIPX = true;
                        }

                        ADD_OBJECT();
                    }
                    else if (i == OBJ_STARPOLE) {
                        int ID = Obj_StarPost;
                        int X = X2;
                        int Y = Y2;
                        int SubType = args[0];

                        bool FLIPX = false;
                        bool FLIPY = false;
                        bool PRIORITY = false;

                        ADD_OBJECT();
                    }
                    else if (i == OBJ_RING) {
                        ObjectProp op;
                        op.X = X2;
                        op.Y = Y2;
                        op.ID = 0xFF;
                        op.LoadFlag = true;

                        RingProps[RingPropCount++] = op;
                    }
                }
            }
        }
        // ImpostorEngine2-type Loading
        else if ((mag >> 24) == 'U') {
			BlankTile = 0;

            reader.ReadByte(); // to get rid of the leftover "ObjectCount"
            int pX = reader.ReadUInt16();
            int pY = reader.ReadUInt16();
            if (PlayerStartX == -1) { // && Not Playing as Knuckles
                PlayerStartX = pX;
                PlayerStartY = pY;
            }
            pX = reader.ReadUInt16();
            pY = reader.ReadUInt16();
            if (PlayerStartX == -1 && false) { // && Playing as Knuckles
                PlayerStartX = pX;
                PlayerStartY = pY;
            }

            int objectCount = reader.ReadUInt16();
            unsigned char ObjectListUnimpl[0x100];
            memset(ObjectListUnimpl, 0, 0x100);
            for (int o = 0; o < objectCount; o++) {
                int X = reader.ReadUInt16BE();
                int Y = reader.ReadUInt16BE();

                int data = (Y & 0xF000) / 0x1000;
                int PRIORITY = (data >> 3) & 0x01;
                int FLIPX = (data >> 1) & 0x01;
                int FLIPY = (data >> 2) & 0x01;
                int ID = reader.ReadByte();
                int SubType = reader.ReadByte();
                Y &= 0x0FFF;

                if (ID == 0x02) {
                    int H = (4 << (SubType & 0x03)) - 1;

                    int groundOnly = (SubType >> 7) & 0x01;
                    int orientation = (SubType >> 2) & 0x01;

                    int leftUpPath = ((SubType >> 4) & 0x01);
                    int rightDownPath = ((SubType >> 3) & 0x01);
                    int leftUpPriority = ((SubType >> 6) & 0x01); //1 is high, 0 is low
                    int rightDownPriority = ((SubType >> 5) & 0x01);

                    PlaneSwitchers[PlaneSwitchCount].X = X;
                    PlaneSwitchers[PlaneSwitchCount].Y = Y;

                    PlaneSwitchers[PlaneSwitchCount].Flags = rightDownPath << 3 | rightDownPriority << 2 | leftUpPath << 1 | leftUpPriority;
                    PlaneSwitchers[PlaneSwitchCount].Size = H;
                    PlaneSwitchers[PlaneSwitchCount].Angle = orientation * 0xC0;
                    PlaneSwitchers[PlaneSwitchCount].OnPath = groundOnly == 1;
                    PlaneSwitchCount++;
                }
                else {
                    ObjectProp op;
                    op.X = X;
                    op.Y = Y;
                    op.ID = ID;
                    op.SubType = SubType;
                    op.LoadFlag = PRIORITY;
                    op.FlipX = FLIPX;
                    op.FlipY = FLIPY;

                    ObjectProps[ObjectPropCount++] = op;

                    Object* obj = GetNewObjectFromID(ID);
                    if (obj) {
                        obj->G = G;
                        obj->App = App;
                        obj->Scene = this;
                        obj->InitialX = X;
                        obj->InitialY = Y;
                        obj->FlipX = FLIPX == 1;
                        obj->FlipY = FLIPY == 1;
                        obj->ID = ID;

                        while (!SpriteMapIDs[ID])
                            ID--;

                        obj->Sprite = SpriteMapIDs[ID];
                        obj->SubType = SubType;
                        Objects[ObjectCount++] = obj;
                    }
                    else {
                        ObjectListUnimpl[ID] = 0xFF;
                    }
                }
            }
            for (int i = 0; i < 0x100; i++) {
                if (ObjectListUnimpl[i] != 0)
                    App->Print(1, "Object %02X (%s) has not been implemented!", i, ObjectName[i] ? ObjectName[i] : "(null)");
            }

            int ringCount = reader.ReadUInt16();
            for (int o = 0; o < ringCount; o++) {
                int X = reader.ReadUInt16BE();
                int Y = reader.ReadUInt16BE();

                ObjectProp op;
                op.X = X;
                op.Y = Y;
                op.ID = 0xFF;
                op.LoadFlag = true;

                RingProps[RingPropCount++] = op;
            }

            int animTilesCount = reader.ReadUInt16();
            vector<int> tils;
            for (int o = 0; o < animTilesCount; o++) {
                int til = reader.ReadUInt16();
                int row = reader.ReadByte();
                int whichanim = reader.ReadByte();
                Data->isAnims[til] = row << 8 | whichanim;

                tils.push_back(Data->isAnims[til]);
            }

            Data->animatedTilesCount = reader.ReadUInt16();

            Data->animatedTileFrames = (int*)calloc(Data->animatedTilesCount, sizeof(int));

            Data->animatedTileDurations = (int**)calloc(Data->animatedTilesCount, sizeof(int*));
            for (int o = 0; o < Data->animatedTilesCount; o++) {
                int framecount = reader.ReadUInt16();
                Data->animatedTileDurations[o] = (int*)calloc(framecount + 2, sizeof(int));
                Data->animatedTileDurations[o][0] = framecount;

                int sum = 0;
                for (int g = 1; g < framecount + 1; g++) {
                    Data->animatedTileDurations[o][g] = reader.ReadByte();
                    sum += Data->animatedTileDurations[o][g];
                }
                Data->animatedTileDurations[o][framecount + 1] = sum;
            }

            for (int o = 0; o < animTilesCount; o++) {
                int framecount = Data->animatedTileDurations[tils[o] & 0xFF][0];

                if (!framecount) {
                    App->Print(2, "Encountered a frame count of 0 on anim tile %d out of %d!", o, animTilesCount);
                    exit(1);
                }

                ISprite::Animation an;
                an.Name = NULL;
                an.FrameCount = framecount;
                an.Frames = (ISprite::AnimFrame*)malloc(framecount * sizeof(ISprite::AnimFrame));
                for (int i = 0; i < framecount; i++) {
                    ISprite::AnimFrame ts_af;
                    ts_af.X = i << 4;
                    ts_af.Y = o << 4;
                    ts_af.W = ts_af.H = 16;
                    ts_af.OffX = ts_af.OffY = -8;
                    an.Frames[i] = ts_af;
                    G->MakeFrameBufferID(AnimTileSprite, an.Frames + i);
                }
                AnimTileSprite->Animations.push_back(an);
            }
        }

		for (int i = 0; i < Data->layerCount; i++) {
			// Build buffers for GL renderer
			if (Data->layers[i].InfoCount) {
				int y = 0;
				int siT, x;
				int tile = 0, flipY = 0, flags = 0;//, wheree = 0;
				int heightSize = 0, tilindx = 0, tilindy = 0, tilindpos = 0, word = 0;
				Layer layer = Data->layers[i];

				int bufHeight = 0;
				for (int s = 0; s < layer.ScrollIndexCount; s++) {
					for (siT = 0; siT < layer.ScrollIndexes[s].Size; siT += heightSize) {
						heightSize = 16;
						if (heightSize > layer.ScrollIndexes[s].Size - siT)
							heightSize = layer.ScrollIndexes[s].Size - siT;
						if (heightSize >((y + siT + 0x10) & ~0xF) - (y + siT)) // Rounded-up
							heightSize = ((y + siT + 0x10) & ~0xF) - (y + siT);

						bufHeight++;
					}
					y += layer.ScrollIndexes[s].Size;
				}

				y = 0;
				int buf = 0;
				int fullFlip;
				// layer.Width * bufHeight
				layer.ScrollIndexes[0].TileBuffers = (int*)calloc(bufHeight, sizeof(int));
				for (int s = 0; s < layer.ScrollIndexCount; s++) {
					for (siT = 0; siT < layer.ScrollIndexes[s].Size; siT += heightSize) {
						heightSize = 16;
						if (heightSize > layer.ScrollIndexes[s].Size - siT)
							heightSize = layer.ScrollIndexes[s].Size - siT;
						if (heightSize >((y + siT + 0x10) & ~0xF) - (y + siT)) // Rounded-up
							heightSize = ((y + siT + 0x10) & ~0xF) - (y + siT);

						G->BeginSpriteListBuffer();

						for (x = 0; x < layer.Width; x++) {
							tilindy = ((y + siT) >> 4);
							tilindx = x;
							tilindpos = x + (tilindy)* layer.Width;

							word = layer.Tiles[tilindpos];
							fullFlip = ((word >> 10) & 3);
							flipY = ((word >> 11) & 1);
							tile = word & 0x3FF;

							if (tile != BlankTile) {
								int anID = Data->isAnims[tile] & 0xFF;

								flags = 0;
								if (flipY)
									flags |= IE_FLIPY;

								int wheree = (y + siT) & 0xF;
								if (flipY)
									wheree = 0x10 - wheree - heightSize;

								if (anID == 0xFF) {
									/*
									// Data->layers[i].ScrollIndexes[0].TileBuffers[x + buf * layer.Width] =
									G->MakeFrameBufferID(TileSprite, &Data->layers[i].ScrollIndexes[0].TileBuffers[x + buf * layer.Width], ((tile & 0x1F) << 4), ((tile >> 5) << 4) + wheree, 16, heightSize, -8, -heightSize / 2, fullFlip);
									//*/
									if (TileSprite->Width > 16)
										G->AddToSpriteListBuffer(TileSprite, ((tile & 0x1F) << 4), ((tile >> 5) << 4) + wheree, 16, heightSize, -8 + (x << 4), -heightSize / 2, fullFlip);
									else
										G->AddToSpriteListBuffer(TileSprite, 0, (tile << 4) + wheree, 16, heightSize, -8 + (x << 4), -heightSize / 2, fullFlip);
								}
							}
						}

						Data->layers[i].ScrollIndexes[0].TileBuffers[buf] = G->FinishSpriteListBuffer();
						buf++;
					}

					y += layer.ScrollIndexes[s].Size;
				}
			}
		}

        IResources::Close(SceneBin);
    }
    else {
        App->Print(2, "Scene.bin at '%s' could not be read.", Str_SceneBin);
        exit(1);
    }

    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Scene loading", (SDL_GetTicks() - startTime) / 1000.0);
    startTime = SDL_GetTicks();

    // Loading StageConfig
    if (Str_StageBin) {
        App->Print(0, "Loading StageConfig...");
        IResource* StageBin = IResources::Load(Str_StageBin); // Stages/MSZ/StageConfig.bin
        if (StageBin) {
            IStreamer stageReader(StageBin);
            free(stageReader.ReadByte4()); // Magic CFG0
            stageReader.ReadByte(); // UseGameObjects

            // Read objects names
            int object_count = stageReader.ReadByte();
            for (int i = 0; i < object_count; i++) {
                free(stageReader.ReadRSDKString()); // Object name
            }

            // Read palette
            int pp = 0;
            for (int i = 0; i < 8; i++) {
                int bitmap = stageReader.ReadUInt16();
                for (int col = 0; col < 16; col++) {
                    if ((bitmap & (1 << col)) != 0) {
                        for (int d = 0; d < 16; d++) {
                            uint8_t R = stageReader.ReadByte();
                            uint8_t G = stageReader.ReadByte();
                            uint8_t B = stageReader.ReadByte();

                            if (i >= 0) {
                                if (pp < 256)
                                    TileSprite->SetPaletteAlt(pp, R << 16 | G << 8 | B);
                                pp++;
                            }
                        }
                    }
                }
            }

            /*
            FILE* f;
            f = fopen(IFILE("Stages/HCZ/ManiaPalette.bin"), "wb");
            fwrite(TileSprite->Palette + 128, 4, 128, f);
            fwrite(TileSprite->PaletteAlt + 128, 4, 128, f);
            fwrite(TileSprite->PaletteAlt, 4, 96, f);
            fclose(f);
            //*/

            // Read WAV channel's max concurrent play
            int wavs_count = stageReader.ReadByte();
            for (int i = 0; i < wavs_count; i++) {
                free(stageReader.ReadRSDKString()); // WAV name
                stageReader.ReadByte(); // Max Concurrent Play
                //App->Print(0, "WAV: '%s' (max: %d)", wav_name, max_concurrent_play);
            }

            IResources::Close(StageBin);
        }
        else {
            App->Print(2, "StageConfig at '%s' could not be read.", Str_StageBin);
            exit(1);
        }
    }
    else {
        if ((TileSprite->GetPalette(0x81) & 0xFFFFFF) == 0x000000)
            TileSprite->PaletteSize = 0x80;
        TileSprite->SplitPalette();
        TileSprite->UpdatePalette();

		if (AnimTileSprite) {
			if ((AnimTileSprite->GetPalette(0x81) & 0xFFFFFF) == 0x000000)
				AnimTileSprite->PaletteSize = 0x80;
			AnimTileSprite->SplitPalette();
			AnimTileSprite->UpdatePalette();
		}

        ItemsSprite->SplitPalette();
        ObjectsSprite->SplitPalette();
        ExplosionSprite->SplitPalette();

        ItemsSprite->UpdatePalette();
        ObjectsSprite->UpdatePalette();
        ExplosionSprite->UpdatePalette();
    }

    startTime = SDL_GetTicks();
}

PUBLIC VIRTUAL void LevelScene::Init() {
    LoadData();

    uint64_t startTime = SDL_GetTicks();
    RestartStage(true, false);
    IApp::Print(-1, "LevelScene \"%s\" took %0.3fs to run.", "Init RestartStage()", (SDL_GetTicks() - startTime) / 1000.0);

    switch (ZoneID) {
        case 2:
            if (Act == 2) {
                int X = 0x40C8;
                int Y = 0x7D0;
                int ID = 0x81;
                int SubType = 0;
                int PRIORITY = true;
                int FLIPX = 0;
                int FLIPY = 0;
                ADD_OBJECT();
                obj->Create();
            }
            break;
        case 3:
            if (Act == 1) {
                int X = 0x2F00;
                int Y = 0xE60;
                int ID = 0x81;
                int SubType = 0;
                int PRIORITY = true;
                int FLIPX = 0;
                int FLIPY = 0;
                ADD_OBJECT();
                obj->Create();
            } else {
                int X = 0x3D00;
                int Y = 0x720;
                int ID = 0x81;
                int SubType = 0;
                int PRIORITY = true;
                int FLIPX = 0;
                int FLIPY = 0;
                ADD_OBJECT();
                obj->Create();
            }
            break;
        case 4:
            if (Act == 1) {
                int X = 0x32C0;
                int Y = 0x20C;
                int ID = 0x81;
                int SubType = 0;
                int PRIORITY = true;
                int FLIPX = 0;
                int FLIPY = 1;
                ADD_OBJECT();
                obj->Create();
            } else {
                int X = 0x4940;
                int Y = 0x270;
                int ID = 0x81;
                int SubType = 0;
                int PRIORITY = true;
                int FLIPX = 0;
                int FLIPY = 1;
                ADD_OBJECT();
                obj->Create();
            }
            break;
		case 5:
			if (Act == 1) {
				int X = 0x6540;
				int Y = 0x05A0;
				int ID = 0x81;
				int SubType = 0;
				int PRIORITY = true;
				int FLIPX = 0;
				int FLIPY = 0;
				ADD_OBJECT();
				obj->Create();
			} else {
				int X = 0x4590;
				int Y = 0x0680;
				int ID = 0x81;
				int SubType = 0;
				int PRIORITY = true;
				int FLIPX = 0;
				int FLIPY = 0;
				ADD_OBJECT();
				obj->Create();
			}
			break;
    }
}

PUBLIC STATIC  int  LevelScene::LoadStatic(void* data) {
    LevelScene* self = (LevelScene*)data;
    self->LoadData();
    return 0;
}

PUBLIC void LevelScene::LoadInBackground() {
    SDL_DetachThread(SDL_CreateThread(LevelScene::LoadStatic, "LevelScene::LoadInBackground", this));
}

PUBLIC VIRTUAL void LevelScene::RestartStage(bool doActTransition, bool drawBackground) {
	if (Sound::SoundBank[0]) {
		App->Audio->ClearMusic();
		App->Audio->PushMusic(Sound::SoundBank[0], true, Sound::Audio->LoopPoint[0]);
	}

	SaveGame::SetLives(Player->Lives);
	SaveGame::SetZone(ZoneID - 1);
	SaveGame::Flush();

    for (int i = 0; i < ObjectSolidCount; i++)
        ObjectsSolid[i] = NULL;
    for (int i = 0; i < ObjectSpringCount; i++)
        ObjectsSpring[i] = NULL;
    for (int i = 0; i < ObjectEnemiesCount; i++)
        ObjectsEnemies[i] = NULL;
    for (int i = 0; i < ObjectBreakableCount; i++)
        ObjectsBreakable[i] = NULL;

    DoneSpinning = false;

    StopTimer = false;
    ShowResults = false;
	ResultsTimer = 0;
	TimerTotal = 0;
	TotalToAdd = 0;

    ObjectSolidCount = 0;
    ObjectSpringCount = 0;
    ObjectEnemiesCount = 0;
    ObjectBreakableCount = 0;
    ObjectPathSwitcherCount = 0;

    for (int i = 0; i < Data->layerCount; i++)
        memcpy(Data->layers[i].Tiles, Data->layers[i].TilesBackup, Data->layers[i].Width * Data->layers[i].Height * sizeof(short));

    StopTimer = false;

    int pX = PlayerStartX;
    int pY = PlayerStartY;
	if (SavedPalette && SavedPalette[0]) {
		// TileSprite->SetPalette(0, 256, SavedPalette);
	}
    if (SavedPositionX > 0) {
        pX = SavedPositionX;
        pY = SavedPositionY;
    }
    if (SpecialSpawnPositionX > 0) {
        pX = SpecialSpawnPositionX;
        pY = SpecialSpawnPositionY;
        SpecialSpawnPositionX = -1;
    }

    for (int p = 0; p < PlayerCount; p++) {
        IPlayer* Player = Players[p];

        Player->EZX = (pX - p * 16);
        Player->EZY = pY;
        Player->DisplayX = Players[p]->X;
        Player->DisplayY = Players[p]->Y;
        Player->Action = ActionType::Normal;

        Player->GroundSpeed = 0;
        Player->XSpeed = 0;
        Player->YSpeed = 0;
        Player->Flip = 1;
        Player->DisplayFlip = 1;

        Player->ControlLocked = false;
        Player->ObjectControlled = 0x00;

        if (ResetTimer)
            Player->Rings = 0;

        Player->Create();
        Player->LateUpdate();
    }

    if (ResetTimer)
        Timer = 0;

    if (!doActTransition) {
        RoutineNumber = 0x00;
        LevelTriggerFlag = 0x00;
    }

    memset(Signal, 0, sizeof(Signal));
    memset(&PauseAnim[0], 0, 8 * sizeof(int));
    LevelCardTimer = 0.0;

    // Set Camera on player
    CameraX = (Player->EZX + Player->CameraX) - App->WIDTH / 2;
    if (ManiaLevel)
        CameraY = (Player->EZY + Player->CameraY - 4) - App->HEIGHT / 2;
    else
        CameraY = (Player->EZY + Player->CameraY + 8) - App->HEIGHT / 2;

    CameraMinX = 0;
    CameraMinY = 0;
    CameraMaxX = 0x7FFF;
    CameraMaxY = 0x7FFF;
    CameraDeltaX = 0;
    CameraDeltaY = 0;
    CameraAutoScrollX = 0;
    CameraAutoScrollY = 0;

    HandleCamera();

    ObjectCount -= ObjectNewCount;

    for (int i = ObjectCount; i < ObjectCount + ObjectNewCount; i++) {
        delete Objects[i];
    }

    ObjectNewCount = 0;

    for (int i = 0; i < ObjectCount; i++)
        Objects[i]->Create();


    for (int o = 0; o < RingPropCount; o++)
        RingProps[o].ID = 0xFF;

    Explosions.clear();

    UpdateDiscord();
}

PUBLIC VIRTUAL void LevelScene::PatchLayer(int layer) {
    if (ZoneID == 1 && Act == 2 && layer == 1) {
        int Width = Data->layers[layer].Width;
        for (int xx = 0x4; xx < 0xB; xx++) {
            for (int yy = 0x23; yy < 0x28; yy++) {
                Data->layers[layer].Tiles[xx + Width * yy] = 0x5B;
            }
        }
    }
}

PUBLIC VIRTUAL void LevelScene::UpdateDiscord() {
    char imgkey[15];
    char levelname[50];
    sprintf(imgkey, "%d", ZoneID);
    sprintf(levelname, "%s%s%d", LevelNameDiscord, " Act ", VisualAct);
    Discord_UpdatePresence("Classic Mode:", levelname, imgkey);
}

PUBLIC bool LevelScene::CollisionAt(int probeX, int probeY) {
    return CollisionAt(probeX, probeY, NULL, 0, NULL);
}
PUBLIC bool LevelScene::CollisionAt(int probeX, int probeY, int* angle) {
    return CollisionAt(probeX, probeY, angle, 0, NULL);
}
PUBLIC bool LevelScene::CollisionAt(int probeX, int probeY, int* angle, int anglemode) {
    return CollisionAt(probeX, probeY, angle, anglemode, NULL);
}

PUBLIC bool LevelScene::CollisionAtClimbable(int probeX, int probeY, int* angle, int anglemode, IPlayer* player) {
    CollisionCheckForClimbable = true;
    bool gh = CollisionAt(probeX, probeY, angle, anglemode, player);
    CollisionCheckForClimbable = false;
    return gh;
}

PUBLIC VIRTUAL bool LevelScene::CollisionAt(int probeX, int probeY, int* angle, int anglemode, IPlayer* player) {
    if (!Data) return false;

    int tileX = probeX / 16;
    int tileY = probeY / 16;

    // Layer Flags:
    // 0x1: Collidable
    // 0x2: Don't Repeat X
    // 0x3: Don't Repeat Y

    int probeXOG = probeX;
    int probeYOG = probeY;

    for (int l = 0; l < Data->layerCount; l++) {
        if (!(Data->layers[l].Flags & 1)) continue;

        probeX = probeXOG;
        probeY = probeYOG;

        probeX -= Data->layers[l].OffsetX;
        probeY -= Data->layers[l].OffsetY;

        if (probeX < 0)
            continue;
        if (probeX >= Data->layers[l].Width * 16)
            continue;
        if (Data->layers[l].IsScrollingVertical) {
            int hh = Data->layers[l].Height * 16;
            probeY = ((probeY % hh) + hh) % hh;
        }
        else {
            if (probeY < 0)
                continue;
            if (probeY >= Data->layers[l].Height * 16)
                continue;
        }

        tileX = probeX >> 4;
        tileY = probeY >> 4;

        int tileID = Data->layers[l].Tiles[tileX + tileY * Data->layers[l].Width];
        if ((tileID & 0x3FF) != 0x3FF && (tileID & 0x3FF) != 0x0) {
            bool tileData1 = ((tileID >> 15) & 1) == 1;
            bool tileData2 = ((tileID >> 14) & 1) == 1;
            bool tileData3 = ((tileID >> 13) & 1) == 1;
            bool tileData4 = ((tileID >> 12) & 1) == 1;

            bool tileDatas[4] = { tileData4, tileData3, tileData2, tileData1 };

            bool flipX = ((tileID >> 10) & 1) == 1;
            bool flipY = ((tileID >> 11) & 1) == 1;

            tileID = tileID & 0x3FF;

            ///*
            if (player) {
                if (!tileDatas[player->Layer * 2] && !tileDatas[player->Layer * 2 + 1])
                    continue;

                if (player->AngleMode != 0 && !tileDatas[player->Layer * 2] && tileDatas[player->Layer * 2 + 1])
                    continue;
            }
            else {
                if (!tileDatas[Player->Layer * 2] && !tileDatas[Player->Layer * 2 + 1])
                    continue;
            }

            // TODO: When exporting levels check to make sure we don't double jumpthrough blocks

            //*/

            for (int c = 0; c < 16; c++) {
                int eex = c;
                if (flipX)
                    eex = 15 - c;

                int tX = tileX * 16;
                int tY = tileY * 16;

                int h1 = Data->tiles1[tileID].Collision[c];
                int h2 = Data->tiles2[tileID].Collision[c];

				if (BlankTile == 0x3FF && (Data->tiles1[tileID].IsCeiling || Data->tiles2[tileID].IsCeiling)) {
					h1 = 15 - h1;
					h2 = 15 - h2;
				}

                int which = 0;
                if (anglemode == 0)
                    which = 0 + flipY * 3;
                if (anglemode == 2)
                    which = 3 - flipY * 3;

                if (anglemode == 1) {
                    which = 1 + flipX;
                }
                if (anglemode == 3)
                    which = 2 - flipX;

                if (player) {
                    if (Data->tiles1[tileID].HasCollision[c] && player->Layer == 0) {
                        if ((Data->tiles1[tileID].IsCeiling ^ flipY)) {
                            if (!tileDatas[player->Layer * 2 + 1] && (anglemode != 0 || player->EZY < tY))
                                continue;

                            if (probeX == tX + eex &&
                                probeY >= tY &&
                                probeY <  tY + 16 - h1) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles1[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;
                                    //tempAngle = tempAngle * -45.f / 32;
                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }

                                    *angle = tempAngle & 0xFF; //(int)wrapAngle(tempAngle);
                                }

                                player->LastObject = NULL;
                                return true;
                            }
                        }
                        else {
                            if (!tileDatas[player->Layer * 2 + 1] && (anglemode != 0 || player->EZY > tY))
                                continue;

                            if (probeX == tX + eex &&
                                probeY >= tY + h1 &&
                                probeY <  tY + 16) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles1[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;
                                    //tempAngle = tempAngle * -45.f / 32;
                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }

                                    *angle = tempAngle & 0xFF; //(int)wrapAngle(tempAngle);
                                }

                                player->LastObject = NULL;
                                return true;
                            }
                        }
                    }
                    if (Data->tiles2[tileID].HasCollision[c] && player->Layer == 1) {
                        if (Data->tiles2[tileID].IsCeiling ^ flipY) {
                            if (!tileDatas[player->Layer * 2 + 1] && (anglemode != 0 || player->EZY < tY))
                                continue;

                            if (probeX == tX + eex &&
                                probeY >= tY &&
                                probeY <  tY + 16 - h2) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles2[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;
                                    //tempAngle = tempAngle * -45.f / 32;
                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }

                                    *angle = tempAngle & 0xFF; //(int)wrapAngle(tempAngle);
                                }

                                player->LastObject = NULL;
                                return true;
                            }
                        }
                        else {
                            if (!tileDatas[player->Layer * 2 + 1] && (anglemode != 0 || player->EZY > tY))
                                continue;

                            if (probeX == tX + eex &&
                                probeY >= tY + h2 &&
                                probeY <  tY + 16) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles2[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;
                                    //tempAngle = tempAngle * -45.f / 32;
                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }

                                    *angle = tempAngle & 0xFF; //(int)wrapAngle(tempAngle);
                                }

                                player->LastObject = NULL;
                                return true;
                            }
                        }
                    }
                }
                else {
                    if (Data->tiles1[tileID].HasCollision[c]) {
                        if ((Data->tiles1[tileID].IsCeiling ^ flipY)) {
                            if (probeX == tX + eex &&
                                probeY >= tY &&
                                probeY <  tY + 16 - h1) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles1[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;
                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }
                                    *angle = tempAngle & 0xFF;
                                }
                                return true;
                            }
                        }
                        else {
                            if (probeX == tX + eex &&
                                probeY >= tY + h1 &&
                                probeY <  tY + 16) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles1[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;
                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }
                                    *angle = tempAngle & 0xFF;
                                }
                                return true;
                            }
                        }
                    }
                    if (Data->tiles2[tileID].HasCollision[c]) {
                        if (Data->tiles2[tileID].IsCeiling ^ flipY) {
                            if (probeX == tX + eex &&
                                probeY >= tY &&
                                probeY <  tY + 16 - h2) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles2[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;
                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }
                                    *angle = tempAngle & 0xFF;
                                }
                                return true;
                            }
                        }
                        else {
                            if (probeX == tX + eex &&
                                probeY >= tY + h2 &&
                                probeY <  tY + 16) {
                                if (angle != NULL) {
                                    int tempAngle = Data->tiles2[tileID].Config[which];
                                    // if (which == 1 && tempAngle >= 0xC0 && tempAngle > 0xF0) tempAngle = 0xC0;

                                    if (tempAngle != 0) {
                                        if (flipX) tempAngle = 256 - tempAngle;
                                        if (flipY) tempAngle = 128 - tempAngle;
                                    }
                                    *angle = tempAngle & 0xFF;
                                }
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    for (unsigned int o = 0; o < (unsigned int)ObjectSolidCount; o++) {
        Object* obj = ObjectsSolid[o];
		if (!obj) continue;
		if (!obj->Active) continue;
		if (!obj->OnScreen) continue;

        if (CollisionCheckForClimbable) continue;

        if (obj->SolidCustomized) {
            if (player) {
                if (obj->CustomSolidityCheck(probeX, probeY, player->PlayerID, anglemode == 0)) {
                    if (angle)
                        *angle = 0;
                    player->LastObject = obj;
                    return true;
                }
            }
        }
        else {
            bool playerCheck = true;
            if (player)
                playerCheck = player->YSpeed >= 0 && player->EZY < obj->Y - obj->H / 2;

            if (obj->Solid || (obj->SolidTop && anglemode == 0 && playerCheck)) {
                int obj_X = obj->X;
                int obj_Y = obj->Y;
                int obj_W = obj->W / 2;
                int obj_H = obj->H / 2;
                if (probeX >= obj_X - obj_W &&
                    probeY >= obj_Y - obj_H &&
                    probeX <  obj_X + obj_W &&
                    probeY <  obj_Y + obj_H) {
                    if (angle)
                        *angle = 0;

                    if (player)
                        player->LastObject = obj;

                    return true;
                }
            }
        }
    }
    return false;
}

PUBLIC void LevelScene::AddActiveRing(int x, int y, int xs, int ys, int mag) {
    Ring* ring = new Ring();
    ring->X = x;
    ring->Y = y;
    ring->MyX = x << 8;
    ring->MyY = y << 8;
    ring->G = G;
    ring->App = App;
    ring->XSpeed = xs;
    ring->YSpeed = ys;
    ring->Scene = this;
    ring->Active = true;
    ring->Priority = true;
    ring->MagnetizedTo = mag;
    Objects[ObjectCount++] = ring;
    ObjectNewCount++;
}

PUBLIC Explosion* LevelScene::AddExplosion(int animation, bool flip, int x, int y) {
    return AddExplosion(ExplosionSprite, animation, flip, x, y, 0);
}

PUBLIC Explosion* LevelScene::AddExplosion(int animation, bool flip, int x, int y, int vl) {
    return AddExplosion(ExplosionSprite, animation, flip, x, y, vl);
}

PUBLIC Explosion* LevelScene::AddExplosion(ISprite* sprite, int animation, bool flip, int x, int y) {
    return AddExplosion(sprite, animation, flip, x, y, 0);
}

PUBLIC Explosion* LevelScene::AddExplosion(ISprite* sprite, int animation, bool flip, int x, int y, int vl) {
    Explosion* dropdashdust;
    dropdashdust = new Explosion();
    dropdashdust->G = G;
    dropdashdust->App = App;
    dropdashdust->CurrentAnimation = animation;
    dropdashdust->FlipX = flip;
    dropdashdust->Active = true;
    dropdashdust->Sprite = sprite;
    dropdashdust->VisualLayer = vl;
    dropdashdust->X = x;
    dropdashdust->Y = y;
    Explosions.push_back(dropdashdust);

    return dropdashdust;
}

PUBLIC void LevelScene::AddScoreGhost(int frame, int x, int y) {
    ScoreGhost* dropdashdust;
    dropdashdust = new ScoreGhost();
    dropdashdust->G = G;
    dropdashdust->App = App;
    dropdashdust->Scene = this;
    dropdashdust->CurrentAnimation = 14;
    dropdashdust->CurrentFrame = frame;
    dropdashdust->Active = true;
    dropdashdust->Sprite = ObjectsSprite;
    dropdashdust->X = x;
    dropdashdust->Y = y;
    dropdashdust->SubY = y - 32;
    Explosions.push_back(dropdashdust);
}

PUBLIC void LevelScene::AddFallingTile(int til, int x, int y, int offX, int offY, bool flipX, bool flipY, int hold) {
    FallingTile* tile = new FallingTile();
    tile->G = G;
    tile->App = App;
    tile->Scene = this;
    tile->Tile = til;
    tile->Hold = hold;
    tile->FlipX = flipX;
    tile->FlipY = flipY;
    tile->Active = true;
    tile->X = x + offX;
    tile->Y = y + offY;
    Explosions.push_back(tile);
}

PUBLIC void LevelScene::AddFallingTile(int til, int x, int y, int offX, int offY, bool flipX, bool flipY, int xspeed, int yspeed) {
    FallingTile* tile = new FallingTile();
    tile->G = G;
    tile->App = App;
    tile->Scene = this;
    tile->Tile = til;
    tile->Hold = 0;
    tile->grv = 0x70;
    tile->FlipX = flipX;
    tile->FlipY = flipY;
    tile->Active = true;
    tile->X = x + offX;
    tile->Y = y + offY;
    tile->XSpeed = xspeed;
    tile->YSpeed = yspeed;
    Explosions.push_back(tile);
}

PUBLIC void LevelScene::AddMovingSprite(ISprite* sprite, int x, int y, int left, int top, int w, int h, int offX, int offY, bool flipX, bool flipY, int xspeed, int yspeed, int grv, int hold) {
    MovingSprite* tile = new MovingSprite();
    tile->G = G;
    tile->App = App;
    tile->Scene = this;
    tile->Sprite = sprite;
    tile->Hold = hold;
    tile->FlipX = flipX;
    tile->FlipY = flipY;
    tile->Active = true;
    tile->Gravity = grv;
    tile->Left = left;
    tile->Top = top;
    tile->Width = w;
    tile->Height = h;
    tile->SubX = (x) << 16;
    tile->SubY = (y) << 16;
    tile->OffX = offX;
    tile->OffY = offY;
    tile->XSpeed = xspeed;
    tile->YSpeed = yspeed;
    if (LastObjectUpdated)
        tile->VisualLayer = LastObjectUpdated->VisualLayer;
    Explosions.push_back(tile);
}

PUBLIC void LevelScene::AddMovingSprite(ISprite* sprite, int x, int y, int left, int top, int w, int h, int offX, int offY, bool flipX, bool flipY, int xspeed, int yspeed, int grv) {
    AddMovingSprite(sprite, x, y, left, top, w, h, offX, offY, flipX, flipY, xspeed, yspeed, grv, 0);
}

PUBLIC void LevelScene::AddMovingSprite(ISprite* sprite, int x, int y, int animation, int frame, bool flipX, bool flipY, int xspeed, int yspeed, int grv) {
    AddMovingSprite(sprite, x, y, animation, frame, flipX, flipY, xspeed, yspeed, grv, -1, 0);
}

PUBLIC void LevelScene::AddMovingSprite(ISprite* sprite, int x, int y, int animation, int frame, bool flipX, bool flipY, int xspeed, int yspeed, int grv, int life, int hold) {
    ISprite::AnimFrame animframe = sprite->Animations[animation].Frames[frame];

    MovingSprite* tile = new MovingSprite();
    tile->G = G;
    tile->App = App;
    tile->Scene = this;
    tile->Sprite = sprite;
    tile->Hold = hold;
    tile->FlipX = flipX;
    tile->FlipY = flipY;
    tile->Active = true;
    tile->Gravity = grv;
    tile->CurrentAnimation = animation;
    tile->Frame = frame;
    tile->LifeSpan = life;
    tile->Left = -1;
    tile->Top = animframe.Y;
    tile->Width = animframe.W;
    tile->Height = animframe.H;
    tile->SubX = (x) << 16;
    tile->SubY = (y) << 16;
    tile->XSpeed = xspeed;
    tile->YSpeed = yspeed;
    if (LastObjectUpdated)
        tile->VisualLayer = LastObjectUpdated->VisualLayer;
    Explosions.push_back(tile);
}

PUBLIC void LevelScene::AddAnimal(int x, int y, bool flipX, bool flipY, int xspeed, int yspeed, bool escaping) {
    int animalZones[14 * 2] = {
        0, 5,
        2, 6,
        0, 5,
        2, 0,
        1, 0,
        3, 6,
        0, 5,
        1, 5,
        2, 5,
        0, 5,
        2, 5,
        1, 5,
        1, 5,
        0, 5,
    };
    int animalSpeeds[7 * 2] = {
        -0x440, -0x400,
        -0x300, -0x400,
        -0x180, -0x300,
        -0x140, -0x180,
        -0x1C0, -0x300,
        -0x200, -0x300,
        -0x140, -0x380,
    };

    int animalType = animalZones[(ZoneID - 1) * 2 + (Frame & 1)];

    Animal* flicky = new Animal();
    flicky->G = G;
    flicky->App = App;
    flicky->Scene = this;
    flicky->Sprite = AnimalsSprite;
    flicky->FlipX = true;
    flicky->FlipY = flipY;
    flicky->Active = true;

    flicky->CurrentAnimation = animalType * 2;
    flicky->CurrentFrame = 0 << 8;

    flicky->SubX = (x) << 16;
    flicky->SubY = (y) << 16;
    flicky->XSpeed = animalSpeeds[animalType * 2 + 0];
    flicky->YSpeed = -0x400;
    flicky->JumpSpeed = animalSpeeds[animalType * 2 + 1];
    Explosions.push_back(flicky);
}

PUBLIC Object* LevelScene::AddNewObject(int ID, int SubType, int X, int Y, bool FLIPX, bool FLIPY) {
    ObjectNewCount++;

    Object* obj = GetNewObjectFromID(ID);
    if (obj) {
        obj->G = G;
        obj->App = App;
        obj->Scene = this;
        obj->InitialX = X;
        obj->InitialY = Y;
        obj->FlipX = FLIPX == 1;
        obj->FlipY = FLIPY == 1;
        obj->ID = ID;
        while (!SpriteMapIDs[ID]) ID--;
            obj->Sprite = SpriteMapIDs[ID];

        obj->SubType = SubType;
        obj->Create();
        Objects[ObjectCount++] = obj;
    }
    return obj;
}

PUBLIC VIRTUAL void LevelScene::EarlyUpdate() {

}

PUBLIC VIRTUAL void LevelScene::Subupdate() {
    if (ManiaLevel) {
        int8_t DeformDelta[64] = {
            0,    0,    1,    1,    0,    0,    0,    0,    1,    0,    0,    0,    0,    1,    0,    0,
            0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    0,    0,    1,    1,    0,    0,
            0,    0,    1,    1,    0,    0,    0,    0,    1,    0,    0,    0,    0,    1,    0,    0,
            0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    0,    0,    1,    1,    0,    0
        };
        for (int i = 0; i < App->HEIGHT; i++) {
            G->Deform[i] = DeformDelta[(i + (Frame >> 1) + CameraY) & 0x3F] - 1;
        }
    }
}

PUBLIC VIRTUAL void LevelScene::DoResults() {
	ShowResults = true;
	ResultsTimer = 0;
	if (Timer < 60 * 60)
		TimerTotal = 50000;
	else if (Timer < 90 * 60)
		TimerTotal = 10000;
	else if (Timer < 120 * 60)
		TimerTotal = 5000;
	else if (Timer < 150 * 60)
		TimerTotal = 4000;
	else if (Timer < 180 * 60)
		TimerTotal = 3000;
	else if (Timer < 210 * 60)
		TimerTotal = 1000;
	else if (Timer < 599 * 60)
		TimerTotal = 100;
	else
		TimerTotal = 100000;
	TotalToAdd = 0;
	Player->DoVictory();
	App->Audio->ClearMusic();
	App->Audio->PushMusic(Sound::SoundBank[0xFC], false, 0);
}
PUBLIC VIRTUAL void LevelScene::FinishResults() {
    GoToNextAct();
}
PUBLIC VIRTUAL void LevelScene::GoToNextAct() {

}
PUBLIC VIRTUAL void LevelScene::TransferCommonLevelData(LevelScene* NextAct) {
    NextAct->GiantRingModel = GiantRingModel;

    NextAct->GlobalDisplaySprite = GlobalDisplaySprite;
    NextAct->MobileButtonsSprite = MobileButtonsSprite;
    NextAct->ItemsSprite = ItemsSprite;
    NextAct->AnimalsSprite = AnimalsSprite;
    NextAct->ObjectsSprite = ObjectsSprite;
    NextAct->Objects2Sprite = Objects2Sprite;
    NextAct->Objects3Sprite = Objects3Sprite;
    NextAct->RobotnikSprite = RobotnikSprite;
    NextAct->ExplosionSprite = ExplosionSprite;
    NextAct->WaterSprite = WaterSprite;
    for (int i = 0; i < 5; i++) {
        NextAct->KnuxSprite[i] = KnuxSprite[i];
    }
    NextAct->Player = Player;
    for (int p = 0; p < PlayerCount; p++) {
        NextAct->Players[p] = Players[p];
        NextAct->Players[p]->Scene = NextAct;
    }
    NextAct->PlayerCount = PlayerCount;
    NextAct->Score = Score;

    // Set all these to NULL so they do not get cleaned up
    PlayerCount = 0;
    KnuxSprite[0] = NULL;
    KnuxSprite[1] = NULL;
    KnuxSprite[2] = NULL;
    KnuxSprite[3] = NULL;
    KnuxSprite[4] = NULL;
    PauseSprite = NULL;
    GlobalDisplaySprite = NULL;
    MobileButtonsSprite = NULL;
    ItemsSprite = NULL;
    AnimalsSprite = NULL;
    ObjectsSprite = NULL;
    Objects2Sprite = NULL;
    Objects3Sprite = NULL;
    RobotnikSprite = NULL;
    ExplosionSprite = NULL;
    WaterSprite = NULL;

    // #PauseSprite#
    // #GlobalDisplaySprite#
    // #MobileButtonsSprite#
    // #ItemsSprite#
    // #AnimalsSprite#
    // #ObjectsSprite#
    // #Objects2Sprite#
    // #Objects3Sprite#
    // #RobotnikSprite#
    // #ExplosionSprite#
    // #WaterSprite#
}

PUBLIC VIRTUAL void LevelScene::DoCustomFadeAction() {

}

#include <time.h>

bool acitvated = false;
uint64_t playerUpdateCount[8];
float    playerUpdateTimers[8];
uint64_t playerLateUpdateCount[8];
float    playerLateUpdateTimers[8];

PUBLIC void LevelScene::Update() {
    // Pause function
    if (FadeAction == 0 && !ShowResults) {
        // Toggle pause
        if (App->Input->GetControllerInput(0)[IInput::I_PAUSE_PRESSED] && LevelCardTimer >= 4.0) {
            if (!Paused && !PauseFinished) {
                PauseFinished = true;
                Paused = true;
                memset(&PauseAnim[0], 0, 8 * sizeof(int));

                PauseSelectedMenuItem = 0;

                App->Audio->AudioPauseAll();
                Sound::Play(Sound::SFX_MENUACCEPT);
            }
            else {
                Paused = false;

                App->Audio->AudioUnpauseAll();
                //Sound::Play(Sound::SFX_MENUACCEPT);
            }
        }

        if (App->Input->GetControllerInput(0)[IInput::I_EXTRA_PRESSED] && Player) {
            if (maxLayer == 1) {
                Player->XSpeed = 0;
                Player->YSpeed = 0;
                Player->GroundSpeed = 0;
                Player->ObjectControlled = 0;
                Player->DoCollision = 0;
                Player->Action = ActionType::Normal;
			} else if (maxLayer == 0) {
				Player->DoCollision = 1;
                Player->DebugObjectIndex = -1;
				if (Player->DebugObject != NULL && Player->DebugObject->isHeldDebugObject) {
                    Player->DebugObject->Active = false;
                    Player->DebugObject = NULL;
				}
                if (Player->Hidden) {
                    Player->Hidden = false;
                }
			}
    		maxLayer = 1 - maxLayer;
        }
    }

	if (!acitvated) {
		memset(playerUpdateCount, 0, sizeof(playerUpdateCount));
		memset(playerUpdateTimers, 0, sizeof(playerUpdateTimers));
		memset(playerLateUpdateCount, 0, sizeof(playerLateUpdateCount));
		memset(playerLateUpdateTimers, 0, sizeof(playerLateUpdateTimers));
		acitvated = true;
	}

    // Unpaused
    if (!PauseFinished) {
        Frame++;

        // Animate animated tiles
        if (Data) {
            for (int o = 0; o < Data->animatedTilesCount; o++) {
                int framecount = Data->animatedTileDurations[o][0];
                int max = Data->animatedTileDurations[o][framecount + 1];

                int f = Frame % max;
                int sum = 0;
                for (int g = 1; g < framecount + 1; g++) {
                    sum += Data->animatedTileDurations[o][g];

                    if (f < sum) {
                        Data->animatedTileFrames[o] = g - 1;
                        break;
                    }
                }
            }
        }

        if (LevelCardTimer >= 0.25)
            ControlsVisible = true;

        if (HUDVisible) {
            if (HUDAnim > 0x000)
                HUDAnim -= 0x10;
        }
        else {
            if (HUDAnim < 0x100)
                HUDAnim += 0x10;
        }

        if (ControlsVisible) {
            if (ControlsAnim > 0x000)
                ControlsAnim -= 0x10;
        }
        else {
            if (ControlsAnim < 0x100)
                ControlsAnim += 0x10;
        }

        if (Player) {
            if (LevelCardTimer >= 1.5 && Player->Action != ActionType::Dead && FadeAction < FadeActionType::TO_BONUS_STAGE1 && !StopTimer)
                Timer++;
        }

        if (LevelCardTimer < 5.0)
            LevelCardTimer += 1.0 / 60.0;

        // Max timer
        if (Timer > 10 * 60 * 60 - 1) Timer = 10 * 60 * 60 - 1;

        if (FadeAction == 0 && LevelCardTimer >= 1.5 && FadeAction < FadeActionType::TO_BONUS_STAGE1) {
            if (Player) {
                Player->InputUp = App->Input->GetControllerInput(0)[IInput::I_UP];
                Player->InputDown = App->Input->GetControllerInput(0)[IInput::I_DOWN];
                Player->InputLeftPress = App->Input->GetControllerInput(0)[IInput::I_LEFT_PRESSED];
                Player->InputLeft = App->Input->GetControllerInput(0)[IInput::I_LEFT];
                Player->InputRightPress = App->Input->GetControllerInput(0)[IInput::I_RIGHT_PRESSED];
                Player->InputRight = App->Input->GetControllerInput(0)[IInput::I_RIGHT];
                Player->InputJump = App->Input->GetControllerInput(0)[IInput::I_CONFIRM_PRESSED];
                Player->InputJumpHold = App->Input->GetControllerInput(0)[IInput::I_CONFIRM];

                if (Player->Action == ActionType::Dead && Player->EZY > CameraY + App->HEIGHT + 32) {
                    if (Player->Lives > 0 && FadeAction == 0) {
                        Player->Lives--;
                        FadeAction = FadeActionType::DIED;
                        FadeTimerMax = 64;
                        FadeMax = 0x100;
                        G->FadeToWhite = false;
                        App->Audio->FadeMusic(0.75);
                    }
                    else {
                        FadeAction = FadeActionType::EXIT;
                        FadeTimerMax = 60 * 5;
                        FadeMax = 0x100;
                        G->FadeToWhite = false;
                        App->Audio->FadeMusic(5.0);
                    }
                }
            }
        }

		if (!maxLayer) {
			// We're in debug mode! Time to have fun!
			if (!ViewPlayerUpdateStats) {
				int Y = 0;
				char tempStr[256];
				G->DrawRectangle(0, 0, 64, 16, 0);
				sprintf(tempStr, "%04X%04X", Player->EZX, Player->EZY);
				G->DrawTextShadow(0, Y, tempStr, 0xFFFFFF);
				Y += 8;
				sprintf(tempStr, "%04X%04X", CameraX, CameraY);
				G->DrawTextShadow(0, Y, tempStr, 0xFFFFFF);
			}

            if (Player->InputLeft || Player->InputRight || Player->InputUp || Player->InputDown) {
                Player->GroundSpeed += 0x40;
                if (Player->GroundSpeed >= 0x1800) {
                    Player->GroundSpeed = 0x1800;
                }
            } else {
                Player->GroundSpeed = 0;
            }
            if (Player->GroundSpeed >= 0x400) {
                Player->EZX -= (Player->InputLeft) * (Player->GroundSpeed >> 8);
    			Player->EZX += (Player->InputRight) * (Player->GroundSpeed >> 8);
    			Player->EZY -= (Player->InputUp) * (Player->GroundSpeed >> 8);
    			Player->EZY += (Player->InputDown) * (Player->GroundSpeed >> 8);
            }
            else {
                Player->EZX -= App->Input->GetControllerInput(0)[IInput::I_LEFT_PRESSED];
    			Player->EZX += App->Input->GetControllerInput(0)[IInput::I_RIGHT_PRESSED];
    			Player->EZY -= App->Input->GetControllerInput(0)[IInput::I_UP_PRESSED];
    			Player->EZY += App->Input->GetControllerInput(0)[IInput::I_DOWN_PRESSED];
            }

			Player->DisplayX = Player->EZX;
			Player->DisplayY = Player->EZY;

			if (Player->DebugObject) {
				Player->DebugObject->X = Player->DisplayX;
				Player->DebugObject->Y = Player->DisplayY;
			}

            int16_t DebugObjectIDList[2] = {0x01, 0x07};
            const int32_t DebugObjectIDListLength = 2;

			if (Player->InputJump) {
                Player->Hidden = true;
				Player->DebugObjectIndex++;
				Player->DebugObjectIndex = Player->DebugObjectIndex % DebugObjectIDListLength;

				if (Player->DebugObject && Player->DebugObject->isHeldDebugObject) {
					Player->DebugObject->Active = false;
					Player->DebugObject = NULL;
				}
                int16_t objId = DebugObjectIDList[Player->DebugObjectIndex];
                Object* obj = GetNewObjectFromID(objId);
				if (obj != NULL) {
					obj->G = G;
					obj->App = App;
					obj->Scene = this;
					obj->InitialX = Player->EZX;
					obj->InitialY = Player->EZY;
					obj->FlipX = 0;
					obj->FlipY = 0;
					obj->ID = objId;
                    obj->Sprite = SpriteMapIDs[objId];

					obj->SubType = 0;

					obj->isDebugModeObject = true;
					obj->isHeldDebugObject = true;
					obj->DebugCreate();

					obj->Active = true;

					Objects[ObjectCount++] = obj;
                    Player->DebugObject = obj;

					//App->Print(0, "Created Object %d via Debug Mode!", objId);
				} else {
					Player->Hidden = false;
					Player->DebugObjectIndex = -1;
					Player->DebugObject = NULL;
				}
			}

            if (App->Input->GetControllerInput(0)[IInput::I_EXTRA2_PRESSED]) {
				if (Player->DebugObject) {
					uint8_t oldSubType = Player->DebugObject->SubType;

					Player->DebugObject->isHeldDebugObject = false;
					Player->DebugObject = NULL;

					// We want to create a copy for easy use, So we do.
					int16_t objId = DebugObjectIDList[Player->DebugObjectIndex];
					Object* obj = GetNewObjectFromID(objId);
					if (obj != NULL) {
						obj->G = G;
						obj->App = App;
						obj->Scene = this;
						obj->InitialX = Player->EZX;
						obj->InitialY = Player->EZY;
						obj->FlipX = 0;
						obj->FlipY = 0;
						obj->ID = objId;
						obj->Sprite = SpriteMapIDs[objId];

						obj->SubType = oldSubType;

						obj->isDebugModeObject = true;
						obj->isHeldDebugObject = true;
						obj->DebugCreate();

						obj->Active = true;

						Objects[ObjectCount++] = obj;
						Player->DebugObject = obj;
					} else {
						Player->Hidden = false;
						Player->DebugObjectIndex = -1;
						Player->DebugObject = NULL;
					}
				}
            }

            if (App->Input->GetControllerInput(0)[IInput::I_DENY_PRESSED]) {
                if (Player->DebugObject) {
					Player->DebugObject->SubType = (Player->DebugObject->SubType + Player->DebugObject->GetSubTypeIncrement()) % Player->DebugObject->GetSubTypeMax();
					Player->DebugObject->UpdateSubType();
                }
            }

            Player->YSpeed = 0;
		}

        if (!(Frame & 3) && ItemsSprite && Thremixed) {
            ISprite* spr = ItemsSprite;
            Uint32 temp = spr->GetPalette(0x3C + 4 - 1);
            for (int i = 4 - 1; i >= 1; i--) {
                spr->SetPalette(0x3C + i, spr->GetPalette(0x3C + i - 1));
            }
            spr->SetPalette(0x3C, temp);

            temp = spr->GetPaletteAlt(0x3C + 4 - 1);
            for (int i = 4 - 1; i >= 1; i--) {
                spr->SetPaletteAlt(0x3C + i, spr->GetPaletteAlt(0x3C + i - 1));
            }
            spr->SetPaletteAlt(0x3C, temp);

            spr->UpdatePalette();
        }

        EarlyUpdate();

        if (maxLayer) {
            if (LevelCardTimer >= 1.0 && FadeAction < FadeActionType::TO_BONUS_STAGE1) {
				for (int p = 0; p < PlayerCount; p++) {
					playerUpdateCount[p]++;

					clock_t start_t = clock();

					Players[p]->Update();

					playerUpdateTimers[p] += ((clock() - start_t) * 1000.0f / CLOCKS_PER_SEC - playerUpdateTimers[p]) / playerUpdateCount[p];
				}
            }
        }

        for (unsigned int o = 0; o < (unsigned int)ObjectCount && Player->Action != ActionType::Dead; o++) {
            Object* obj = Objects[o];
            if (obj != NULL) {
                if (obj->Active) {
                    LastObjectUpdated = obj;

                    bool OnScreen = false;
                    //*
                    if (obj->VisW > obj->W || obj->VisH > obj->H)
                        OnScreen |= (
                            obj->X + obj->VisW >= CameraX &&
                            obj->Y + obj->VisH >= CameraY &&
                            obj->X - obj->VisW <  CameraX + App->WIDTH &&
                            obj->Y - obj->VisH <  CameraY + App->HEIGHT);

                    //*/
                    OnScreen |= (
                        obj->X + obj->W / 2 >= CameraX &&
                        obj->Y + obj->H / 2 >= CameraY &&
                        obj->X - obj->W / 2 <  CameraX + App->WIDTH &&
                        obj->Y - obj->H / 2 <  CameraY + App->HEIGHT);

                    if (Data->layers[Data->cameraLayer].IsScrollingVertical) {
                        OnScreen |= (
                            obj->X + obj->W / 2 >= CameraX &&
                            (obj->Y + obj->H / 2) % (Data->layers[Data->cameraLayer].Height * 16) >= CameraY &&
                            obj->X - obj->W / 2 <  CameraX + App->WIDTH &&
                            (obj->Y - obj->H / 2) % (Data->layers[Data->cameraLayer].Height * 16) <  CameraY + App->HEIGHT);
                    }

                    if (obj->OnScreen && !OnScreen) {
                        obj->OnScreen = OnScreen;
                        obj->OnLeaveScreen();
                    }

                    obj->OnScreen = OnScreen;
                    if (obj->Priority || OnScreen) {
                        obj->CollidingWithPlayer = false;
                        for (int p = 0; p < PlayerCount && maxLayer; p++) {
                            if (obj->X + obj->W / 2 >= Players[p]->EZX - Players[p]->W / 2 - 1 &&
                                obj->Y + obj->H / 2 >= Players[p]->EZY - Players[p]->H / 2 - 1 &&
                                obj->X - obj->W / 2 <  Players[p]->EZX + Players[p]->W / 2 + 1 &&
                                obj->Y - obj->H / 2 <  Players[p]->EZY + Players[p]->H / 2 + 1) {

                                if (Players[p]->Action == ActionType::Dead) continue;

                                int wy = (Player->W + obj->W) * ((int)Players[p]->EZY - (int)obj->Y);
                                int hx = (Player->H + obj->H) * ((int)Players[p]->EZX - (int)obj->X);

                                int hitFrom = (int)CollideSide::RIGHT;

                                if (wy > hx)
                                    if (wy > -hx)
                                        hitFrom = (int)CollideSide::BOTTOM;
                                    else
                                        hitFrom = (int)CollideSide::LEFT;
                                else
                                    if (wy > -hx)
                                        hitFrom = (int)CollideSide::RIGHT;
                                    else
                                        hitFrom = (int)CollideSide::TOP;

                                obj->CollidingWithPlayer |= obj->OnCollisionWithPlayer(Players[p]->PlayerID, hitFrom, 0);
                            }

                            if (obj->X + obj->W / 2 >= Players[p]->EZX - Players[p]->W / 2 &&
                                obj->X - obj->W / 2 <  Players[p]->EZX + Players[p]->W / 2 &&
                                Players[p]->EZY + Players[p]->H / 2 >= obj->Y - obj->H / 2 - 4 &&
                                Players[p]->EZY + Players[p]->H / 2 <  obj->Y - obj->H / 2 + 2 &&
                                Players[p]->YSpeed >= 0 &&
                                Players[p]->Ground) {
                                obj->BeingStoodOn = true;
                            }
                            else
                                obj->BeingStoodOn = false;
                        }

                        obj->Update();
                    }
                }
            }
        }

		if (maxLayer) {
			if (LevelCardTimer >= 1.0 && FadeAction < FadeActionType::TO_BONUS_STAGE1) {
				for (int p = 0; p < PlayerCount; p++) {
					playerLateUpdateCount[p]++;

					clock_t start_t = clock();

					Players[p]->LateUpdate();

					playerLateUpdateTimers[p] += ((clock() - start_t) * 1000.0f / CLOCKS_PER_SEC - playerLateUpdateTimers[p]) / playerLateUpdateCount[p];
				}
			}
        }

        for (vector<Object*>::iterator it = Explosions.begin(); it != Explosions.end(); ++it) {
            if ((*it)->Active)
                (*it)->Update();
            if (!(*it)->Active) {
                //Object* j = (*it);
                //if (j)
                    //delete j;
                Explosions.erase(it, it);
            }
        }

        RingAnimationFrame += 0x80;
        RingAnimationFrame &= 0xFFF;

        if (WaterSprite) {
            WaterAnimationFrame = (Frame % 40) << 6;
        }

        HandleCamera();
        Subupdate();

        if (ShakeTimer > 0)
            ShakeTimer--;
    }
    // Pause menu
    else {
        if (Paused) {
            PauseAnim[0] += (260 * 0x100 - PauseAnim[0]) / 3;
            PauseAnim[1] += (100 * 0x100 - PauseAnim[1]) / 3;
            PauseAnim[2] += (210 * 0x100 - PauseAnim[2]) / 3;
        }
        else {
            PauseAnim[0] += (-10 * 0x100 - PauseAnim[0]) / 3;
            PauseAnim[1] += (-10 * 0x100 - PauseAnim[1]) / 3;
            PauseAnim[2] += (-10 * 0x100 - PauseAnim[2]) / 3;

            if (PauseAnim[0] <= 0) {
                PauseFinished = false;
                memset(&PauseAnim[0], 0, 8 * sizeof(int));
            }
        }
        PauseAnim[3] = (PauseAnim[3] + 11) % (360);
        for (int i = 0; i < 4; i++)
            if (i == PauseSelectedMenuItem)
                PauseAnim[4 + i] = IMath::min(2, PauseAnim[4 + i] + 1);
            else
                PauseAnim[4 + i] = IMath::max(0, PauseAnim[4 + i] - 1);

        if (FadeAction == 0) {
            if (App->Input->GetControllerInput(0)[IInput::I_UP_PRESSED]) {
                PauseSelectedMenuItem--;
                Sound::Play(Sound::SFX_MENUBLEEP);
            }
            if (App->Input->GetControllerInput(0)[IInput::I_DOWN_PRESSED]) {
                PauseSelectedMenuItem++;
                Sound::Play(Sound::SFX_MENUBLEEP);
            }

            if (App->Input->GetControllerInput(0)[IInput::I_CONFIRM_PRESSED]) { // confirm
                if (PauseSelectedMenuItem == 0) {
                    Paused = false;
                    App->Audio->AudioUnpauseAll();
                    //Sound::Play(Sound::SFX_MENUACCEPT);
                }
                else if (PauseSelectedMenuItem == 1) {
                    FadeAction = FadeActionType::RESTART;
                    FadeTimerMax = 80;
                    FadeMax = 0x200;
                    G->FadeToWhite = false;
                    SavedPositionX = -1;
                    SavedPositionY = -1;
                    Checkpoint = -1;
                    App->Audio->ClearMusic();

                    Sound::Play(Sound::SFX_MENUACCEPT);
                }
                else if (PauseSelectedMenuItem == 2) {
                    FadeAction = FadeActionType::EXIT;
                    FadeTimerMax = 48;
                    FadeMax = 0x120;
                    G->FadeToWhite = false;

                    Sound::Play(Sound::SFX_MENUACCEPT);
                }
            }
            else if (App->Input->GetControllerInput(0)[IInput::I_DENY_PRESSED]) { // deny
                Paused = false;
            }
        }

        if (PauseSelectedMenuItem < 0)
            PauseSelectedMenuItem = 2;
        if (PauseSelectedMenuItem > 2)
            PauseSelectedMenuItem = 0;
    }

    if (ShowResults) {
        if (ResultsTimer > 300) {
            ControlsVisible = false;
            if (!DoneSpinning) {
                if (App->Input->GetControllerInput(0)[IInput::I_PAUSE]) {
                    while (TimerTotal > 0) {
                        int amountToSubtract = 100;
                        if (TimerTotal < amountToSubtract)
                            amountToSubtract = TimerTotal;

                        TimerTotal -= amountToSubtract;
                        TotalToAdd += amountToSubtract;
                    }

                    while (Player->Rings > 0) {
                        Player->Rings--;
                        TotalToAdd += 100;
                    }

                    DoneSpinning = true;
                }
                else {
                    if (Frame % 3 == 0) {
                        Sound::Play(Sound::SFX_SCORE_ADD);
                    }

                    if (TimerTotal > 0) {
                        int amountToSubtract = 100;
                        if (TimerTotal < amountToSubtract)
                            amountToSubtract = TimerTotal;

                        TimerTotal -= amountToSubtract;
                        TotalToAdd += amountToSubtract;
                    }
                    if (Player->Rings > 0) {
                        Player->Rings--;
                        TotalToAdd += 100;
                    }

                    ResultsTimer--;

                    if (Player->Rings == 0 && TimerTotal == 0)
                        DoneSpinning = true;
                }

                if (DoneSpinning) {
                    Sound::Play(Sound::SFX_SCORE_TOTAL);
                    Score += TotalToAdd;
                }
            }
        }
        if (ResultsTimer > 390 + 60) {
            Timer = 0;

            ShowResults = false;

            FinishResults();
        }
        ResultsTimer++;
    }

    // Fade functionality
    if (FadeTimer == -1 && FadeTimerMax > 1)
        FadeTimer = FadeTimerMax;
    if (FadeTimer > 0) {
        FadeTimer--;
        if (FadeAction != FADEIN)
            G->SetFade((int)((1.0f - (float)FadeTimer / FadeTimerMax) * FadeMax));
        else
            //G->SetFade((int)(((float)FadeTimer / FadeTimerMax) * FadeMax));
            G->SetFade(0xFF - (int)((IMath::clampDoubleDown(LevelCardTimer, 1.0, 1.5) / 0.5) * 0xFF));
    }

    // Fade actions
    if (FadeTimer == 0) {
        if (FadeAction == FadeActionType::RESTART) {
            Paused = false;
            PauseFinished = true;
            RestartStage(false, true);
            FadeAction = FadeActionType::FADEIN;
            FadeTimerMax = 90;
            FadeMax = 0x140;
            G->FadeToWhite = false;
        }
        else if (FadeAction == FadeActionType::DIED) {
            Paused = false;
            PauseFinished = true;
            RestartStage(false, true);
            FadeAction = FadeActionType::FADEIN;
            FadeTimerMax = 90;
            FadeMax = 0x140;
            G->FadeToWhite = false;
        }
        else if (FadeAction == FadeActionType::EXIT) {
            //App->Running = false;
            //FadeAction = 0;
            //FadeTimerMax = 1;
            // Cleanup();

            App->NextScene = new Scene_DataSelect(App, G);
        }
        else if (FadeAction == FadeActionType::FADEIN) {
            FadeAction = 0;
            FadeTimerMax = 1;
        }
        else if (FadeAction == FadeActionType::TO_SPECIAL_STAGE) {
            FadeAction = 0;
            FadeTimerMax = 1;
        }
        else if (FadeAction == FadeActionType::NEXT_ZONE) {
            FadeAction = 0;
            FadeTimerMax = 1;
            GoToNextAct();
        }
        else if (FadeAction == FadeActionType::CUSTOM_FADE_ACTION) {
            FadeAction = 0;
            FadeTimerMax = 1;
            DoCustomFadeAction();
        }
        FadeTimer = -1;
    }
    
    CleanupObjects();
}

PUBLIC VIRTUAL void LevelScene::HandleCamera() {
    int d0 = Data->layers[Data->cameraLayer].Width * 16 - App->WIDTH;
    int d1 = Data->layers[Data->cameraLayer].Height * 16 - App->HEIGHT;

    if (Data->layers[Data->cameraLayer].IsScrollingVertical)
        d1 = 0xFFFFF;

    int OffsetX = 0, OffsetY = 0;
    if (Player) {
        if (Player->Action == ActionType::Dead)
            return;

        bool OffCenteredCamera = false;
        if (CameraAutoScrollX != 0) {
            CameraX += CameraAutoScrollX;
            CameraDeltaX += CameraAutoScrollX;
            // CameraDeltaY += CameraAutoScrollY;

            if (Player->EZX - Player->W / 2 < CameraX) {
                Player->EZX = IMath::max(CameraX + Player->W / 2, Player->EZX);
                if (Player->Ground) {
                    if (Player->GroundSpeed < CameraAutoScrollX * 0x100)
                        Player->GroundSpeed = CameraAutoScrollX * 0x100;
                }
                else {
                    // if (Player->XSpeed < CameraAutoScrollX)
                    //     Player->XSpeed = CameraAutoScrollX;
                }
            }
            if (Player->EZX > -8 * OffCenteredCamera + CameraX + App->WIDTH / 2) {
                Player->EZX = IMath::min(-8 * OffCenteredCamera + CameraX + App->WIDTH / 2, Player->EZX);
            }

            return;
        }

        if (Data->layers[Data->cameraLayer].IsScrollingVertical) {
            if (Player->EZY < 0) {
                Player->EZY += Data->layers[Data->cameraLayer].Height * 16;
                CameraY += Data->layers[Data->cameraLayer].Height * 16;
            }

            if (Player->EZY >= Data->layers[Data->cameraLayer].Height * 16) {
                Player->EZY -= Data->layers[Data->cameraLayer].Height * 16;
                CameraY -= Data->layers[Data->cameraLayer].Height * 16;
            }
        }

        for (int p = 0; p < PlayerCount; p++) {
            IPlayer* Player = Players[p];
            if (Player->Action == ActionType::Dead)
                continue;

            if (maxLayer == 0) continue;

            if (!Data->layers[Data->cameraLayer].IsScrollingVertical && Player->EZY > CameraMaxY + App->HEIGHT && maxLayer)
                Player->Die(false);

            if (CameraX == CameraMinX ||
                CameraX == CameraMaxX) {
                Player->CameraLockTimer = 0;
            }

            if (Player->CameraLockTimer <= 0) {
                if (Player->X < CameraMinX + 16) {
                    Player->X = CameraMinX + 16;
                    Player->XSpeed = 0;
                    Player->GroundSpeed = 0;
                }

                if (Player->EZX > CameraMaxX + App->WIDTH - 16) {
                    Player->EZX = CameraMaxX + App->WIDTH - 16;
                    Player->XSpeed = 0;
                    Player->GroundSpeed = 0;
                }
            }
            if (!Data->layers[Data->cameraLayer].IsScrollingVertical && CameraMinY != 0) {
                if (Player->EZY < CameraMinY - 64) {
                    Player->EZY = CameraMinY - 64;
                    Player->YSpeed = 0;
                    Player->GroundSpeed = 0;
                    Player->Ground = false;
                    Player->Angle = 0;
                    Player->AngleMode = 0;
                }
            }
        }

        int camX = int(Player->EZX + Player->CameraX);
        int camY;
        if (ManiaLevel)
            camY = int(Player->EZY + Player->CameraY - 4);
        else
            camY = int(Player->EZY + Player->CameraY + 8);

        if (Player->Action == ActionType::Rolling)
            camY -= Player->H / 2 - 16;

        OffsetX = (camX - (CameraX + App->WIDTH / 2 - 8 * OffCenteredCamera));
        OffsetY = (camY - (CameraY + App->HEIGHT / 2));

        int Max = 0x18;
        if (ZoneID == 5 && Act == 1 && Player->EZX < 0x3880) {
            Max = 0x90;
        }
        if (IMath::abs(OffsetX) > 8) {
            OffsetX -= 8 * IMath::sign(OffsetX);
        }
        else {
            OffsetX  = 0;
        }


        if (Player->CameraLockTimer > 0)
            OffsetX = 0;


        OffsetX = IMath::min(IMath::abs(OffsetX), Max) * IMath::sign(OffsetX);

        if (!Player->Ground) {
            if (IMath::abs(OffsetY) > 32)
                OffsetY -= 32 * IMath::sign(OffsetY);
            else
                OffsetY  = 0;
        }

        Max = 6;
        if (IMath::abs(Player->YSpeed) >= 0x43C && Player->Ground)
            Max = 24;

        // NOTE: Mania notes
        // When player lands, wait 16 frames

        if (IMath::abs(Player->YSpeed) < 0x43C && Player->Ground)
            OffsetY = IMath::min(IMath::abs(OffsetY) / 2, Max) * IMath::sign(OffsetY);

        CameraX += OffsetX;
        CameraY += OffsetY;
    }

    if (CameraMaxX > d0)
        CameraMaxX = d0;
    if (CameraMaxY > d1)
        CameraMaxY = d1;

    if (CameraX < CameraMinX)
        OffsetX = 0;
    if (CameraX > CameraMaxX)
        OffsetX = 0;

    CameraDeltaX += OffsetX;
    CameraDeltaY += OffsetY;

    if (CameraX < CameraMinX)
        CameraX = CameraMinX;
    if (CameraX > CameraMaxX)
        CameraX = CameraMaxX;

    if (!Data->layers[Data->cameraLayer].IsScrollingVertical) {
        if (CameraY < CameraMinY)
            CameraY = CameraMinY;
        if (CameraY > CameraMaxY)
            CameraY = CameraMaxY;
    }
    else {
        if (CameraY < CameraMinY && CameraMinY != 0)
            CameraY = CameraMinY;
        if (CameraY > CameraMaxY && CameraMaxY != Data->layers[Data->cameraLayer].Height * 16 - App->HEIGHT)
            CameraY = CameraMaxY;
    }
}

PUBLIC void LevelScene::AddSelfToRegistry(Object* obj, const char* where) {
    if (!strcmp(where, "Solid"))
        ObjectsSolid[ObjectSolidCount++] = obj;
    else if (!strcmp(where, "Spring"))
        ObjectsSpring[ObjectSpringCount++] = obj;
    else if (!strcmp(where, "Enemies"))
        ObjectsEnemies[ObjectEnemiesCount++] = (Enemy*)obj;
    else if (!strcmp(where, "Breakable"))
        ObjectsBreakable[ObjectBreakableCount++] = obj;
    else if (!strcmp(where, "PathSwitcher"))
        ObjectsPathSwitcher[ObjectPathSwitcherCount++] = obj;
}

PUBLIC void LevelScene::CleanupObjects() {
    // Clean up any un-needed Objects.
    
    Object** RefreshObjects = (Object**)calloc(2000, sizeof(Object*));
    int NewObjectCount = 0;
    
    Object** RefreshObjectsSolid = (Object**)calloc(1000, sizeof(Object*));
    int NewObjectSolidCount = 0;
    
    Object** RefreshObjectsSpring = (Object**)calloc(100, sizeof(Object*));
    int NewObjectSpringCount = 0;
    
    Enemy** RefreshObjectsEnemies = (Enemy**)calloc(300, sizeof(Enemy*));
    int NewObjectEnemiesCount = 0;
    
    Object** RefreshObjectsBreakable = (Object**)calloc(100, sizeof(Object*));
    int NewObjectBreakableCount = 0;
    
    Object** UnrefreshedObjects = Objects;
    int OldObjectCount = ObjectCount;
    
    Object** UnrefreshedObjectsSolid = ObjectsSolid;
    int OldObjectSolidCount = ObjectSolidCount;
    
    Object** UnrefreshedObjectsSpring = ObjectsSpring;
    int OldObjectSpringCount = ObjectSpringCount;
    
    Enemy** UnrefreshedObjectsEnemies = ObjectsEnemies;
    int OldObjectEnemiesCount = ObjectEnemiesCount;

    Object** UnrefreshedObjectsBreakable = ObjectsBreakable;
    int OldObjectBreakableCount = ObjectBreakableCount;
    
    for (int i = 0; i < ObjectCount; i++) {
        if (Objects[i] == nullptr) {
            continue;
        } else if (!Objects[i]->Active) {
            continue;
        }
        RefreshObjects[NewObjectCount] = Objects[i];
        NewObjectCount++;
    }
    
    for (int i = 0; i < ObjectSolidCount; i++) {
        if (ObjectsSolid[i] == nullptr) {
            continue;
        } else if (!ObjectsSolid[i]->Active) {
            continue;
        }
        RefreshObjectsSolid[NewObjectSolidCount] = ObjectsSolid[i];
        NewObjectSolidCount++;
    }
    
    for (int i = 0; i < ObjectSpringCount; i++) {
        if (ObjectsSpring[i] == nullptr) {
            continue;
        } else if (!ObjectsSpring[i]->Active) {
            continue;
        }
        RefreshObjectsSpring[NewObjectSpringCount] = ObjectsSpring[i];
        NewObjectSpringCount++;
    }
    
    for (int i = 0; i < ObjectEnemiesCount; i++) {
        if (ObjectsEnemies[i] == nullptr) {
            continue;
        } else if (!ObjectsEnemies[i]->Active) {
            continue;
        }
        RefreshObjectsEnemies[NewObjectEnemiesCount] = ObjectsEnemies[i];
        NewObjectEnemiesCount++;
    }
    
    for (int i = 0; i < ObjectBreakableCount; i++) {
        if (ObjectsBreakable[i] == nullptr) {
            continue;
        } else if (!ObjectsBreakable[i]->Active) {
            continue;
        }
        RefreshObjectsBreakable[NewObjectBreakableCount] = ObjectsBreakable[i];
        NewObjectBreakableCount++;
    }
    
    Objects = RefreshObjects;
    ObjectCount = NewObjectCount;
    
    ObjectsSolid = RefreshObjectsSolid;
    ObjectSolidCount = NewObjectSolidCount;
    
    ObjectsSpring = RefreshObjectsSpring;
    ObjectSpringCount = NewObjectSpringCount;
    
    ObjectsEnemies = RefreshObjectsEnemies;
    ObjectEnemiesCount = NewObjectEnemiesCount;
    
    ObjectsBreakable = RefreshObjectsBreakable;
    ObjectBreakableCount = NewObjectBreakableCount;
    
	for (int i = 0; i < OldObjectCount; i++) {
		if (UnrefreshedObjects[i] == nullptr) {
			continue;
		}
		if (!UnrefreshedObjects[i]->Active) {
			delete UnrefreshedObjects[i];
			UnrefreshedObjects[i] = nullptr;
		}
	}
    
	for (int i = 0; i < OldObjectSolidCount; i++) {
		if (UnrefreshedObjectsSolid[i] == nullptr) {
			continue;
		}
		if (!UnrefreshedObjectsSolid[i]->Active) {
			delete UnrefreshedObjectsSolid[i];
			UnrefreshedObjectsSolid[i] = nullptr;
		}
	}
    
    for (int i = 0; i < OldObjectSpringCount; i++) {
		if (UnrefreshedObjectsSpring[i] == nullptr) {
			continue;
		}
		if (!UnrefreshedObjectsSpring[i]->Active) {
			delete UnrefreshedObjectsSpring[i];
			UnrefreshedObjectsSpring[i] = nullptr;
		}
	}
    
    for (int i = 0; i < OldObjectEnemiesCount; i++) {
		if (UnrefreshedObjectsEnemies[i] == nullptr) {
			continue;
		}
		if (!UnrefreshedObjectsEnemies[i]->Active) {
			delete UnrefreshedObjectsEnemies[i];
			UnrefreshedObjectsEnemies[i] = nullptr;
		}
	}
    
	for (int i = 0; i < OldObjectBreakableCount; i++) {
		if (UnrefreshedObjectsBreakable[i] == nullptr) {
			continue;
		}
		if (!UnrefreshedObjectsBreakable[i]->Active) {
			delete UnrefreshedObjectsBreakable[i];
			UnrefreshedObjectsBreakable[i] = nullptr;
		}
	}
    
    free(UnrefreshedObjects);
    free(UnrefreshedObjectsSolid);
    free(UnrefreshedObjectsSpring);
    free(UnrefreshedObjectsEnemies);
    free(UnrefreshedObjectsBreakable);
}

PUBLIC void LevelScene::RenderAnimatedSprites(int layer) {

}

PUBLIC void LevelScene::RenderRings() {
    for (unsigned int o = 0; o < (unsigned int)RingPropCount; o++) {
        ObjectProp obj = RingProps[o];
		if (!obj.ID) continue;

		bool OnScreenH =
			obj.X + 8 >= CameraX &&
			obj.X - 8 < CameraX + App->WIDTH;
		if (!OnScreenH) continue;

        int oY = obj.Y;
		bool OnScreen =
			oY + 8 >= CameraY &&
            oY - 8 <  CameraY + App->HEIGHT;

        if (Data->layers[Data->cameraLayer].IsScrollingVertical && !OnScreen) {
            oY -= Data->layers[Data->cameraLayer].Height * 16;
            OnScreen =
				oY + 8 >= CameraY &&
                oY - 8 <  CameraY + App->HEIGHT;
        }

        if (OnScreen) {
            if (Thremixed)
                G->DrawSprite(ItemsSprite, 7, RingAnimationFrame >> 8, obj.X - CameraX, oY - CameraY, 0, IE_NOFLIP);
            else
                G->DrawSprite(ItemsSprite, 7, RingAnimationFrame >> 10, obj.X - CameraX, oY - CameraY, 0, IE_NOFLIP);
        }
    }
}

PUBLIC void LevelScene::RenderHUD() {
    bool Mobile = IApp::Platform == Platforms::iOS || IApp::Platform == Platforms::Android;
    App->Input->UseTouchController = true;

    int value;
    int valen;

    int STR_X = 16 - (HUDAnim >> 1);

	ISprite* GlobalDisplaySprite = this->GlobalDisplaySprite;
	if (!Thremixed) {
		GlobalDisplaySprite = GlobalDisplaySpriteS3K;
	}
    // Score
    G->DrawSprite(GlobalDisplaySprite, 0, 0, STR_X, 12, 0, IE_NOFLIP);

    // Time
    G->DrawSprite(GlobalDisplaySprite, 0, 1, STR_X, 28, 0, IE_NOFLIP);

    // Rings
    int CurrentFrame = 3;
    if (Player->Rings == 0 && (this->Frame & 0xF) < 8)
        CurrentFrame = 4;
    G->DrawSprite(GlobalDisplaySprite, 0, CurrentFrame, STR_X, 44, 0, IE_NOFLIP);

    // Score value
    value = Score;
    for (int i = 0; i < 7 && (value > 0 || (value == 0 && i == 0)); i++) {
        G->DrawSprite(GlobalDisplaySprite, 1, value % 10, STR_X + 48 + 8 * 7 - 8 * i - 8, 12 + 14, 0, IE_NOFLIP);
        value /= 10;
    }

    // Timer " and '
	G->DrawSprite(GlobalDisplaySprite, 0, 12, STR_X + 48 + 8 * 1 - 8 + 3, 28 + 14 - 16, 0, IE_NOFLIP);

    // Timer value (Centiseconds)
    value = (Timer % 60) * 100 / 60;
    if (Timer >= 10 * 60 * 60 - 3)
        value = 99;
    for (int i = 0; i < 2; i++) {
        G->DrawSprite(GlobalDisplaySprite, 1, value % 10, STR_X + 48 + 8 * 7 - 8 * i - 8, 28 + 14, 0, IE_NOFLIP);
        value /= 10;
    }
    // Timer value (Seconds)
    value = (Timer / 60) % 60;
    for (int i = 0; i < 2; i++) {
        G->DrawSprite(GlobalDisplaySprite, 1, value % 10, STR_X + 48 + 8 * 4 - 8 * i - 9, 28 + 14, 0, IE_NOFLIP);
        value /= 10;
    }
    // Timer value (Minutes)
    value = (Timer / 60 / 60);
    if (value > 9)
        value = 9;
    G->DrawSprite(GlobalDisplaySprite, 1, value % 10, STR_X + 48 + 8 * 1 - 10, 28 + 14, 0, IE_NOFLIP);

    // Ring value (Or object count value in Debug mode.)
    if (!maxLayer) {
        value = ObjectCount;
    } else {
        value = Player->Rings;
    }
    
    for (int i = 0; i < 7 && (value > 0 || (value == 0 && i == 0)); i++) {
        G->DrawSprite(GlobalDisplaySprite, 1, value % 10, STR_X + 48 + 8 * 7 - 8 * i - 8, 44 + 14, 0, IE_NOFLIP);
        value /= 10;
    }

    int iconX = STR_X;
    int iconY = App->HEIGHT - 12;

    if (Mobile) {
        iconX = App->WIDTH - 64 - 8 - STR_X; // 24 is for Pause Button
        iconY = 12 + 17;
    }

    // Character Icon
    G->DrawSprite(GlobalDisplaySprite, 2, (int)Player->Character, iconX, iconY, 0, IE_NOFLIP);

    // x symbol
	if (Thremixed)
		G->DrawSprite(GlobalDisplaySprite, 0, 14, iconX, iconY, 0, IE_NOFLIP);

    // Lives value
	int LifeNumberAnim = 1;
	if (!Thremixed)
		LifeNumberAnim = 10;

    valen = 1;
    value = Player->Lives;
    if (value >= 10)
        valen = 2;
    if (value >= 99)
        value = 99;
    for (int i = 0; i < valen; i++) {
        G->DrawSprite(GlobalDisplaySprite, LifeNumberAnim, value % 10, iconX + 32 + 2 * 8 - i * 8, iconY, 0, IE_NOFLIP);
        value /= 10;
    }

    if (Mobile) {
        G->SetDrawAlpha(0xC0 - (0xC0 * ControlsAnim >> 8));
        int bX = 48;
        int bY = App->HEIGHT - 48;
        if (App->Input->GetControllerInput(0)[IInput::I_UP])
            G->DrawSprite(MobileButtonsSprite, 0, 1, bX, bY, 0, IE_NOFLIP);
        else if (App->Input->GetControllerInput(0)[IInput::I_DOWN])
            G->DrawSprite(MobileButtonsSprite, 0, 2, bX, bY, 0, IE_NOFLIP);
        else if (App->Input->GetControllerInput(0)[IInput::I_LEFT])
            G->DrawSprite(MobileButtonsSprite, 0, 3, bX, bY, 0, IE_FLIPX);
        else if (App->Input->GetControllerInput(0)[IInput::I_RIGHT])
            G->DrawSprite(MobileButtonsSprite, 0, 3, bX, bY, 0, IE_NOFLIP);
        else
            G->DrawSprite(MobileButtonsSprite, 0, 0, bX, bY, 0, IE_NOFLIP);

        // A button
        if (App->Input->GetControllerInput(0)[IInput::I_CONFIRM])
            G->SetDrawAlpha(0xFF);

        G->DrawSprite(MobileButtonsSprite, 0, 4, App->WIDTH - 48, bY, 0, IE_NOFLIP);
        G->SetDrawAlpha(0xFF);

        // Pause
        G->DrawSprite(MobileButtonsSprite, 0, 5, App->WIDTH - 22, 22, 0, IE_NOFLIP);
    }
}

PUBLIC void LevelScene::RenderTitleCard() {
    if (LevelCardHide)
        return;

    if (LevelCardTimer == 0.0) return;

    if (LevelCardTimer > 4.0)
        return;

    int redBar = (int)(G->easeOutQuad(IMath::clampDoubleDown(LevelCardTimer, 0.0, 0.5) / 0.5) * App->HEIGHT) - App->HEIGHT;
    int redBarOriginX = App->WIDTH / 2 - 75;
    int off = (int)(G->easeInQuad(IMath::clampDoubleDown(LevelCardTimer, 3.25, 3.5) / 0.25) * -App->HEIGHT);
    redBar += off;

    G->DrawRectangleSkewedH(
        redBarOriginX - App->HEIGHT + redBar, redBar,
        100, App->HEIGHT, App->HEIGHT, 0xEE0000);

    // G->DrawSprite(GlobalDisplaySprite, CurrentAnimation, CurrentFrame, redBarOriginX + 5 + redBar, 220 + redBar, 0, IE_NOFLIP);

    int text1Off = (int)(G->easeOutBack(IMath::clampDoubleDown(LevelCardTimer, 0.25, 0.75) / 0.5) * -400) + 400;
    int text2Off = (int)(G->easeOutBack(IMath::clampDoubleDown(LevelCardTimer, 0.35, 0.85) / 0.5) * -400) + 400;

    off = (int)(G->easeInBack(IMath::clampDoubleDown(LevelCardTimer, 3.25, 4.0) / 0.75) * 400);
    text1Off += off;
    text2Off += off;

    int textX, textY, textW;

    int ex = 0;

    for (size_t i = 0; i < strlen(LevelName); i++) {
        if (LevelName[i] == ' ') {
            ex += 16;
        }
        else {
            ISprite::AnimFrame Frame;
            Frame = GlobalDisplaySprite->Animations[15].Frames[LevelName[i] - 'A'];
            ex += Frame.W + 1;
        }
    }
    textW = ex;

    textX = App->WIDTH - 34 - textW + text1Off;
    textY = 100;

    G->DrawRectangleSkewedH(
        textX - 30, textY - 8,
        textW + 35, 27, 27, 0x222222);

    G->SetClip(textX - 25, textY - 24, textW + 25, 47);

    ex = 0;
    for (int i = 0; i < (int)strlen(LevelName); i++) {
        if (LevelName[i] == ' ') {
            ex += 16;
        }
        else {
            ISprite::AnimFrame Frame;
            Frame = GlobalDisplaySprite->Animations[15].Frames[LevelName[i] - 'A'];
            G->DrawSprite(GlobalDisplaySprite, 15, LevelName[i] - 'A', textX + ex + Frame.W / 2, textY, 0, IE_NOFLIP);

            ex += Frame.W + 1;
        }
    }

    G->ClearClip();

    textY += 36;

    textX = App->WIDTH - 34 + 8 + text2Off;
    G->DrawRectangleSkewedH(
        textX - 25 - 32 * 4, textY - 5,
        128 + 25, 13, 13, 0xEEEEEE);

    for (int i = 0; i < 4; i++) {
        G->DrawSprite(GlobalDisplaySprite, 16, i, textX, textY, 0, IE_NOFLIP);
    }
}

PUBLIC void LevelScene::RenderPauseScreen() {
    if (!PauseFinished) {
        G->ClearClone();
        return;
    }

    // HACK: For when the Scene cleans up before we render PauseSprite for the last time
    if (!PauseSprite) return;

    int anim_off;

    // Top-left green shape
    anim_off = -260 + PauseAnim[0] / 0x100;
    G->DrawRectangle(0 + anim_off, 0, 188, 58, 0x00CC00);
    G->DrawTriangle(188 + anim_off, 0, 188 + anim_off, 58, 188 + anim_off + 58, 0, 0x00CC00);

    // Top-right red triangle
    anim_off = -210 + PauseAnim[2] / 0x100;
    G->DrawTriangle(App->WIDTH - 100, 0 + anim_off, App->WIDTH, 100 + anim_off, App->WIDTH, 0 + anim_off, 0xE00000);

    // Large yellow triangle
    anim_off = 210 - PauseAnim[2] / 0x100;
    G->DrawTriangle(App->WIDTH - 1 + anim_off, 10,
        App->WIDTH - 1 + anim_off, App->HEIGHT,
        App->WIDTH - 1 - (App->HEIGHT - 10) + anim_off, App->HEIGHT, 0xF7DB08);

    // small Mania logo sprite
    //G->DrawSprite(GlobalDisplaySprite, CurrentAnimation, CurrentFrame, App->WIDTH - 1 - 62 + anim_off - 12, App->HEIGHT - 1 - 24, 0, IE_NOFLIP);

    // Paused text sprite
    // anim_off = -100 + PauseAnim[1] / 0x100;
    // G->DrawRectangle(0, 21 + anim_off, 120, 24, 0x000000);
    // G->DrawTriangle(120, 21 + anim_off,
    //     120 + 23, 21 + anim_off + 23,
    //     120, 21 + anim_off + 23,
    //     0x000000);
    // G->DrawSprite(PauseSprite, 1, 1, 96, 32, 11, 13 + anim_off, 0, IE_NOFLIP, 0, 0);
    //
    // anim_off = 210 - PauseAnim[2] / 0x100;
    // int baseX = 280 + anim_off;
    // int baseY = 70 + 20;
    //
    // for (int i = 0; i < 3; i++) {
    //     int o = PauseAnim[4 + i];
    //
    //     if (o != 0) {
    //         uint8_t B = 0xC0;
    //         uint8_t RG = IMath::max(0, IMath::min(  (Cos[PauseAnim[3]] * 0x60 + 0x6000) / 0x100, 0xFF));
    //
    //         G->DrawRectangle(baseX - 8 - o - i * 36, baseY + o + i * 36 + 4, 240, 22, RG << 16 | RG << 8 | B);
    //         G->DrawTriangle(
    //             baseX - 8 - o - i * 36, baseY + o + i * 36 + 4,
    //             baseX - 29 - o - i * 36, baseY + o + i * 36 + 4,
    //             baseX - 8 - o - i * 36, baseY + o + i * 36 + 25,
    //             RG << 16 | RG << 8 | B);
    //     }
    //
    //     G->DrawRectangle(baseX - 8 + o - i * 36, baseY - o + i * 36 + 4, 240, 22, 0x000000);
    //     G->DrawTriangle(
    //         baseX - 8 + o - i * 36, baseY - o + i * 36 + 4,
    //         baseX - 29 + o - i * 36, baseY - o + i * 36 + 4,
    //         baseX - 8 + o - i * 36, baseY - o + i * 36 + 25,
    //         0x000000);
    // }
    //
    // int oo = PauseAnim[4];
    // G->DrawSprite(PauseSprite, 1, 34, 112, 22,
    //               baseX - 02 + oo,  baseY - 2 - oo, 0, IE_NOFLIP, 0, 0);
    //
    // oo = PauseAnim[5];
    // G->DrawSprite(PauseSprite, 1, 57, 99, 22,
    //               baseX - 36 + oo, baseY + 36 - oo, 0, IE_NOFLIP, 0, 0);
    //
    // oo = PauseAnim[6];
    // G->DrawSprite(PauseSprite, 114, 34, 50, 22,
    //               baseX - 72 + oo, baseY + 72 - oo, 0, IE_NOFLIP, 0, 0);

    anim_off = -100 + PauseAnim[1] / 0x100;
    G->DrawRectangle(0, 21 + anim_off, 120, 24, 0x000000);
    G->DrawTriangle(120, 21 + anim_off,
        120 + 24, 21 + anim_off + 24,
        120, 21 + anim_off + 24,
        0x000000);
    G->DrawSprite(PauseSprite, 10, 3, 11 + 48, 13 + anim_off + 20, 0, IE_NOFLIP);

    anim_off = 210 - PauseAnim[2] / 0x100;
    int baseX = 280 + anim_off;
    int baseY = 70 + 20;

    for (int i = 0; i < 3; i++) {
      int o = PauseAnim[4 + i];

      if (o != 0) {
          uint8_t B = 0xC0;
          uint8_t RG = IMath::clamp(((IMath::cosHex(PauseAnim[3]) * 0x60 >> 16) + 0x6000) / 0x100, 0x00, 0xFF);

          G->DrawRectangle(baseX - 8 - o - i * 36, baseY + o + i * 36 + 4, 240, 22, RG << 16 | RG << 8 | B);
          G->DrawTriangle(
              baseX - 30 - o - i * 36, baseY + o + i * 36 + 4,
              baseX - 8 - o - i * 36,  baseY + o + i * 36 + 4,
              baseX - 8 - o - i * 36,  baseY + o + i * 36 + 26,
              RG << 16 | RG << 8 | B);
      }

      G->DrawRectangle(baseX - 8 + o - i * 36, baseY - o + i * 36 + 4, 240, 22, 0x000000);
      G->DrawTriangle(
          baseX - 30 + o - i * 36, baseY - o + i * 36 + 4,
          baseX - 8 + o - i * 36,  baseY - o + i * 36 + 4,
          baseX - 8 + o - i * 36,  baseY - o + i * 36 + 26,
          0x000000);
    }

    int oo = PauseAnim[4];
    G->DrawSprite(PauseSprite, 10, 0,
                baseX + 13 - 02 + oo,  baseY + 16 - 2 - oo, 0, IE_NOFLIP);

    oo = PauseAnim[5];
    G->DrawSprite(PauseSprite, 10, 1,
                baseX + 13 - 36 + oo, baseY + 16 + 36 - oo, 0, IE_NOFLIP);

    oo = PauseAnim[6];
    G->DrawSprite(PauseSprite, 10, 2,
                baseX + 13 - 72 + oo, baseY + 16 + 72 - oo, 0, IE_NOFLIP);
}

PUBLIC void LevelScene::RenderResults() {
	if (!ShowResults) return;

	int value;

	double resultsTimer = ResultsTimer / 60.0;

	int anim_player_got;
	int anim_through_act;
	int anim_time_bonus;
	int anim_ring_bonus;
	int anim_total;

	if (ResultsTimer > 390) {
		resultsTimer = (60 - (ResultsTimer - 390)) / 60.0;
	}

	anim_player_got = (int)(G->easeOutQuad(IMath::clampDoubleDown(resultsTimer, 0.0, 0.4) / 0.4) * App->WIDTH) - App->WIDTH;
	anim_through_act = (int)(G->easeOutQuad(IMath::clampDoubleDown(resultsTimer, 0.2, 0.6) / 0.4) * -App->WIDTH) + App->WIDTH;
	anim_time_bonus = (int)(G->easeOutQuad(IMath::clampDoubleDown(resultsTimer, 0.5, 0.8) / 0.3) * -App->WIDTH) + App->WIDTH;
	anim_ring_bonus = (int)(G->easeOutQuad(IMath::clampDoubleDown(resultsTimer, 0.6, 0.9) / 0.3) * -App->WIDTH) + App->WIDTH;
	anim_total = (int)(G->easeOutQuad(IMath::clampDoubleDown(resultsTimer, 0.7, 1.0) / 0.3) * -App->WIDTH) + App->WIDTH;

	ISprite* GlobalDisplaySprite = this->GlobalDisplaySprite;
	if (GlobalDisplaySpriteS3K) {
		GlobalDisplaySprite = GlobalDisplaySpriteS3K;
	}

	// Player Name
	G->DrawSprite(GlobalDisplaySprite, 3, (int)Player->Character, App->WIDTH / 2 + anim_player_got, 64, 0, IE_NOFLIP);

	// Got
	G->DrawSprite(GlobalDisplaySprite, 4, 0, App->WIDTH / 2 + anim_player_got, 64, 0, IE_NOFLIP);

	// Through
	G->DrawSprite(GlobalDisplaySprite, 4, 1, App->WIDTH / 2 + anim_through_act, 88, 0, IE_NOFLIP);

	// Time Bonus
	G->DrawSprite(GlobalDisplaySprite, 0, 1, App->WIDTH / 2 - 128 + anim_time_bonus, 128, 0, IE_NOFLIP);
	G->DrawSprite(GlobalDisplaySprite, 0, 8, App->WIDTH / 2 - 128 + 40 + anim_time_bonus, 128, 0, IE_NOFLIP);

	// Ring Bonus
	G->DrawSprite(GlobalDisplaySprite, 0, 5, App->WIDTH / 2 - 128 + anim_ring_bonus, 144, 0, IE_NOFLIP);

	G->DrawSprite(GlobalDisplaySprite, 0, 8, App->WIDTH / 2 - 128 + 40 + anim_ring_bonus, 144, 0, IE_NOFLIP);

	// Total
	G->DrawSprite(GlobalDisplaySprite, 0, 9, App->WIDTH / 2 - 112 + anim_total, 176, 0, IE_NOFLIP);

	// Time Bonus Value
	value = TimerTotal;
	for (int i = 0; i < 7 && (value > 0 || (value == 0 && i == 0)); i++) {
		G->DrawSprite(GlobalDisplaySprite, 1, value % 10, App->WIDTH / 2 + 112 - i * 8 - 4 + anim_time_bonus, 128 + 14, 0, IE_NOFLIP);
		value /= 10;
	}

	// Ring Bonus Value
	value = Player->Rings * 100;
	for (int i = 0; i < 7 && (value > 0 || (value == 0 && i == 0)); i++) {
		G->DrawSprite(GlobalDisplaySprite, 1, value % 10, App->WIDTH / 2 + 112 - i * 8 - 4 + anim_ring_bonus, 144 + 14, 0, IE_NOFLIP);
		value /= 10;
	}

	// Total Value
	value = TotalToAdd;
	for (int i = 0; i < 7 && (value > 0 || (value == 0 && i == 0)); i++) {
		G->DrawSprite(GlobalDisplaySprite, 1, value % 10, App->WIDTH / 2 + 112 - i * 8 - 4 + anim_total, 176 + 14, 0, IE_NOFLIP);
		value /= 10;
	}
}
PUBLIC VIRTUAL void LevelScene::RenderAboveBackground() {

}
PUBLIC VIRTUAL void LevelScene::RenderAboveForeground() {

}

PUBLIC void LevelScene::DrawThing(int l) {
    //Layer layer = Data->layers[l];
    //int EndTileBaseX, EndTileBaseY;
    //int index, TileBaseX, TileBaseY, split, extra, tileHeight, TileSel, TileSelBoost;


}

PUBLIC VIRTUAL void LevelScene::RenderEverything() {
    // TODO: Optimize

    G->DrawRectangle(0, 0, App->WIDTH, App->HEIGHT, BackgroundColor);
    G->WaterPaletteStartLine = VisualWaterLevel - CameraY;

    int highlightedTile = -1;

    Layer layer;
	int fullFlip;
    int s, siT, x, y;
    int tile, flipX, flipY, baseX, baseY, wheree;
    int index, TileBaseX, TileBaseY;
    // int EndTileBaseX, EndTileBaseY;
	bool DeformObjects = false;
	bool DeformPlayer = false;

    for (int l = 0; l < Data->layerCount; l++) {
        y = 0;

        layer = Data->layers[l];

        // Draw Tiles
        if (layer.Visible) {
            G->DoDeform = true;
            memcpy(G->Deform, layer.Deform, App->HEIGHT);
            if (layer.InfoCount > 1) {
                int buf = 0;
                for (s = 0; s < layer.ScrollIndexCount; s++) {
                    index = layer.ScrollIndexes[s].Index;

                    int heightSize, tilindx, tilindy, tilindpos, word, fBX;
                    for (siT = 0; siT < layer.ScrollIndexes[s].Size; siT += heightSize) {
                        heightSize = 16;
                        if (heightSize > layer.ScrollIndexes[s].Size - siT)
                            heightSize = layer.ScrollIndexes[s].Size - siT;
                        if (heightSize > ((y + siT + 0x10) & ~0xF) - (y + siT)) // Rounded-up
                            heightSize = ((y + siT + 0x10) & ~0xF) - (y + siT);

                        if (layer.UseDeltaCameraX)
                            TileBaseX = (CameraDeltaX * layer.Info[index].RelativeX + layer.Info[index].ConstantX * Frame) >> 8;
                        else
                            TileBaseX = (CameraX * layer.Info[index].RelativeX + layer.Info[index].ConstantX * Frame) >> 8;

                        TileBaseY = y + siT;
                        if (layer.UseDeltaCameraY)
                            TileBaseY -= (CameraDeltaY * layer.RelativeY + layer.ConstantY * Frame) >> 8;
                        else
                            TileBaseY -= (CameraY * layer.RelativeY + layer.ConstantY * Frame) >> 8;

                        TileBaseX -= layer.OffsetX;
                        TileBaseY -= layer.OffsetY;

                        int bufVal = 0;
                        if (TileBaseY + heightSize > 0 && TileBaseY < App->HEIGHT) {
                            bufVal = layer.Width * 16;
                            fBX = (((-TileBaseX) % bufVal + bufVal) % bufVal);

                            for (x = (TileBaseX >> 4) - 1; x < 2 + ((TileBaseX + App->WIDTH) >> 4); x++) {
                                tilindy = ((y + siT) >> 4);
                                tilindy = (tilindy % layer.Height + layer.Height) % layer.Height; // so it loops
                                tilindx = ((x % layer.Width + layer.Width) % layer.Width);
                                tilindpos = tilindx + (tilindy) * layer.Width;

                                baseX = (x << 4) - TileBaseX;
                                baseY = TileBaseY;

                                word = layer.Tiles[tilindpos];
								fullFlip = (word >> 10) & 3;
                                tile = word & 0x3FF;

                                if (tile != BlankTile) {
                                    int anID = Data->isAnims[tile] & 0xFF;

                                    if (anID != 0xFF) {
                                        G->DrawSprite(AnimTileSprite, Data->isAnims[tile] >> 8, Data->animatedTileFrames[anID], baseX + 8, baseY + 8, 0, fullFlip);
                                    }
                                    else {
                                        // tilindx + buf * layer.Width
                                        if (layer.ScrollIndexes[0].TileBuffers && layer.ScrollIndexes[0].TileBuffers[buf] > 0) {
                                            if (bufVal != 0)
                                                G->DrawSpriteListBuffer(TileSprite,
                                                    layer.ScrollIndexes[0].TileBuffers[buf], layer.Width,
                                                    fBX + 8, baseY + heightSize / 2);
                                                G->DrawSpriteListBuffer(TileSprite,
                                                    layer.ScrollIndexes[0].TileBuffers[buf], layer.Width,
                                                    fBX + 8 - bufVal, baseY + heightSize / 2);
                                                // G->DrawSpriteListBuffer(TileSprite,
                                                //     layer.ScrollIndexes[0].TileBuffers[buf], layer.Width,
                                                //     fBX + 8 + bufVal, baseY + heightSize / 2);

                                                // G->DrawSpriteBuffered(TileSprite,
                                                //     layer.ScrollIndexes[0].TileBuffers[buf], // bufVal,
                                                //     // baseX + 8, baseY + heightSize / 2, 0, bufVal);//fullFlip);
                                                //     TileBaseX + 8, baseY + heightSize / 2, 0, bufVal);//fullFlip);

                                            bufVal = 0;
                                        }
                                        else {
											flipY = ((fullFlip >> 1) & 1);

											wheree = (y + siT) & 0xF;
											if (flipY)
												wheree = 0x10 - wheree - heightSize;

											if (TileSprite->Width > 16)
												G->DrawSprite(TileSprite, ((tile & 0x1F) << 4), wheree + ((tile >> 5) << 4), 16, heightSize, baseX + 8, baseY + heightSize / 2, 0, fullFlip, -8, -heightSize / 2 - (heightSize & flipY));
											else
												G->DrawSprite(TileSprite, 0, wheree + (tile << 4), 16, heightSize, baseX + 8, baseY + heightSize / 2, 0, fullFlip, -8, -heightSize / 2 - (heightSize & flipY));
                                        }
                                    }
                                }
                            }
                        }

                        buf++;
                    }
                    y += layer.ScrollIndexes[s].Size;
                }
            } else {
                if (layer.Info[0].HeatWaveEnabled) {
                    if (ManiaLevel)
                        G->DoDeform = true;
                }

                if (layer.UseDeltaCameraX)
                    TileBaseX = (CameraDeltaX * layer.Info[0].RelativeX + layer.Info[0].ConstantX * Frame) >> 8;
                else
                    TileBaseX = (CameraX * layer.Info[0].RelativeX + layer.Info[0].ConstantX * Frame) >> 8;

                if (layer.UseDeltaCameraY)
                    TileBaseY = (CameraDeltaY * layer.RelativeY + layer.ConstantY * Frame) >> 8;
                else
                    TileBaseY = (CameraY * layer.RelativeY + layer.ConstantY * Frame) >> 8;

                TileBaseX -= layer.OffsetX;
                TileBaseY -= layer.OffsetY;

                // EndTileBaseX = 2 + ((TileBaseX + App->WIDTH) >> 4);
                // EndTileBaseY = 2 + ((TileBaseY + App->HEIGHT) >> 4);

                //int lWid = layer.Width;

                // int j;
				// int bufVal;
				int tBX = (TileBaseX >> 4) - 1;
				int tBW = 3 + (App->WIDTH >> 4);
				int tBH = 2 + (App->HEIGHT >> 4);
				int ix = tBX,
					iy = (TileBaseY >> 4),
					fx = 0,
					fy = 0,
					// th = (TileBaseY >> 4) + tBH,
					tw = tBX + tBW;
				int fullSize = tBW * tBH;
				int anID;
				for (int w = 0; w < fullSize; w++) {
					fx = ix;
					fy = iy;
					if (layer.Flags & 0x2) {
						if (fx < 0) {
							// Increment
							ix++;
							if (ix >= tw) {
								ix = tBX;
								iy++;
							}
							continue;
						}
						if (fx >= layer.Width) {
							// Increment
							ix++;
							if (ix >= tw) {
								ix = tBX;
								iy++;
							}
							continue;
						}
					}
					if (layer.Flags & 0x4) {
						if (fy < 0) {
							// Increment
							ix++;
							if (ix >= tw) {
								ix = tBX;
								iy++;
							}
							continue;
						}
						if (fy >= layer.Height) {
							// Increment
							ix++;
							if (ix >= tw) {
								ix = tBX;
								iy++;
							}
							continue;
						}
					}

					while (fx < 0) fx += layer.Width;
					while (fx >= layer.Width) fx -= layer.Width;

					while (fy < 0) fy += layer.Height;
					while (fy >= layer.Height) fy -= layer.Height;

					baseX = (ix << 4) - TileBaseX;
					baseY = (iy << 4) - TileBaseY + layer.TileOffsetY[fx];

					tile = layer.Tiles[fx + fy * layer.Width];

					int colTypeA = ((tile >> 12) & 3);
					int colTypeB = ((tile >> 14) & 3);

					fullFlip = (tile >> 10) & 3;
					tile = tile & 0x3FF;

					if (tile != BlankTile) {
						anID = Data->isAnims[tile] & 0xFF;
						if (anID != 0xFF)
							G->DrawSprite(AnimTileSprite, Data->isAnims[tile] >> 8, Data->animatedTileFrames[anID], baseX + 8, baseY + 8, 0, fullFlip);
						else
							G->DrawSprite(TileSprite, 0, tile, baseX + 8, baseY + 8, 0, fullFlip);

						if (ViewTileCollision) {
							flipX = ((fullFlip >> 0) & 1);
							flipY = ((fullFlip >> 1) & 1);

							for (int c = 0; c < 16; c++) {
								int eex = c;
								if (flipX)
									eex = 15 - c;

								int h1 = Data->tiles1[tile].Collision[c];
								int h2 = Data->tiles2[tile].Collision[c];

								if (BlankTile == 0x3FF && (Data->tiles1[tile].IsCeiling || Data->tiles2[tile].IsCeiling)) {
									h1 = 15 - h1;
									h2 = 15 - h2;
								}

								if (Player->Layer == 0 && (colTypeA & 1)) {
									uint32_t col = colTypeA == 3 ? 0 : colTypeA == 2 ? 0xFFFF00 : 0xFFFFFF;

									if (Data->tiles1[tile].HasCollision[c]) {
										if (Data->tiles1[tile].IsCeiling ^ flipY) {
											G->DrawRectangle(baseX + eex, baseY, 1, 16 - h1, col);
										}
										else {
											G->DrawRectangle(baseX + eex, baseY + h1, 1, 16 - h1, col);
										}
									}
								} 
								else if (Player->Layer == 1 && (colTypeB & 1)) {
									if (Data->tiles2[tile].HasCollision[c]) {
										uint32_t col = colTypeB == 3 ? 0 : colTypeB == 2 ? 0xFFFF00 : 0xFFFFFF;

										if (Data->tiles2[tile].IsCeiling ^ flipY) {
											G->DrawRectangle(baseX + eex, baseY, 1, 16 - h2, col);
										}
										else {
											G->DrawRectangle(baseX + eex, baseY + h2, 1, 16 - h2, col);
										}
									}
								}
							}

							int mx = App->Input->MouseX;
							int my = App->Input->MouseY;
							if (mx >= baseX && my >= baseY && mx < baseX + 16 && my < baseY + 16) {
								
							}
						}
					}

					ix++;
					if (ix >= tw) {
						ix = tBX;
						iy++;
					}
				}

                if (layer.Info[0].HeatWaveEnabled) {
                    if (ManiaLevel)
                        G->DoDeform = false;
                }
            }
        }

        // Rendering above background
		/*
        if (l == Data->cameraLayer - 1)
            RenderAboveBackground();
		//*/

        G->DoDeform = DeformObjects;
        

        // Rendering objects
        for (int i = 0; i < ObjectCount; i++) {
            Object* obj = Objects[i];
            //if (obj->Active && (obj->OnScreen || obj->Priority)) {
			if (obj == NULL) {
				break;
			}
			if (obj->Active && obj->OnScreen) {
                if (l == Data->cameraLayer + obj->VisualLayer) {
                    obj->Render(CameraX, CameraY);
                }
            }
        }

        // Rendering rings
        if (l == Data->cameraLayer)
            RenderRings();

        // Rendering temporary sprites
        for (vector<Object*>::iterator it = Explosions.begin(); it != Explosions.end(); ++it) {
            if ((*it)->Active)
				if (l == Data->cameraLayer + (*it)->VisualLayer)
					(*it)->Render(CameraX, CameraY);
        }

        G->DoDeform = DeformPlayer;

        // Rendering players
        for (int p = PlayerCount - 1; p >= 0; p--) {
            if (l == Data->cameraLayer + Players[p]->VisualLayer)
                Players[p]->Render(CameraX, CameraY);
        }

        // Rendering above foreground
		/*
        if (l == Data->layerCount - 1)
            RenderAboveForeground();
		//*/
    }

    G->DoDeform = false;

    if (ZoneID == 2) {
        G->DrawModeOverlay = true;
        G->SetDrawAlpha(0x80);
        if (WaterLevel - CameraY >= -16 &&
            WaterLevel - CameraY < App->HEIGHT + 16) {
            for (int x = CameraX; x <= CameraX + App->WIDTH + 32; x += 32) {
                G->DrawSprite(WaterSprite, 0, WaterAnimationFrame >> 8,
                    (x & 0xFFFFFE0) - CameraX, WaterLevel - CameraY, 0, IE_NOFLIP);
            }
        }
        G->DrawModeOverlay = false;
        G->SetDrawAlpha(0xFF);
    }

    RenderHUD();
    
    if (!ViewPlayerUpdateStats && !maxLayer) {
        int Y = 0;
        char tempStr[256];
        G->DrawRectangle(0, 0, 64, 16, 0);
        sprintf(tempStr, "%04X%04X", Player->EZX, Player->EZY);
        G->DrawTextShadow(0, Y, tempStr, 0xFFFFFF);
        Y += 8;
        sprintf(tempStr, "%04X%04X", CameraX, CameraY);
        G->DrawTextShadow(0, Y, tempStr, 0xFFFFFF);
    }

	if (ViewPalettes) {
        char palettetitle[16];
        sprintf(palettetitle, "Normal");
        G->DrawTextShadow(App->WIDTH - 132, App->HEIGHT - 74, palettetitle, 0xFFFFFF);

        sprintf(palettetitle, "Alt");
        G->DrawTextShadow(App->WIDTH - 64, App->HEIGHT - 74, palettetitle, 0xFFFFFF);

        for (int i = 0; i < 256; i++) {
            G->DrawRectangle(App->WIDTH - 132 + ((i & 0xF) << 2), App->HEIGHT - 64 + ((i >> 4) << 2), 4, 4, TileSprite->GetPalette(i));

            G->DrawRectangle(App->WIDTH - 64 + ((i & 0xF) << 2), App->HEIGHT - 64 + ((i >> 4) << 2), 4, 4, TileSprite->GetPaletteAlt(i));
        }

        for (int i = 0; i < 256; i++) {
            G->DrawRectangle(App->WIDTH - 132 + ((i & 0xF) << 2), App->HEIGHT - 64 + ((i >> 4) << 2) - 96, 4, 4, Player->Sprites[0]->GetPalette(i));

            G->DrawRectangle(App->WIDTH - 64 + ((i & 0xF) << 2), App->HEIGHT - 64 + ((i >> 4) << 2) - 96, 4, 4, Player->Sprites[0]->GetPaletteAlt(i));
        }
    }
    if (ViewPathswitchers) {
        for (int i = 0; i < PlaneSwitchCount; i++) {
            //int Len = 16 * PlaneSwitchers[i].Size + 16; // this is the real len, below is radius
            int Len = 8 * PlaneSwitchers[i].Size + 8;

            float cos_ = Player->Sin[PlaneSwitchers[i].Angle];
            float sin_ = Player->Cos[PlaneSwitchers[i].Angle];

            int cos_t = int(cos_ * Len);
            int sin_t = int(sin_ * Len);

            int ox = PlaneSwitchers[i].X;
            int oy = PlaneSwitchers[i].Y;

            int px = int(ox + sin_ * 8);
            int py = int(oy + cos_ * 8);

            G->DrawLine(
                px - cos_t - CameraX,
                py - sin_t - CameraY,
                px + cos_t - CameraX,
                py + sin_t - CameraY, ((PlaneSwitchers[i].Flags >> 3) & 1) ? 0xFF0000 : 0x0000FF);

            px = int(ox + sin_ * 6);
            py = int(oy + cos_ * 6);

            G->DrawLine(
                px - cos_t - CameraX,
                py - sin_t - CameraY,
                px + cos_t - CameraX,
                py + sin_t - CameraY, ((PlaneSwitchers[i].Flags >> 2) & 1) ? 0xFF0000 : 0x0000FF);

            px = int(ox + sin_ * -8);
            py = int(oy + cos_ * -8);

            G->DrawLine(
                px - cos_t - CameraX,
                py - sin_t - CameraY,
                px + cos_t - CameraX,
                py + sin_t - CameraY, ((PlaneSwitchers[i].Flags >> 1) & 1) ? 0xFF0000 : 0x0000FF);

            px = int(ox + sin_ * -6);
            py = int(oy + cos_ * -6);

            G->DrawLine(
                px - cos_t - CameraX,
                py - sin_t - CameraY,
                px + cos_t - CameraX,
                py + sin_t - CameraY, ((PlaneSwitchers[i].Flags) & 1) ? 0xFF0000 : 0x0000FF);

            char ghgh[3];
            sprintf(ghgh, "%02X", PlaneSwitchers[i].Angle & 0xFF);
            G->DrawTextShadow(PlaneSwitchers[i].X - 8 - CameraX, PlaneSwitchers[i].Y - CameraY - 4, ghgh, PlaneSwitchers[i].OnPath ? 0xF2D141 : 0xFFFFFF);
        }
    }
    if (ViewPlayerStats) {
		int Y = 0;

		if (ViewPlayerUpdateStats) {
			char tempStr[256];
			G->DrawRectangle(0, 0, 200, 35, 0);
			for (int i = 0; i < PlayerCount; i++) {
				sprintf(tempStr, "Player %d Update: %0.2f ms", i, playerUpdateTimers[i]);
				G->DrawTextShadow(0, Y, tempStr, 0xFFFFFF);
				Y += 8;
				sprintf(tempStr, "    Late Update: %0.2f ms", playerLateUpdateTimers[i]);
				G->DrawTextShadow(0, Y, tempStr, 0xFFFFFF);
				Y += 8;
				Y += 2;
			}
		}

        int16_t X = Player->EZX;
        Y = Player->EZY;

        Y -= 32;
        char pooerp[256];

        if (X < CameraX)
            X = CameraX;
        if (Y < CameraY)
            Y = CameraY;

        ///*
        sprintf(pooerp, "%04X %04X (%02X %d)", Player->EZX, Player->EZY, Player->Angle, Player->AngleMode);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;

        sprintf(pooerp, "%04X %04X", CameraX, CameraY);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;

        sprintf(pooerp, "Sensor A: %d", Player->SensorA);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0x00FF00);
        Y += 8;

        sprintf(pooerp, "Sensor B: %d", Player->SensorB);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0x00FF99);
        Y += 8;

        sprintf(pooerp, "Sensor C: %d", Player->SensorC);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0x0099FF);
        Y += 8;

        sprintf(pooerp, "Sensor D: %d", Player->SensorD);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFF00);
        Y += 8;

        sprintf(pooerp, "Sensor E: %d", Player->SensorE);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFF00FF);
        Y += 8;

        sprintf(pooerp, "Sensor F: %d", Player->SensorF);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFF4444);
        Y += 8;

        sprintf(pooerp, "Ground: %s", Player->Ground ? "TRUE" : "FALSE");
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;

        sprintf(pooerp, "GSP: %X", Player->GroundSpeed);
        if (Player->GroundSpeed >= 0)
            sprintf(pooerp, "GSP: %X", Player->GroundSpeed);
        else
            sprintf(pooerp, "GSP: -%X", -Player->GroundSpeed);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;

        sprintf(pooerp, "Action: %X", (int)Player->Action);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;

        if (Player->XSpeed >= 0)
            sprintf(pooerp, "XSP: %X", Player->XSpeed);
        else
            sprintf(pooerp, "XSP: -%X", -Player->XSpeed);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFF4444);
        Y += 8;

        if (Player->YSpeed >= 0)
            sprintf(pooerp, "YSP: %X", Player->YSpeed);
        else
            sprintf(pooerp, "YSP: -%X", -Player->YSpeed);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0x8888FF);
        Y += 8;

        sprintf(pooerp, "Layer: %d", Player->Layer);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;

        sprintf(pooerp, "Frame: %d", Player->CurrentFrame / 0x100);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;

        sprintf(pooerp, "UnderwaterTimer: %X", Player->UnderwaterTimer);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;
        //*/

        sprintf(pooerp, "LevelTriggerFlag: %X", LevelTriggerFlag);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;
        sprintf(pooerp, "RoutineNumber: %X", RoutineNumber);
        G->DrawTextShadow(X - CameraX + 37 - 1, Y - CameraY - 1, pooerp, 0xFFFFFF);
        Y += 8;
    }
    if (ViewTileInfo) {
        int ang = IMath::atanHex(App->Input->MouseX - App->WIDTH / 2, App->Input->MouseY - App->HEIGHT / 2);

        char poop[50];
        sprintf(poop, "Angle %02X", ang);
        G->DrawTextShadow(4, 120, poop, 0xFFFF00);

        G->DrawLine(App->WIDTH / 2, App->HEIGHT / 2, App->WIDTH / 2 + (IMath::cosHex(Frame >> 2) >> 11), App->HEIGHT / 2 + (IMath::sinHex(Frame >> 2) >> 11), 0xFFFFFF);
        G->DrawLine(App->WIDTH / 2, App->HEIGHT / 2, App->WIDTH / 2 + (IMath::cosHex(ang) >> 10), App->HEIGHT / 2 + (IMath::sinHex(ang) >> 10), 0xFF00FF);

        G->DrawLine(App->Input->MouseX - 10, App->Input->MouseY - 10, App->Input->MouseX + 10, App->Input->MouseY + 10, 0xFF0000);
        G->DrawLine(App->Input->MouseX + 10, App->Input->MouseY - 10, App->Input->MouseX - 10, App->Input->MouseY + 10, 0xFF0000);

        int mx = App->Input->MouseX + CameraX;
        int my = App->Input->MouseY + CameraY;
        if (maxLayer) {
            //mx += 8;
            //my += 8;

            mx &= ~0xF;
            my &= ~0xF;
        }
        if (App->Input->MousePressed) {
            DEBUG_MOUSE_X = mx;
            DEBUG_MOUSE_Y = my;
        }
        else  if (App->Input->MouseReleased) {
            DEBUG_MOUSE_X = -1;
            DEBUG_MOUSE_Y = -1;
        }
        if (DEBUG_MOUSE_X == -1) {
            sprintf(poop, "X %d Y %d (%04X %04X)", mx, my, mx, my);
        }
        else {
            sprintf(poop, "X %d Y %d (%d %d)", mx, my, mx - DEBUG_MOUSE_X, my - DEBUG_MOUSE_Y);
            G->DrawRectangleStroke(DEBUG_MOUSE_X - CameraX, DEBUG_MOUSE_Y - CameraY, mx - DEBUG_MOUSE_X, my - DEBUG_MOUSE_Y, 0xFF00FF);
        }

        int dx = App->Input->MouseX;
        if (dx + (int)strlen(poop) * 8 >= App->WIDTH - 3)
            dx = App->WIDTH - 3 - strlen(poop) * 8;
        //G->DrawTextShadow(dx, App->Input->MouseY - 8, poop, 0xFFFFFF);

        mx >>= 4;
        my >>= 4;
        for (int l = 0; l < Data->layerCount; l++) {
            Layer layer = Data->layers[l];
            int tile = layer.Tiles[mx + my * layer.Width];
            // bool flipX = ((tile >> 10) & 1);
            // bool flipY = ((tile >> 11) & 1);
            tile = tile & 0x3FF;

            sprintf(poop, "%s: %04X", layer.Name, tile);
            G->DrawTextShadow(0, l * 8, poop, 0xFFFFFF);
        }
    }

    if (highlightedTile > 0) {
        int which[4];

        which[0] = Data->tiles1[highlightedTile].Config[0];
        which[1] = Data->tiles1[highlightedTile].Config[1];
        which[2] = Data->tiles1[highlightedTile].Config[2];
        which[3] = Data->tiles1[highlightedTile].Config[3];

        if (which[1] >= 0xC0 && which[1] > 0xD0) which[1] = 0xC0;

        char tex[7];
        sprintf(tex, "%02X %02X %02X %02X",
            which[0],
            which[1],
            which[2],
            which[3]);
        G->DrawTextShadow(64, 64, tex, (Data->tiles1[highlightedTile].IsCeiling ? 0xF2D141 : 0xFFFFFF) - (flipY ? 0xD100 : 0));
    }

    RenderResults();

    int filTemp = G->GetFilter();
    G->SetFilter(0);
    RenderTitleCard();
    G->SetFilter(filTemp);
}

PUBLIC VIRTUAL void LevelScene::Render() {
    if (App->NextScene) return;

    int tCamY = CameraY;
    if (Data) {
        if (CameraX < 0)
            CameraX = 0;
        if (!Data->layers[Data->cameraLayer].IsScrollingVertical && CameraY < 0)
            CameraY = 0;

        if (ShakeTimer > 0) {
            int Shaking[0x14] = {
                1, -1, 1, -1,
                2, -2, 2, -2,
                3, -3, 3, -3,
                4, -4, 4, -4,
                5, -5, 5, -5
            };
            CameraY = tCamY + Shaking[ShakeTimer];
        }
        else if (ShakeTimer < 0) {
            int Shaking[0x40] = {
                1,  2,  1,  3,  1,  2,  2,  1,  2,  3,  1,  2,  1,  2,  0,  0,
                2,  0,  3,  2,  2,  3,  2,  2,  1,  3,  0,  0,  1,  0,  1,  3,
                1,  2,  1,  3,  1,  2,  2,  1,  2,  3,  1,  2,  1,  2,  0,  0,
                2,  0,  3,  2,  2,  3,  2,  2,  1,  3,  0,  0,  1,  0,  1,  3
            };
            CameraY = tCamY + Shaking[Frame & 0x3F];
        }
        if (!Data->layers[Data->cameraLayer].IsScrollingVertical && CameraY > Data->layers[Data->cameraLayer].Height * 16 - App->HEIGHT)
        CameraY = Data->layers[Data->cameraLayer].Height * 16 - App->HEIGHT;
    }

    if (FadeAction != 0)
        G->SetFilter(G->GetFilter() | 0x4);
    else
        G->SetFilter(G->GetFilter() & ~0x4);

	if (!G->HaveClone) {
		G->SetFilter(G->GetFilter() | PauseFinished);
		RenderEverything();
		G->SetFilter(G->GetFilter() & ~PauseFinished);

        // if (PauseFinished)
        //     G->MakeClone();
    }
    else {
        G->DrawClone();
    }

    CameraY = tCamY;

    RenderPauseScreen();

    if (ViewTileCollision) {
        bool col = CollisionAt(CameraX + App->Input->MouseX, CameraY + App->Input->MouseY);
        G->DrawRectangle(App->Input->MouseX - 7, App->Input->MouseY, 15, 1, col ? 0x00FF00 : 0xFF00FF);
        G->DrawRectangle(App->Input->MouseX, App->Input->MouseY - 7, 1, 15, col ? 0x00FF00 : 0xFF00FF);
    }

    G->SetFilter(0);
}

PUBLIC VIRTUAL void LevelScene::Cleanup() {
    #define CLEANUP(name) if (name) { name->Cleanup(); delete name; name = NULL; }

    App->Audio->ClearMusic();
    Sound::SoundBank[0]->Cleanup();
    delete Sound::SoundBank[0];
    Sound::SoundBank[0] = NULL;

    for (int i = 0; i < Data->layerCount; i++) {
        free(Data->layers[i].Info);
        free(Data->layers[i].Tiles);
        free(Data->layers[i].ScrollIndexes);
    }
    delete Data;

    bool ClearedKnuxSprite = false;

    if (KnuxSprite[0]) {
        ClearedKnuxSprite = true;
        for (int i = 0; i < 5; i++)
            CLEANUP(KnuxSprite[i]);
    }

    for (int i = 0; i < PlayerCount; i++) {
        if (Players[i]->Character != CharacterType::Knuckles || !ClearedKnuxSprite) {
            for (int s = 0; s < 8; s++) {
                CLEANUP(Players[i]->Sprites[s]);
            }
        }
        CLEANUP(Players[i]->SpriteShields);
        CLEANUP(Players[i]->SpriteShields2);
        delete Players[i];
    }

    for (int i = 0; i < ObjectCount; i++) {
        delete Objects[i];
    }
    ObjectCount = 0;

    free(Objects);
    free(ObjectsSolid);
    free(ObjectsSpring);
    free(ObjectsEnemies);
    free(ObjectsBreakable);
    free(ObjectsPathSwitcher);
    free(ObjectProps);
    free(RingProps);
    free(AnimatedSprite0Props);
    free(AnimatedSprite1Props);
    free(SoundBank);
    free(SpriteMapIDs);

    CLEANUP(TileSprite);
    CLEANUP(GiantRingModel);
    CLEANUP(PauseSprite);
    CLEANUP(GlobalDisplaySprite);
    CLEANUP(MobileButtonsSprite);
    CLEANUP(ItemsSprite);
    CLEANUP(AnimalsSprite);
    CLEANUP(ObjectsSprite);
    CLEANUP(Objects2Sprite);
    CLEANUP(Objects3Sprite);
    CLEANUP(RobotnikSprite);
    CLEANUP(ExplosionSprite);
    CLEANUP(WaterSprite);

    free(PlaneSwitchers);

    //*/
}
