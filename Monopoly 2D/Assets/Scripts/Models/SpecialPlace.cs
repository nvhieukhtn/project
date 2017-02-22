using System;
using UnityEngine;

namespace AssemblyCSharp
{
	public abstract class SpecialPlace:BasePlace
	{
		public UnUpgradableBuilding Building{get;set;}
        public override void Reset()
        {
            Building = null;
        }
        public abstract string GetPlaceType();
    }
}

