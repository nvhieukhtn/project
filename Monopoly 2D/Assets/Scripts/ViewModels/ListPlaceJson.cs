using UnityEngine;

namespace AssemblyCSharp
{
    class ListPlaceJson
    {
        public PlaceJson[] ListPlaces;

        public static ListPlaceJson LoadFromJson()
        {
            var map = Resources.Load<TextAsset>("Data/map");
            return JsonUtility.FromJson<ListPlaceJson>(map.text);
        }
    }
}
