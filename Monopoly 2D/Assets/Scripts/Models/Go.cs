using System;
using UnityEngine;

namespace AssemblyCSharp
{
    public class Go : SpecialPlace
    {
        public override string GetPlaceType()
        {
            return "Go";
        }

        public override string GetSource()
        {
            return "Place/Go";
        }
    }
}

