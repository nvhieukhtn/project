using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;


namespace AssemblyCSharp
{
    public class Station:Service
    {
        public override string GetSource()
        {
            return "Place/Station";
        }
    }
}
