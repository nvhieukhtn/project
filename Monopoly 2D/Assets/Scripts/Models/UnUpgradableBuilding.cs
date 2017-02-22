using System;

namespace AssemblyCSharp
{
    public class UnUpgradableBuilding : BaseBuilding
    {
        public ServiceType Type;
        public override string GetResource()
        {
            switch (Type)
            {
                case ServiceType.BUS:
                    return "Service/Station";
                default:
                    return string.Empty;
            }
        }
    }
}

