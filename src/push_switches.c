#include "modding.h"
#include "global.h"
#include "recompconfig.h"

#include "overlays/actors/ovl_Bg_Dblue_Movebg/z_bg_dblue_movebg.h"

#define PUSH_SWITCH_SPEED recomp_get_config_u32("push_switch_speed")

extern void func_80A2A670(BgDblueMovebg *this, PlayState *play);
extern void func_80A2AAB8(BgDblueMovebg *this, PlayState *play);
extern s32 func_80A29A80(PlayState *play, s32 switchFlagBase, s32 arg2);

RECOMP_PATCH void func_80A2A444(BgDblueMovebg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 sp20;
    Player* temp_a3;
    s16 temp_v0;

    this->unk_188 = PUSH_SWITCH_SPEED;

    sp20 = Math_StepToS(&this->unk_18A, 900, this->unk_188);
    temp_v0 = this->unk_18A * this->unk_17E;
    this->dyna.actor.shape.rot.y =
        (s32)DEG_TO_BINANG_ALT3((this->unk_18C + temp_v0) * 0.1f) + this->dyna.actor.home.rot.y;

    if ((player->stateFlags2 & PLAYER_STATE2_10) && (this->unk_184 > 0.0f)) {
        player->actor.world.pos.x =
            (Math_SinS(this->dyna.actor.shape.rot.y - this->unk_18E) * this->unk_184) + this->dyna.actor.home.pos.x;
        player->actor.world.pos.z =
            (Math_CosS(this->dyna.actor.shape.rot.y - this->unk_18E) * this->unk_184) + this->dyna.actor.home.pos.z;
    } else {
        this->unk_184 = 0.0f;
    }

    if (sp20) {
        player->stateFlags2 &= ~PLAYER_STATE2_10;
        this->dyna.pushForce = 0.0f;
        Flags_SetSwitch(play, this->switchFlag);
        Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_STONEDOOR_STOP);

        if (func_80A29A80(play, this->unk_1C8, this->unk_1C4)) {
            this->unk_172 |= 1;
        } else {
            this->unk_172 &= ~1;
        }

        if (!(this->unk_174 & 1) && (this->unk_172 & 1)) {
            Lib_PlaySfx_2(NA_SE_EV_PIPE_STREAM_START);
        }
        func_80A2A670(this, play);
    } else {
        Actor_PlaySfx_Flagged(&this->dyna.actor, NA_SE_EV_COCK_SWITCH_ROLL - SFX_FLAG);
    }
}

RECOMP_PATCH void func_80A2A7F8(BgDblueMovebg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 sp28;
    s16 sp26;
    s32 temp;

    this->unk_188 = PUSH_SWITCH_SPEED;

    sp28 = Math_StepToS(&this->unk_18A, 900, this->unk_188);
    sp26 = this->unk_18A * this->unk_17E;
    this->dyna.actor.shape.rot.y = (s32)DEG_TO_BINANG_ALT3((this->unk_18C + sp26) * 0.1f) + this->dyna.actor.home.rot.y;

    if ((player->stateFlags2 & PLAYER_STATE2_10) && (this->unk_184 > 0.0f)) {
        player->actor.world.pos.x =
            (Math_SinS(this->dyna.actor.shape.rot.y - this->unk_18E) * this->unk_184) + this->dyna.actor.home.pos.x;
        player->actor.world.pos.z =
            (Math_CosS(this->dyna.actor.shape.rot.y - this->unk_18E) * this->unk_184) + this->dyna.actor.home.pos.z;
    } else {
        this->unk_184 = 0.0f;
    }

    if (sp28) {
        temp = (this->unk_18C + sp26 + 3600) % 3600;

        if ((temp == 900) || (temp == 2700)) {
            Flags_SetSwitch(play, this->switchFlag);
        } else {
            Flags_UnsetSwitch(play, this->switchFlag);
        }

        player->stateFlags1 |= PLAYER_STATE1_20;
        player->stateFlags2 &= ~PLAYER_STATE2_10;
        this->dyna.pushForce = 0.0f;

        this->unk_18C = (this->unk_18C + sp26 + 3600) % 3600;
        this->unk_188 = 0;
        this->unk_18A = 0;
        this->unk_17E = 0;

        Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_STONEDOOR_STOP);

        if (func_80A29A80(play, this->unk_1C8, this->unk_1C4)) {
            this->unk_172 |= 1;
        } else {
            this->unk_172 &= ~1;
        }

        if (!(this->unk_174 & 1) && (this->unk_172 & 1)) {
            Lib_PlaySfx_2(NA_SE_EV_PIPE_STREAM_START);
        }

        this->unk_174 = this->unk_172;
        this->unk_1D0 = 17;
        this->actionFunc = func_80A2AAB8;
    } else {
        Actor_PlaySfx_Flagged(&this->dyna.actor, NA_SE_EV_COCK_SWITCH_ROLL - SFX_FLAG);
    }
}