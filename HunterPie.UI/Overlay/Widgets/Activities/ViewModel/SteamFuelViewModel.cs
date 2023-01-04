﻿using HunterPie.Core.Architecture;

namespace HunterPie.UI.Overlay.Widgets.Activities.ViewModel;

public class SteamFuelViewModel : Bindable
{
    private int _naturalFuel;
    public int NaturalFuel { get => _naturalFuel; set => SetValue(ref _naturalFuel, value); }

    private int _storedFuel;
    public int StoredFuel { get => _storedFuel; set => SetValue(ref _storedFuel, value); }
}