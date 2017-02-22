using UnityEngine;
using UnityEngine.UI;
using System.Collections;

namespace AssemblyCSharp{
	public class PlaceInfomationViewModel:MonoBehaviour{
		Canvas Popup;
		Button Close;
        Text Title;
		Text Content;
        public void GetUserComponent()
        {
            Text[] text = gameObject.GetComponentsInChildren<Text>() as Text[];
            int n = text.Length;
            for (int i = 0; i < n; i++)
            {
                if (text[i].name == "PlaceName")
                    Title = text[i];
                else if (text[i].name == "Detail")
                    Content = text[i];
            }
        }
        public void Start()
        {
            Popup = gameObject.GetComponentInChildren<Canvas>() as Canvas;
            Close = gameObject.GetComponentInChildren<Button>() as Button;
            
            Close.onClick.AddListener(delegate {
                HidePopup();
            });
        }
        public void SetPlaceName(string placeName)
        {
            GetUserComponent();
            Title.text = placeName;
        }

        public void SetContent(string content)
        {
            GetUserComponent();
            Content.text = content;
        }
        public void HidePopup(){
            var inputManager = InputManager.GetInstance();
			Popup.enabled = false;
            if(--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;
            DestroyImmediate(gameObject);
        }
    }
}