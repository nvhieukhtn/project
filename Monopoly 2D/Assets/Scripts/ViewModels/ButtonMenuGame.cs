using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.SceneManagement;
namespace AssemblyCSharp
{ 
    public class ButtonMenuGame: MonoBehaviour {

	    public Button playGame;
	    public Button info;
	    public Canvas popupInfo;
        public Canvas popupSettings;

	    // Use this for initialization
	    void Start () {
	
	    }
	
	    // Update is called once per frame
	    void Update () {
	
	    }

	    public void PlayGame(){
            var inputManager = InputManager.GetInstance();
            inputManager.Reset();
            SceneManager.LoadScene("MainGame");
        }

	    public void CloseInfo(){
		    popupInfo.enabled = false;
	    }

        public void SetSettings1()
        {
            BaseBuilding.Style = 0;
        }

        public void SetSettings2()
        {
            BaseBuilding.Style = 1;
        }

        public void ToggleBackground()
        {
            var audioManager = AudioManager.GetInstance();
            audioManager.ToggleBackground();
        }

        public void ToggleEffectSound()
        {
            var audioManager = AudioManager.GetInstance();
            audioManager.ToggleEffectSound();
        }
        public void CloseSettings()
        {
            popupSettings.enabled = false;
        }

	    public void ShowPopupInfo(){
		    popupInfo.enabled = true;
	    }
        public void ShowPopupSettings()
        {
            popupSettings.enabled = true;
        }
    }

}