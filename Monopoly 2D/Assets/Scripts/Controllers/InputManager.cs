using System;
using System.Collections;
using UnityEngine;
using System.IO;

namespace AssemblyCSharp
{
    public class InputManager
    {
        private int _numberOfPopupExist;
        public int NumberOfPopupExist
        {
            get
            {
                return _numberOfPopupExist;
            }

            set
            {
                if (value >= 0)
                    _numberOfPopupExist = value;
                else _numberOfPopupExist = 0;
            }
        }
        private static InputManager _instance;
        public static InputManager GetInstance()
        {
            if (_instance == null)
                _instance = new InputManager();
            return _instance;
        }
        private InputManager()
        {
            IsAllowed = true;
        }

        public bool IsAllowed { get; set; }

        public void Reset()
        {
            IsAllowed = true;
            NumberOfPopupExist = 0;
        }
    }
}
