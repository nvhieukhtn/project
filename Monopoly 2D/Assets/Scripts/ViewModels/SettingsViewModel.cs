using UnityEngine;
using UnityEngine.UI;
using System.Collections;
namespace AssemblyCSharp
{
    public class SettingsViewModel : MonoBehaviour
    {
        Button[] buttonsUI = new Button[2];
        // Use this for initialization
        void Start()
        {
            
            var buttons = gameObject.GetComponentsInChildren<Button>();
            for (int i = 0; i < buttons.Length; i++)
                if (buttons[i].name == "Button1")
                    buttonsUI[0] = buttons[i];
            else if (buttons[i].name == "Button2")
                    buttonsUI[1] = buttons[i];
            buttonsUI[0].GetComponent<Image>().color = Color.red;
            buttonsUI[1].GetComponent<Image>().color = Color.white;
            buttonsUI[0].onClick.AddListener(() => Button1Click());
            buttonsUI[1].onClick.AddListener(() => Button2Click());
        }
        public void Button1Click()
        {
            buttonsUI[0].GetComponent<Image>().color = Color.red;
            buttonsUI[1].GetComponent<Image>().color = Color.white;
        }

        public void Button2Click()
        {
            buttonsUI[1].GetComponent<Image>().color = Color.red;
            buttonsUI[0].GetComponent<Image>().color = Color.white;
        }
        // Update is called once per frame
        void Update()
        {

        }
    }
}
