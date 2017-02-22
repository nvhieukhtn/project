using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AssemblyCSharp
{
    public abstract class BaseAction
    {
        public string Describe { get; set; }
        public abstract void Action(ref Player owner, ref Player enemy, BasePlace[] listPlaces);
    }
}
