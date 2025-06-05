#include "modding.h"
#include "global.h"
#include "recompconfig.h"

#include "overlays/actors/ovl_Bg_Dblue_Movebg/z_bg_dblue_movebg.h"

#define PUSH_SWITCH_SPEED recomp_get_config_u32("push_switch_speed")

extern void func_80A2A670(BgDblueMovebg *this, PlayState *play);
extern void func_80A2AAB8(BgDblueMovebg *this, PlayState *play);
extern s32 func_80A29A80(PlayState *play, s32 switchFlagBase, s32 arg2);

BgDblueMovebg* mThis;
PlayState* mPlay;

RECOMP_HOOK("func_80A2A444") void func_80A2A444_Hook(BgDblueMovebg* this, PlayState* play) {
    mThis = this;
    mPlay = play;
}

RECOMP_HOOK_RETURN("func_80A2A444") void func_80A2A444_Return() {
    Player* player = GET_PLAYER(mPlay);
    s32 sp20;
    Player* temp_a3;
    s16 temp_v0;

    mThis->unk_188 = PUSH_SWITCH_SPEED;

    sp20 = Math_StepToS(&mThis->unk_18A, 900, mThis->unk_188);
    temp_v0 = mThis->unk_18A * mThis->unk_17E;
    mThis->dyna.actor.shape.rot.y =
        (s32)DEG_TO_BINANG_ALT3((mThis->unk_18C + temp_v0) * 0.1f) + mThis->dyna.actor.home.rot.y;

    if ((player->stateFlags2 & PLAYER_STATE2_10) && (mThis->unk_184 > 0.0f)) {
        player->actor.world.pos.x =
            (Math_SinS(mThis->dyna.actor.shape.rot.y - mThis->unk_18E) * mThis->unk_184) + mThis->dyna.actor.home.pos.x;
        player->actor.world.pos.z =
            (Math_CosS(mThis->dyna.actor.shape.rot.y - mThis->unk_18E) * mThis->unk_184) + mThis->dyna.actor.home.pos.z;
    } else {
        mThis->unk_184 = 0.0f;
    }

    if (sp20) {
        player->stateFlags2 &= ~PLAYER_STATE2_10;
        mThis->dyna.pushForce = 0.0f;
        Flags_SetSwitch(mPlay, mThis->switchFlag);
        Actor_PlaySfx(&mThis->dyna.actor, NA_SE_EV_STONEDOOR_STOP);

        if (func_80A29A80(mPlay, mThis->unk_1C8, mThis->unk_1C4)) {
            mThis->unk_172 |= 1;
        } else {
            mThis->unk_172 &= ~1;
        }

        if (!(mThis->unk_174 & 1) && (mThis->unk_172 & 1)) {
            Lib_PlaySfx_2(NA_SE_EV_PIPE_STREAM_START);
        }
        func_80A2A670(mThis, mPlay);
    } else {
        Actor_PlaySfx_Flagged(&mThis->dyna.actor, NA_SE_EV_COCK_SWITCH_ROLL - SFX_FLAG);
    }
}

RECOMP_HOOK("func_80A2A7F8") void func_80A2A7F8_Hook(BgDblueMovebg* this, PlayState* play) {
    mThis = this;
    mPlay = play;
}

RECOMP_HOOK_RETURN("func_80A2A7F8") void func_80A2A7F8_Return() {
    Player* player = GET_PLAYER(mPlay);
    s32 sp28;
    s16 sp26;
    s32 temp;

    mThis->unk_188 = PUSH_SWITCH_SPEED;

    sp28 = Math_StepToS(&mThis->unk_18A, 900, mThis->unk_188);
    sp26 = mThis->unk_18A * mThis->unk_17E;
    mThis->dyna.actor.shape.rot.y = (s32)DEG_TO_BINANG_ALT3((mThis->unk_18C + sp26) * 0.1f) + mThis->dyna.actor.home.rot.y;

    if ((player->stateFlags2 & PLAYER_STATE2_10) && (mThis->unk_184 > 0.0f)) {
        player->actor.world.pos.x =
            (Math_SinS(mThis->dyna.actor.shape.rot.y - mThis->unk_18E) * mThis->unk_184) + mThis->dyna.actor.home.pos.x;
        player->actor.world.pos.z =
            (Math_CosS(mThis->dyna.actor.shape.rot.y - mThis->unk_18E) * mThis->unk_184) + mThis->dyna.actor.home.pos.z;
    } else {
        mThis->unk_184 = 0.0f;
    }

    if (sp28) {
        temp = (mThis->unk_18C + sp26 + 3600) % 3600;

        if ((temp == 900) || (temp == 2700)) {
            Flags_SetSwitch(mPlay, mThis->switchFlag);
        } else {
            Flags_UnsetSwitch(mPlay, mThis->switchFlag);
        }

        player->stateFlags1 |= PLAYER_STATE1_20;
        player->stateFlags2 &= ~PLAYER_STATE2_10;
        mThis->dyna.pushForce = 0.0f;

        mThis->unk_18C = (mThis->unk_18C + sp26 + 3600) % 3600;
        mThis->unk_188 = 0;
        mThis->unk_18A = 0;
        mThis->unk_17E = 0;

        Actor_PlaySfx(&mThis->dyna.actor, NA_SE_EV_STONEDOOR_STOP);

        if (func_80A29A80(mPlay, mThis->unk_1C8, mThis->unk_1C4)) {
            mThis->unk_172 |= 1;
        } else {
            mThis->unk_172 &= ~1;
        }

        if (!(mThis->unk_174 & 1) && (mThis->unk_172 & 1)) {
            Lib_PlaySfx_2(NA_SE_EV_PIPE_STREAM_START);
        }

        mThis->unk_174 = mThis->unk_172;
        mThis->unk_1D0 = 17;
        mThis->actionFunc = func_80A2AAB8;
    } else {
        Actor_PlaySfx_Flagged(&mThis->dyna.actor, NA_SE_EV_COCK_SWITCH_ROLL - SFX_FLAG);
    }
}