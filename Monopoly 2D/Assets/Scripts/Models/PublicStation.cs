using System;
using UnityEngine;

namespace AssemblyCSharp
{
    public class PublicStation : SpecialPlace
    {
        public override string GetPlaceType()
        {
            return "Station";
        }

        public override string GetSource()
        {
            return "Place/Station";
        }
    }
}

