using UnityEngine.UI;
using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

namespace AssemblyCSharp
{
    public class PopupGameOver : MonoBehaviour {

        Text playerWin;
        Button menu;
        Button replay;

        public bool isPending = true;

        // Use this for initialization
        void Start() {

        }

        public void Init() {
            GetText();
            GetButton();
        }

        void GetText() {
            Text[] texts = gameObject.GetComponentsInChildren<Text>();
            for (int i = 0; i < texts.Length; i++)
                if (texts[i].name == "Player")
                    playerWin = texts[i];
        }

        void GetButton() {
            Button[] btns = gameObject.GetComponentsInChildren<Button>();
            for (int i = 0; i < btns.Length; i++)
                if (btns[i].name == "Menu")
                    menu = btns[i];
                else if (btns[i].name == "Replay")
                    replay = btns[i];

            menu.onClick.AddListener(() => MenuClick());
            replay.onClick.AddListener(() => ReplayClick());
        }

        public void SetWinnerName(string name)
        {
            playerWin.text = name + " win";
        }

        void MenuClick() {
            var inputManager = InputManager.GetInstance();
            inputManager.Reset();
            SceneManager.LoadScene("StartGame");
        }
        public void ClosePopup()
        {
            Destroy(gameObject);
        }

	    void ReplayClick (){
            var inputManager = InputManager.GetInstance();
            inputManager.Reset();
            SceneManager.LoadScene("MainGame");
        }
	
	    // Update is called once per frame
	    void Update () {
	
	    }
    }

}