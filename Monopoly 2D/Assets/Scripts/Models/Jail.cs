using System;
using UnityEngine;

namespace AssemblyCSharp
{
    public class Jail : SpecialPlace
    {
        public override string GetPlaceType()
        {
            return "Jail";
        }

        public override string GetSource()
        {
            return "Place/Jail";
        }
    }
}

