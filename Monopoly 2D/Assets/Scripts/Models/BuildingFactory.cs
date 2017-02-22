using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AssemblyCSharp
{
    public class BuildingFactory
    {
        public static BaseBuilding GetBuilding(BuildingType type)
        {
            switch (type)
            {
                case BuildingType.UPGRADE:
                    return new UpgradableBuilding();
                default:
                    return new UnUpgradableBuilding();
            }
        }
    }
}
