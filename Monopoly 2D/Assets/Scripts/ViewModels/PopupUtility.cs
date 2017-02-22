using UnityEngine;
using UnityEngine.UI;
using System.Collections;

namespace AssemblyCSharp{
	public class PopupUtility : MonoBehaviour {

		Text title;
		Text detail;
		Text timeOut;
		Button close;
        public bool isPending = true;
		void Start(){
			//Init ("CHANCE", "Detail here");
		}

		public void Init(string _title, string _detail){
			setButton ();
			setText ();
			title.text = _title;
			detail.text = _detail;
			setTimeOut ();
		}

		void setButton (){
			close = gameObject.GetComponentInChildren<Button> ();
			close.onClick.AddListener (() => Finish ());
		}
		void setText () {
			Text[] texts = gameObject.GetComponentsInChildren<Text> ();

			for (int i = 0; i < texts.Length; i++)
				if (texts [i].name == "Title")
					title = texts [i];
				else if (texts [i].name == "Detail")
					detail = texts [i];
				else if (texts [i].name == "TimeOut")
					timeOut = texts [i];
		}
		void setTimeOut () {
			timeOut.text = "10";
			StartCoroutine (SetTimeOut());
		}

        void Finish()
        {
            isPending = false;
        }
		public void ClosePopup(){
			Destroy (gameObject);
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
			Finish ();
		}
	}
}