using System;
using System.Collections.Generic;

namespace AssemblyCSharp
{
	public class EState:NormalPlace
	{
		public BaseBuilding ListBuilding{ get; set; }

        
        public override string GetSource()
        {
            return "Place/Empty";
        }
    }
}

