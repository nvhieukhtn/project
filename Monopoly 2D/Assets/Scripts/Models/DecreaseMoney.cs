using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AssemblyCSharp
{
    class DecreaseMoney : BaseAction
    {
        private int money = 200;
        public DecreaseMoney()
        {
            Describe = "Bạn bị giảm 200$";
        }
   
        public DecreaseMoney(string descripe, int money)
        {
            Describe = descripe;
            this.money = money;
        }

        public override void Action( ref Player currentPlayer,ref  Player enemy, BasePlace[] listPlaces)
        {
            currentPlayer.Money -= money;
        }
    }
}
