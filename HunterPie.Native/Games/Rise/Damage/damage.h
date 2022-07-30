#pragma once
#include "../../../pch.h"
#include "../Common/Monster.h"

using namespace Games::Rise::Common;

namespace Games
{
    namespace Rise
    {
        namespace Damage
        {
            struct MHREntityData
            {
                intptr_t unk0;
                int32_t unk1;
                int32_t unk2;
                int32_t unk3;
                float rawDamage;
                float elementalDamage;
                int unk4;
                float unk5;
                float unk6;
                float unk7;
                float unk8;
                float unk9;
                float unk10;
                float unk11;
                float unk12;
                uint8_t padding[112]; // A lot of values, but we gonna keep an array here since I don't want to map them all
                int attackerDamageType;
                int unk14;
                int unk15;
                int Id;
                uint8_t padding_tail[100];
            };

            using fnCalculateEntityDamage = MHREntityData* (*)(
                intptr_t arg1,
                Monster* target,
                intptr_t arg3,
                intptr_t arg4,
                intptr_t arg5,
                void* arg6
            );

            using fnDealDamageToEntity = uintptr_t** (*)(
                intptr_t** arg1,
                intptr_t* arg2,
                UnkMonster arg3,
                int arg4,
                int arg5,
                int arg6
            );

            using fnHitTarget = MHREntityData* (*)(
                intptr_t arg1,
                Monster* arg2,
                intptr_t arg3
            );

            using fnUnkButInteresting = intptr_t(*)(
                intptr_t arg1,
                Monster* target,
                MHREntityData* damageData
            );

            static MHREntityData* (*ogHitTarget)(
                intptr_t arg1,
                Monster* arg2,
                intptr_t arg3
            );

            static MHREntityData* (*ogCalculateEntityDamage)(
                intptr_t arg1,
                Monster* target,
                intptr_t arg3,
                intptr_t arg4,
                intptr_t arg5,
                void* arg6
            );

            static uintptr_t** (*ogDealDamageToEntity)(
                intptr_t** arg1,
                intptr_t* arg2,
                UnkMonster arg3,
                int arg4,
                int arg5,
                int arg6
            );

            static intptr_t (*ogUnkButInteresting)(
                intptr_t arg1,
                Monster* target,
                MHREntityData* damageData
            );
        }
    }
}