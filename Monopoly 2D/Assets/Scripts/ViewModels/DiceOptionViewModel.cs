using UnityEngine;
using UnityEngine.UI;
using System.Collections;


namespace AssemblyCSharp{
	public class DiceOptionViewModel : MonoBehaviour {

		Text timeOut;
		Button[] listButtons;
        string[] listButtonName = { 
			"Odd",
			"Even",
			"Money"
		};
        public bool isPending = true;

        public OpportunityOptions Oppotunity { get; private set; }

		// Use this for initialization
		void Start () {
            Init();
        }
		
		// Update is called once per frame
		void Update () {
		
		}

		public void Init(){
			listButtons = new Button[listButtonName.Length];
			GetAllButtons ();
			SetEventAllButtonsChoose ();
			InitTimeOut ();
		}

		private void InitTimeOut(){
			Text[] texts = gameObject.GetComponentsInChildren<Text>() as Text[];
            for (int i = 0; i < texts.Length; i++)
                if (texts[i].name == "TimeOutForOpportunity")
                {
                    timeOut = texts[i];
                    break;
                }
			timeOut.text = "5";
			StartCoroutine (SetTimeOut());
		}

		private void GetAllButtons(){
			Button[] buttons = gameObject.GetComponentsInChildren<Button> ();
			for (int i = 0; i < buttons.Length; i++) {
				GetButton (buttons [i]);
			}
		}

		private void GetButton(Button button){
			for (int i = 0; i < listButtonName.Length; i++) {
				if (button.name == listButtonName[i]) {
					listButtons [i] = button;
					return;
				}
			}
		}

		private void SetEventAllButtonsChoose(){
			for (int i = 0; i < listButtons.Length; i++) {
				int pos = i;
				listButtons [i].onClick.AddListener (() => ChooseOpportunity (pos));
			}
		}

		private void ChooseOpportunity(int pos){
            switch (pos)
            {
                case -1:
                    Oppotunity = OpportunityOptions.DEFAULT;
                    break;
                case 0:
                    Oppotunity = OpportunityOptions.ODD;
                    break;
                case 1:
                    Oppotunity = OpportunityOptions.EVEN;
                    break;
                case 2:
                    Oppotunity = OpportunityOptions.MONEY;
                    break;
                default:
                    break;
            }
            isPending = false;
		}

        public void ClosePopup()
        {
            Destroy(gameObject);
        }
		IEnumerator SetTimeOut(){
            int time = int.Parse(timeOut.text);
            while (time > 0)
            {
                yield return new WaitForSeconds(1);
                time = int.Parse(timeOut.text) - 1;
                if (time >= 0)
                    timeOut.text = time.ToString();
                else
                    yield break;
            }
            ChooseOpportunity (-1);
		}
	}
}