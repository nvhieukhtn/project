using System;
using UnityEngine;


namespace AssemblyCSharp
{
    public class Shared
    {
        public static Vector2 ConvertPosition(int position)
        {
            Vector2 vector2 = new Vector2();
            if (position >= 1 && position <= 7)
            {
                vector2.x = (7 - position) * 100 + 50;
                vector2.y = 50;
            }
            else if (position <= 13)
            {
                vector2.x = 50;
                vector2.y = (position - 7) * 100 + 50;
            }
            else if (position <= 19)
            {
                vector2.x = (position - 13) * 100 + 50;
                vector2.y = 650;
            }
            else
            {
                vector2.x = 650;
                vector2.y = (25 - position) * 100 + 50;
            }
            return vector2;
        }
    }
}

