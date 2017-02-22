using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AssemblyCSharp
{
    class IncreaseMoney : BaseAction
    {
        private int money = 200;

        public IncreaseMoney()
        {
            Describe = "Bạn được tăng 200$";
        }
        public IncreaseMoney(string descripe, int money)
        {
            Describe = descripe;
            this.money = money;
        }

        public override void Action(ref Player currentPlayer,ref  Player enemy, BasePlace[] listPlaces)
        {
            currentPlayer.Money += money;
        }
    }
}
