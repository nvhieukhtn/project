using System;
using UnityEngine;

namespace AssemblyCSharp
{
    [Serializable]
	public class Question
    {
        public string Describe;
        public string[] Options;
        public int Answer;
    }
}

