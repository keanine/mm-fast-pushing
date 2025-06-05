#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "overlays\actors\ovl_Obj_Oshihiki\z_obj_oshihiki.h"

#define PUSH_BLOCK_SPEED recomp_get_config_u32("push_block_speed")

ObjOshihiki* gThis;
PlayState* gPlay;
RECOMP_HOOK("ObjOshihiki_Push") void ObjOshihiki_Push_Hook(ObjOshihiki* this, PlayState* play) {
    gThis = this;
    gPlay = play;

    this->pushSpeed = this->pushSpeed + (PUSH_BLOCK_SPEED * 0.25) + 0.5f;
    this->pushSpeed = CLAMP_MAX(this->pushSpeed, 2.0f + (PUSH_BLOCK_SPEED * 0.5));
}

RECOMP_HOOK_RETURN("ObjOshihiki_Push") void ObjOshihiki_Push_Return() {
    gThis->timer = 10 - ((PUSH_BLOCK_SPEED * 3) / 2);
}