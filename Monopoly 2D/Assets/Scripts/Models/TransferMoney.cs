using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AssemblyCSharp
{
    class TransferMoney : BaseAction
    {
        private int money = 200;
        public TransferMoney()
        {
            Describe = "Đối phương chuyển cho bạn 200$";
        }
   
        public TransferMoney(string descripe, int money)
        {
            Describe = descripe;
            this.money = money;
        }

        public override void Action(ref Player currentPlayer,ref Player enemy, BasePlace[] listPlaces)
        {
            currentPlayer.Money += money;
            enemy.Money -= money;
        }
    }
}
