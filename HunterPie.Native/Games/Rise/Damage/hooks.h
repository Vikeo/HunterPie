#pragma once
#include "damage.h"
#include "../Common/Monster.h"
#include "../../../Core/Interfaces/IInitializable.h"

using namespace Core;
using namespace Games::Rise::Common;

namespace Games
{
    namespace Rise
    {
        namespace Damage
        {
            namespace Hook
            {
                MHREntityData* CalculateEntityDamage(
                    intptr_t arg1,
                    Monster* target,
                    intptr_t arg3,
                    intptr_t arg4,
                    intptr_t arg5,
                    void* arg6
                );

                uintptr_t** DealDamageToEntity(
                    intptr_t** arg1,
                    intptr_t* arg2,
                    UnkMonster arg3,
                    int arg4,
                    int arg5,
                    int arg6
                );

                MHREntityData* HitTarget(
                    intptr_t arg1,
                    Monster* target,
                    intptr_t arg3
                );

                class DamageHooks : public IInitializable
                {
                public:
                    virtual bool Init(uintptr_t* pointers);
                };
            }
        }
    }
}