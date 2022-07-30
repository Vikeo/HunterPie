#include "hooks.h"
#include <inttypes.h>
#include "../../../libs/MinHook/MinHook.h"
#include "../../../Core/Managers/Damage/manager.h"
#include "../../../Core/Utils/addresses.h"
#include "../Utils/RiseUtils.h"

using namespace Games::Rise::Damage;
using namespace HunterPie::Core::Damage;
using namespace Games::Rise::Damage::Hook;

uintptr_t fnCalculateEntityDamagePtr = (uintptr_t)nullptr;
uintptr_t fnDealDamageToEntityPtr = (uintptr_t)0x1438870F0;
uintptr_t fnHitTargetPtr = (uintptr_t)0x141148640;
uintptr_t fnUnkButInterestingPtr = (uintptr_t)0x14114DC90;

EntityType GetEntityByDamageType(int damageType);
bool HookFunctions();

uintptr_t UnkButInteresting(
    intptr_t arg1,
    Monster* target,
    MHREntityData* damageData
)
{
    uint8_t* raw = (uint8_t*)malloc(sizeof(MHREntityData));
    memcpy(raw, damageData, sizeof(MHREntityData));

    LOG("UnkButInteresting(%llx, %llx, %llx); Damage done: %f, Raw: %llx", arg1, target, damageData, damageData->rawDamage + damageData->elementalDamage, raw);
    return ogUnkButInteresting(arg1, target, damageData);
}

MHREntityData* Hook::HitTarget(
    intptr_t arg1,
    Monster* target,
    intptr_t arg3
)
{
    MHREntityData* damageData = ogHitTarget(arg1, target, arg3);

    if (!Utils::IsBigMonster(target->id))
        return damageData;

    //LOG("[%llx][%d:%d] HitTarget: monster id: %d damage: %f", arg3,);

    Entity entity = Entity{
        damageData->Id,
        GetEntityByDamageType(damageData->attackerDamageType)
    };

    // Each player's main pet will inherits the owner's index, but be flagged as a PET
    if (entity.type == PET && entity.index <= 3)
        entity.index = entity.index + 4 + 1;

    EntityDamageData entityData = EntityDamageData{
        (intptr_t)target,
        entity,
        damageData->rawDamage,
        damageData->elementalDamage,
    };

    DamageTrackManager::GetInstance()->UpdateDamage(entityData);

    return damageData;
}

uintptr_t** Games::Rise::Damage::Hook::DealDamageToEntity(
    intptr_t** arg1, 
    intptr_t* arg2, 
    UnkMonster arg3,
    int arg4, 
    int arg5, 
    int arg6
)
{   
    LOG("%llX %llX %llX %d %d %X", arg1, arg2, arg3, arg4, arg5, arg6);

    return ogDealDamageToEntity(arg1, arg2, arg3, arg4, arg5, arg6);
    intptr_t* damageDataRef = reinterpret_cast<intptr_t*>(arg6 - 0x180);

    Monster* target = arg3.monster;

    MHREntityData* damageData = (MHREntityData*)damageDataRef;

    //LOG("Hit monster id: %d damageData: %llX id: %d damage: %f", target->id, damageDataRef, damageData->Id, damageData->rawDamage + damageData->elementalDamage);

    Entity entity = Entity{
        damageData->Id,
        GetEntityByDamageType(damageData->attackerDamageType)
    };

    // Each player's main pet will inherits the owner's index, but be flagged as a PET
    if (entity.type == PET && entity.index <= 3)
        entity.index = entity.index + 4 + 1;

    EntityDamageData entityData = EntityDamageData{
        (intptr_t)target,
        entity,
        damageData->rawDamage,
        damageData->elementalDamage,
    };

    DamageTrackManager::GetInstance()->UpdateDamage(entityData);

    return ogDealDamageToEntity(arg1, arg2, arg3, arg4, arg5, arg6);
}

MHREntityData* Hook::CalculateEntityDamage(
    intptr_t arg1,
    Monster* target,
    intptr_t arg3,
    intptr_t arg4,
    intptr_t arg5,
    void* arg6
)
{
    MHREntityData* damageData = ogCalculateEntityDamage(
        arg1, 
        target, 
        arg3, 
        arg4, 
        arg5, 
        arg6
    );

    uint8_t* raw = (uint8_t*)malloc(sizeof(MHREntityData));
    memcpy(raw, damageData, sizeof(MHREntityData));

    LOG("CalculateEntityDamage(%llx, %llx, %llx); Damage Done: %f, Raw: %llx", arg1, target, damageData, damageData->rawDamage + damageData->elementalDamage, raw);

    if (!Utils::IsBigMonster(target->id))
        return damageData;

    //LOG("CalculateEntityDamage: Hit monster id: %d damageData: %llX", target->id, damageData);

    return damageData;

    Entity entity = Entity{
        damageData->Id,
        GetEntityByDamageType(damageData->attackerDamageType)
    };

    // Each player's main pet will inherits the owner's index, but be flagged as a PET
    if (entity.type == PET && entity.index <= 3)
        entity.index = entity.index + 4 + 1;

    EntityDamageData entityData = EntityDamageData{
        (intptr_t)target,
        entity,
        damageData->rawDamage,
        damageData->elementalDamage,
    };

    DamageTrackManager::GetInstance()->UpdateDamage(entityData);

    return damageData;
}

bool DamageHooks::Init(uintptr_t* pointers)
{
    fnCalculateEntityDamagePtr = pointers[FUN_CALCULATE_ENTITY_DAMAGE];

    LOG("Added trampoline to function %016X", fnCalculateEntityDamagePtr);

    return HookFunctions();
}

bool HookFunctions()
{
    MH_STATUS status;

    status = MH_CreateHook(
        (fnHitTarget)fnHitTargetPtr,
        &HitTarget,
        reinterpret_cast<LPVOID*>(&ogHitTarget)
    );

    LOG("%s status: %s", NAMEOF(fnHitTarget), MH_StatusToString(status));

    
    status = MH_CreateHook(
        (fnCalculateEntityDamage)fnCalculateEntityDamagePtr,
        &CalculateEntityDamage,
        reinterpret_cast<LPVOID*>(&ogCalculateEntityDamage)
    );

    LOG("%s status: %s", NAMEOF(fnCalculateEntityDamage), MH_StatusToString(status));

    status = MH_CreateHook(
        (fnUnkButInteresting)fnUnkButInterestingPtr,
        &UnkButInteresting,
        reinterpret_cast<LPVOID*>(&ogUnkButInteresting)
    );

    LOG("%s status: %s", NAMEOF(fnUnkButInteresting), MH_StatusToString(status));


    /*
    status = MH_CreateHook(
        (fnDealDamageToEntity)fnDealDamageToEntityPtr,
        &DealDamageToEntity,
        reinterpret_cast<LPVOID*>(&ogDealDamageToEntity)
    );

    LOG("%s status: %s", NAMEOF(fnDealDamageToEntity), MH_StatusToString(status));*/

    return status == MH_OK;
}

EntityType GetEntityByDamageType(int damageType)
{
    switch (damageType)
    {
        case 0:
            return PLAYER;
        case 0x15:
        case 0x16:
        case 0x17:
            return PET;
        default:
            return UNKNOWN;
    }
}