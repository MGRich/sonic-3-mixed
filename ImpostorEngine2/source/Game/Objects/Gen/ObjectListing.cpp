#include "ObjectListing.h"

Object* GetNewObjectFromID(int ID) {
    switch (ID) {
        case 0x01U:
            return new Monitor();
            break;
        case 0x03U:
            return new HollowTree();
            break;
        case 0x04U:
            return new CollapsingPlatform();
            break;
        case 0x05U:
            return new Rock();
            break;
        case 0x06U:
            return new RideVine();
            break;
        case 0x07U:
            return new Spring();
            break;
        case 0x08U:
            return new Spikes();
            break;
        case 0x09U:
            return new TreeBark();
            break;
        case 0x0AU:
            return new RopePeg();
            break;
        case 0x0CU:
            return new SwingVine();
            break;
        case 0x0DU:
            return new BreakableWall();
            break;
        case 0x0EU:
            return new Ridge();
            break;
        case 0x0FU:
            return new CollapsingBridge();
            break;
        case 0x24U:
            return new AutomaticTunnel();
            break;
        case 0x26U:
            return new RollEnforcer();
            break;
        case 0x28U:
            return new InvisibleBlock();
            break;
        case 0x29U:
            return new DisappearingFloor();
            break;
        case 0x2AU:
            return new CorkFloor();
            break;
        case 0x2BU:
            return new FlippingBridge();
            break;
        case 0x2CU:
            return new CollapsingLogBridge();
            break;
        case 0x2DU:
            return new FallingLog();
            break;
        case 0x2EU:
            return new SpikedLog();
            break;
        case 0x2FU:
            return new NonAnim();
            break;
        case 0x30U:
            return new AnimSprite();
            break;
        case 0x32U:
            return new Drawbridge();
            break;
        case 0x33U:
            return new Button();
            break;
        case 0x34U:
            return new StarPost();
            break;
        case 0x35U:
            return new ForegroundPlant();
            break;
        case 0x36U:
            return new BreakBar();
            break;
        case 0x37U:
            return new HCZVWaterStream();
            break;
        case 0x38U:
            return new Fan();
            break;
        case 0x39U:
            return new LargeFan();
            break;
        case 0x3AU:
            return new HandLauncher();
            break;
        case 0x3BU:
            return new HCZWaterWall();
            break;
        case 0x3CU:
            return new Door();
            break;
        case 0x3EU:
            return new HCZConveyorBelt();
            break;
        case 0x3FU:
            return new HCZConveyorSpike();
            break;
        case 0x40U:
            return new Block();
            break;
        case 0x41U:
            return new Balloon();
            break;
        case 0x46U:
            return new HoverFan();
            break;
        case 0x48U:
            return new VacuumTube();
            break;
        case 0x4BU:
            return new Bouncepad();
            break;
        case 0x4FU:
            return new MGZQuickMud();
            break;
        case 0x51U:
            return new FloatingPlatform();
            break;
        case 0x52U:
            return new MGZSmashingPillar();
            break;
        case 0x53U:
            return new MGZSwingingPlatform();
            break;
        case 0x54U:
            return new Bubbler();
            break;
        case 0x55U:
            return new Relief();
            break;
        case 0x57U:
            return new MGZTriggerPlatform();
            break;
        case 0x59U:
            return new MGZDashTrigger();
            break;
        case 0x5BU:
            return new TopPlatform();
            break;
        case 0x67U:
            return new HCZSnakeBlocks();
            break;
        case 0x68U:
            return new SpinningColumn();
            break;
        case 0x69U:
            return new HCZSlide();
            break;
        case 0x6AU:
            return new InvisibleSpikes();
            break;
        case 0x6BU:
            return new InvisibleDeath();
            break;
        case 0x6CU:
            return new TensionBridge();
            break;
        case 0x6DU:
            return new HCZWaterSplash();
            break;
        case 0x80U:
            return new HiddenMonitor();
            break;
        case 0x81U:
            return new EggCapsule();
            break;
        case 0x82U:
            return new CutsceneKnuckles();
            break;
        case 0x85U:
            return new SpecialRing();
            break;
        case 0x8CU:
            return new Bloominator();
            break;
        case 0x8DU:
            return new RhinoBot();
            break;
        case 0x8EU:
            return new MonkeyDude();
            break;
        case 0x8FU:
            return new CaterkillerJr();
            break;
        case 0x90U:
            return new AIZMinibossCutscene();
            break;
        case 0x91U:
            return new AIZMiniboss();
            break;
        case 0x92U:
            return new AIZBoss();
            break;
        case 0x93U:
            return new Jawz();
            break;
        case 0x94U:
            return new Blastoid();
            break;
        case 0x95U:
            return new Buggernaut();
            break;
        case 0x96U:
            return new TurboSpiker();
            break;
        case 0x97U:
            return new MegaChomper();
            break;
        case 0x98U:
            return new Pointdexter();
            break;
        case 0x99U:
            return new HCZMiniboss();
            break;
        case 0x9AU:
            return new HCZBoss();
            break;
        case 0x9BU:
            return new BubblesBadnik();
            break;
        case 0x9CU:
            return new Spiker();
            break;
        case 0x9DU:
            return new Mantis();
            break;
        case 0x9EU:
            return new MGZDriller();
            break;
        case 0x9FU:
            return new MGZMiniBoss();
            break;
        case 0xA0U:
            return new DrillerEggman();
            break;
        case 0xA1U:
            return new MGZBoss();
            break;
        case 0xA5U:
            return new Batbright();
            break;
        case 0x400U:
            return new AirshipBomb();
            break;
        case 0x401U:
            return new AIZMinibossBomb();
            break;
        case 0x402U:
            return new AIZMinibossFlame();
            break;
        case 0x403U:
            return new BloominatorSpore();
            break;
        case 0x404U:
            return new MonkeyDudeCoconut();
            break;
        case 0x405U:
            return new BatbrightParts();
            break;
        case 0x406U:
            return new BubbleAir();
            break;
        case 0x407U:
            return new PlatformOutliner();
            break;
        case 0x408U:
            return new EggCapsuleButton();
            break;
        case 0x409U:
            return new Signpost();
            break;
        case 0x40AU:
            return new TensionBridgeChild();
            break;
        case 0x40BU:
            return new ViewCollisionToggler();
            break;
        case 0x40CU:
            return new BlastoidShot();
            break;
        case 0x40DU:
            return new ReliefEye();
            break;
        case 0x40EU:
            return new Platform();
            break;

        default:
            break;
    }
    return NULL;
}

