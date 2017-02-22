using System;
using UnityEngine;
using UnityEngine.UI;

namespace AssemblyCSharp
{
	public abstract class BasePlace
	{
		public Color PlaceColor{ get; set; }
		public string Name{ get; set; }
        public string Describe { get; set; }

        public abstract string GetSource();
        public abstract void Reset();
        public Material LoadMaterial()
        {
            var source = GetSource();
            var material = (Material)Resources.Load(source, typeof(Material));
            var cloneMaterial = new Material(material);
            cloneMaterial.shader = Shader.Find("Mobile/Particles/VertexLit Blended");
            cloneMaterial.SetColor("_EmisColor", PlaceColor);
            return cloneMaterial;
        }
    }
}

