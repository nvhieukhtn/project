using System;
using UnityEngine;

namespace AssemblyCSharp
{
    public class Park : SpecialPlace
    {
        public override string GetPlaceType()
        {
            return "Park";
        }

        public override string GetSource()
        {
            return "Place/Park";
        }
    }
}

