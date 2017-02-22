using System;
using UnityEngine;
using System.Collections;

namespace AssemblyCSharp
{
    public abstract class NormalPlace : BasePlace
    {
        public bool IsMortgage { get; set; }
        public int MortgagePrice { get; set; }
        public Player Owner { get; set; }
        public int SellingPrice { get; set; }
        public int TaxPrice { get; set; }

        public int UpgradePrice = 200;

        public override void Reset()
        {
            Owner = null;
            TaxPrice = SellingPrice;
        }

        public bool HasOwner()
        {
            return Owner != null;
        }

        public void ChangeOwner(Player owner)
        {
            Owner = owner;
        }

        public void NotifyOwnerReceiveMoney(int money)
        {
            Owner.ReceiveSalary(money);
        }
    }
}

