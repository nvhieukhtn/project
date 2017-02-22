using System;

namespace AssemblyCSharp
{
	public class UpgradableBuilding:BaseBuilding
	{
		public int Level{get;set;}
        public int UpgradePrice = 200;        

        public UpgradableBuilding()
        {
            Level = 1;
        }

        public override string GetResource()
        {
            if (Level >= 1 && Level <= 4)
                return "House" + Style + "/House" + Level;
            return string.Empty;
        }
        public void Upgrade()
        {
            Level++;
        }

        public bool CanUpgrade()
        {
            return Level < 4;
        }
    }
}

