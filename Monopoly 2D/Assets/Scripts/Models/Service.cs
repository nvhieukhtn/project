using System;
using UnityEngine;

namespace AssemblyCSharp
{
	public abstract class Service: NormalPlace
	{
        public UnUpgradableBuilding Building{ get; set; }
        public override void Reset()
        {
            Building = null;
        }
    }
}

