﻿using HunterPie.Features.Statistics.Models;
using Newtonsoft.Json;
using System;

namespace HunterPie.Integrations.Poogie.Statistics.Models;

internal record PoogiePlayerDamageStatisticsModel(
    [JsonProperty("damage")] float Damage,
    [JsonProperty("dealt_at")] DateTime DealtAt
)
{
    public PlayerDamageFrameModel ToEntity() => new PlayerDamageFrameModel(
        Damage: Damage,
        DealtAt: DealtAt
    );

    public static PoogiePlayerDamageStatisticsModel From(PlayerDamageFrameModel model) =>
        new PoogiePlayerDamageStatisticsModel(
            Damage: model.Damage,
            DealtAt: model.DealtAt
        );
}
