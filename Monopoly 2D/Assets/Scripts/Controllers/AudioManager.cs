using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AssemblyCSharp
{
    public class AudioManager
    {
        public bool IsBackground = true;
        public bool IsEffectSound = true;

        private static AudioManager _instance;
        public static AudioManager GetInstance()
        {
            if (_instance == null)
                _instance = new AudioManager();
            return _instance;
        }
        private AudioManager()
        {
            IsBackground = true;
            IsEffectSound = true;
        }

        public void ToggleBackground()
        {
            IsBackground = !IsBackground;
        }

        public void ToggleEffectSound()
        {
            IsEffectSound = !IsEffectSound;
        }

    }
}
