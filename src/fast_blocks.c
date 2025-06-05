#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"

#define PUSH_SPEED recomp_get_config_u32("push_speed")

#define PUSHBLOCK_PUSH           (1 << 4)
#define PUSHBLOCK_SETUP_PUSH     (1 << 5)

struct ObjOshihiki;
typedef void (*ObjOshihikiActionFunc)(struct ObjOshihiki*, PlayState*);

typedef struct ObjOshihiki {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ObjOshihikiActionFunc actionFunc;
    /* 0x160 */ u16 stateFlags;
    /* 0x162 */ s16 timer;
    /* 0x164 */ f32 yawSin;
    /* 0x168 */ f32 yawCos;
    /* 0x16C */ f32 pushSpeed;
    /* 0x170 */ f32 pushDist;
    /* 0x174 */ f32 direction;
    /* 0x178 */ s32 floorBgIds[5];
    /* 0x18C */ CollisionPoly* floorPolys[5];
    /* 0x1A0 */ f32 floorHeights[5];
    /* 0x1B4 */ s16 highestFloor;
    /* 0x1B6 */ u8 cantMove;
    /* 0x1B8 */ struct ObjOshihiki* blockUnder;
    /* 0x1BC */ f32 underDistX;
    /* 0x1C0 */ f32 underDistZ;
    /* 0x1C4 */ s8 textureStep;
    /* 0x1C5 */ Color_RGB8 color;
    /* 0x1C8 */ AnimatedMaterial* texture;
} ObjOshihiki; // size = 0x1CC

extern s32 ObjOshihiki_CheckFloor(ObjOshihiki* this, PlayState* play);
extern void ObjOshihiki_SetupFall(ObjOshihiki* this, PlayState* play);
extern s32 ObjOshihiki_CheckWall(PlayState* play, s16 angle, f32 direction, ObjOshihiki* this);
extern void ObjOshihiki_SetupOnScene(ObjOshihiki* this, PlayState* play);
extern void ObjOshihiki_SetupOnActor(ObjOshihiki* this, PlayState* play);

extern void ObjOshihiki_Push(ObjOshihiki* this, PlayState* play);

ObjOshihiki* gThis;
PlayState* gPlay;
RECOMP_HOOK("ObjOshihiki_Push") void ObjOshihiki_Push_Hook(ObjOshihiki* this, PlayState* play) {
    gThis = this;
    gPlay = play;

    this->pushSpeed = this->pushSpeed + (PUSH_SPEED * 0.25) + 0.5f;
    this->pushSpeed = CLAMP_MAX(this->pushSpeed, 2.0f + (PUSH_SPEED * 0.5));
}

RECOMP_HOOK_RETURN("ObjOshihiki_Push") void ObjOshihiki_Push_Return() {
    gThis->timer = 10 - ((PUSH_SPEED * 3) / 2);
}