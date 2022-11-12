﻿using System;
using System.Globalization;
using System.Windows.Data;
using Converter = System.Convert;

namespace HunterPie.UI.Architecture.Converters;
public class PercentageToRelativeSizeConverter : IMultiValueConverter
{
    public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
    {
        if (values.Length < 3)
            throw new ArgumentException("there must be 3 double values");

        double current = Converter.ToDouble(values[0]); ;
        double max = Math.Max(1, Converter.ToDouble(values[1]));
        double relativeSize = Converter.ToDouble(values[2]);

        return current / max * relativeSize;
    }
    public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture) => throw new NotImplementedException();
}
