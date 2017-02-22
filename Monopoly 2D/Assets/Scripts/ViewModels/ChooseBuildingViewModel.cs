using UnityEngine;
using UnityEngine.UI;
using System.Collections;

namespace AssemblyCSharp{
	public class ChooseBuildingViewModel : MonoBehaviour {

		NormalPlace normalPlace;
		Text title;
		Text costBuy;
		Text timeOut;
		Button yes;
		Button no;

        public bool Result;
        public bool isPending = true;

		public void Init(NormalPlace norPlace){
			normalPlace = norPlace;
			SetText ();
			SetButton ();
			StartCoroutine (SetTimeOut());
		}

		void SetText(){
			Text[] texts = gameObject.GetComponentsInChildren<Text> ();
			for (int i = 0; i < texts.Length; i++)
				if (texts [i].name == "CostBuy")
					costBuy = texts [i];
				else if (texts [i].name == "TimeOutForBuyPlace")
					timeOut = texts [i];
				else if (texts [i].name == "Title")
					title = texts [i];
			title.text = normalPlace.Name;
			costBuy.text = normalPlace.SellingPrice + " $";
		}

		void SetButton(){
			Button[] buttons = gameObject.GetComponentsInChildren<Button> ();
			for (int i = 0; i < buttons.Length; i++)
				if (buttons [i].name == "Yes")
					yes = buttons [i];
				else if (buttons [i].name == "No")
					no = buttons [i];

			yes.onClick.AddListener (() => NotifyResult (true));
			no.onClick.AddListener (() => NotifyResult (false));
		}

		void NotifyResult(bool result){
            Result = result;
            isPending = false;
		}
        public void ClosePopup()
        {
            Destroy(gameObject);
        }

		IEnumerator SetTimeOut(){
			int time = int.Parse (timeOut.text);
			while (time > 0) {
				yield return new WaitForSeconds (1);
				time = int.Parse (timeOut.text) - 1;
				if (time >= 0)
					timeOut.text = time.ToString ();
				else
					yield break;
			}
			NotifyResult (false);
		}
	}
}