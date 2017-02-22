using System;
using UnityEngine;

namespace AssemblyCSharp
{
	public abstract class BaseBuilding
	{
		public string BuildingName{ get; set; }
		public Color ColorBuilding{ get; set; }
        public string Describe { get; private set; }
		public int BuildingPrice{ get; set; }
        public static int Style = 0;
        public abstract string GetResource();
        public Material LoadMaterial()
        {
            var resource = GetResource();
            Material material = null;
            if (!string.IsNullOrEmpty(resource))
                material = (Material)Resources.Load(resource, typeof(Material));
            return material;
        }
    }
}