Object* GetNewObjectFromCRC32(uint32_t Hash) {
    switch (Hash) {
        case 0xBDE7E33AU:
            return new Monitor();
            break;
        case 0x1E5BFA5FU:
            return new HollowTree();
            break;
        case 0x95087B30U:
            return new CollapsingPlatform();
            break;
        case 0xFE0305DDU:
            return new Rock();
            break;
        case 0x7F20AD1DU:
            return new RideVine();
            break;
        case 0x2802B89EU:
            return new Spring();
            break;
        case 0x44BC7B0EU:
            return new Spikes();
            break;
        case 0x590E62AFU:
            return new TreeBark();
            break;
        case 0x7FB64D3AU:
            return new RopePeg();
            break;
        case 0xD691E4F3U:
            return new SwingVine();
            break;
        case 0x60436E3AU:
            return new BreakableWall();
            break;
        case 0xEC54C850U:
            return new Ridge();
            break;
        case 0x18C4AE0DU:
            return new CollapsingBridge();
            break;
        case 0xC286FB4DU:
            return new AutomaticTunnel();
            break;
        case 0x624D9DA4U:
            return new RollEnforcer();
            break;
        case 0xB4878D6AU:
            return new InvisibleBlock();
            break;
        case 0x16A80CA8U:
            return new DisappearingFloor();
            break;
        case 0x4A50924FU:
            return new CorkFloor();
            break;
        case 0xB48623AU:
            return new FlippingBridge();
            break;
        case 0xC558537EU:
            return new CollapsingLogBridge();
            break;
        case 0xBFF26300U:
            return new FallingLog();
            break;
        case 0x22DC09C3U:
            return new SpikedLog();
            break;
        case 0x10F942A2U:
            return new NonAnim();
            break;
        case 0x9B4046ECU:
            return new AnimSprite();
            break;
        case 0x5465065DU:
            return new Drawbridge();
            break;
        case 0xF72B92E4U:
            return new Button();
            break;
        case 0xC8B337E6U:
            return new StarPost();
            break;
        case 0x9D88FCCEU:
            return new ForegroundPlant();
            break;
        case 0xEFFDFB55U:
            return new BreakBar();
            break;
        case 0xD396DEF2U:
            return new HCZVWaterStream();
            break;
        case 0x7849B3C1U:
            return new Fan();
            break;
        case 0x35EA70E0U:
            return new LargeFan();
            break;
        case 0xE6E16DBCU:
            return new HandLauncher();
            break;
        case 0xEC4012B6U:
            return new HCZWaterWall();
            break;
        case 0x81EC1830U:
            return new Door();
            break;
        case 0x98E1D2FFU:
            return new HCZConveyorBelt();
            break;
        case 0xD719B2DU:
            return new HCZConveyorSpike();
            break;
        case 0x58F95E1CU:
            return new Block();
            break;
        case 0x65BE2A3AU:
            return new Balloon();
            break;
        case 0x7C977754U:
            return new HoverFan();
            break;
        case 0x177568D6U:
            return new VacuumTube();
            break;
        case 0x3B4DCB47U:
            return new Bouncepad();
            break;
        case 0x43F8965EU:
            return new MGZQuickMud();
            break;
        case 0xC3C9288BU:
            return new FloatingPlatform();
            break;
        case 0x6B9FADA2U:
            return new MGZSmashingPillar();
            break;
        case 0x3B006393U:
            return new MGZSwingingPlatform();
            break;
        case 0x8E41B4A6U:
            return new Bubbler();
            break;
        case 0xFC346615U:
            return new Relief();
            break;
        case 0x5809E61CU:
            return new MGZTriggerPlatform();
            break;
        case 0xFF451B7U:
            return new MGZDashTrigger();
            break;
        case 0xC2A459B3U:
            return new TopPlatform();
            break;
        case 0x1DBC27C1U:
            return new HCZSnakeBlocks();
            break;
        case 0x7DC95D09U:
            return new SpinningColumn();
            break;
        case 0xBECC7360U:
            return new HCZSlide();
            break;
        case 0xE40C8E50U:
            return new InvisibleSpikes();
            break;
        case 0xA0BE8640U:
            return new InvisibleDeath();
            break;
        case 0x5F02A27FU:
            return new TensionBridge();
            break;
        case 0xAFDD0C56U:
            return new HCZWaterSplash();
            break;
        case 0x8616E0BDU:
            return new HiddenMonitor();
            break;
        case 0x973691D6U:
            return new EggCapsule();
            break;
        case 0x2822B56DU:
            return new CutsceneKnuckles();
            break;
        case 0xABC2C658U:
            return new SpecialRing();
            break;
        case 0x9B85E709U:
            return new Bloominator();
            break;
        case 0x4374E9C7U:
            return new RhinoBot();
            break;
        case 0xB7C95057U:
            return new MonkeyDude();
            break;
        case 0x3F487ED4U:
            return new CaterkillerJr();
            break;
        case 0xBE856454U:
            return new AIZMinibossCutscene();
            break;
        case 0x8030D25CU:
            return new AIZMiniboss();
            break;
        case 0xCC92A8FAU:
            return new AIZBoss();
            break;
        case 0x2160C123U:
            return new Jawz();
            break;
        case 0xCD00547AU:
            return new Blastoid();
            break;
        case 0x78C3EB13U:
            return new Buggernaut();
            break;
        case 0x8E6F27CFU:
            return new TurboSpiker();
            break;
        case 0xC50988CEU:
            return new MegaChomper();
            break;
        case 0xF365CCC4U:
            return new Pointdexter();
            break;
        case 0x31F4D537U:
            return new HCZMiniboss();
            break;
        case 0x6DE7611DU:
            return new HCZBoss();
            break;
        case 0xA8CF1ABBU:
            return new BubblesBadnik();
            break;
        case 0xDF9FA830U:
            return new Spiker();
            break;
        case 0x2C7E197CU:
            return new Mantis();
            break;
        case 0xCCA7D752U:
            return new MGZDriller();
            break;
        case 0x8886B305U:
            return new MGZMiniBoss();
            break;
        case 0x88549378U:
            return new DrillerEggman();
            break;
        case 0xDD1ABC3AU:
            return new MGZBoss();
            break;
        case 0x9D8C15CAU:
            return new Batbright();
            break;
        case 0x8E8C37A1U:
            return new AirshipBomb();
            break;
        case 0x89077B11U:
            return new AIZMinibossBomb();
            break;
        case 0x3195387EU:
            return new AIZMinibossFlame();
            break;
        case 0xBF8DF98AU:
            return new BloominatorSpore();
            break;
        case 0x5C688649U:
            return new MonkeyDudeCoconut();
            break;
        case 0xD5ADBFDBU:
            return new BatbrightParts();
            break;
        case 0x11DAFFC3U:
            return new BubbleAir();
            break;
        case 0x5DFC3336U:
            return new PlatformOutliner();
            break;
        case 0xA0128E04U:
            return new EggCapsuleButton();
            break;
        case 0x6A8C340CU:
            return new Signpost();
            break;
        case 0x8ED6F84CU:
            return new TensionBridgeChild();
            break;
        case 0x27340CB0U:
            return new ViewCollisionToggler();
            break;
        case 0x3692A9F5U:
            return new BlastoidShot();
            break;
        case 0xBBCA6AC5U:
            return new ReliefEye();
            break;
        case 0xC4B304CCU:
            return new Platform();
            break;

        default:
            break;
    }
    return NULL;
}