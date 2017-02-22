using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AssemblyCSharp
{
    public class Map
    {
        public List<BasePlace> ListPlaces { get; set; }
        public Map()
        {
            ListPlaces = new List<BasePlace>();
        }
    }
}
