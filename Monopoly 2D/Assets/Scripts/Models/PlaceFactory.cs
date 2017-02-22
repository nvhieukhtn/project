using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;


namespace AssemblyCSharp
{
    public class PlaceFactory
    {
        private static BasePlace GetPlaceInstance(PlaceJson placeJson)
        {
            BasePlace place = null;
            string name = placeJson.Name;
            switch (placeJson.Type)
            {
                case "Normal":
                    if (name == "Station")
                        place = new Station();
                    else
                        place = new EState();
                    ((NormalPlace)place).SellingPrice = placeJson.Price;
                    ((NormalPlace)place).TaxPrice = placeJson.Price; 
                    break;
                case "Utility":
                    if (name == "Chance")
                        place = new Chance();
                    else
                        place = new CommunityChest();
                    break;
                case "Special":
                    if (name == "Jail")
                        place = new Jail();
                    else if (name == "Park")
                        place = new Park();
                    else if (name == "Station")
                        place = new PublicStation();
                    else
                        place = new Go();
                    break;
                default:
                    break;
            }
            place.Name = name;
            return place;
        }
        private static Color GetPlaceColor(string color)
        {
            switch (color)
            {
                case "Red":
                    return Color.red;
                case "Yellow":
                    return  Color.yellow;
                case "Green":
                    return Color.green;
                case "Blue":
                    return Color.blue;
                case "Magenta":
                    return Color.magenta;
                case "Cyan":
                    return Color.cyan;
                default:
                    return Color.white;
            }
        }
        public static BasePlace GetPlace(PlaceJson placeJson)
        {
            var place = GetPlaceInstance(placeJson);
            Color placeColor = GetPlaceColor(placeJson.Color);
            place.PlaceColor = placeColor;
            place.Describe = placeJson.Describe;
            return place;
        }
        public static PlaceType GetPlaceType(PlaceJson placeJson)
        {
            switch (placeJson.Type)
            {
                case "Normal":
                    return PlaceType.Normal;
                case "Utility":
                    return PlaceType.Utility;
                case "Special":
                    return PlaceType.Special;
                default:
                    return PlaceType.Normal;
            }
        }
    }
}
