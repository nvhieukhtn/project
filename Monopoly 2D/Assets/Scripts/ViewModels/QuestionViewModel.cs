using UnityEngine;
using UnityEngine.UI;
using System.Collections;

namespace AssemblyCSharp{
	public class QuestionViewModel : MonoBehaviour {
		Text timeOut;
		Text questionContentUI;
		Text[] options = new Text[4];
		Button[] btnOptions = new Button[4];
		Question questionContent;

        private bool result;
        public bool isPending = true;
        public bool Result
        {
            get
            {
                return result;
            }

            set
            {
                result = value;
            }
        }

        // Use this for initialization
        void Start () {
            
		}
		
		// Update is called once per frame
		void Update () {

		}


		public void Init(){
			Text[] texts = gameObject.GetComponentsInChildren<Text>() as Text[];
			for (int i = 0; i < texts.Length; i++) {
				switch (texts [i].name) {
				case "QuestionContent":
					questionContentUI = texts [i];
					break;
				case "TimeOutForQuestion":
					timeOut = texts [i];
					break;
				case "AnswerA":
					options[0] = texts [i];
					break;
				case "AnswerB":
					options[1] = texts [i];
					break;
				case "AnswerC":
					options[2] = texts [i];
					break;
				case "AnswerD":
					options[3] = texts [i];
					break;
				}
			}

			Button[] buttons = gameObject.GetComponentsInChildren<Button>() as Button[];
			for (int i = 0; i < buttons.Length; i++) {
				switch (buttons [i].name) {
				case "AnswerA":
					btnOptions[0] = SetListenEvent(buttons [i], 0);
					break;
				case "AnswerB":
					btnOptions[1] = SetListenEvent(buttons [i], 1);
					break;
				case "AnswerC":
					btnOptions[2] = SetListenEvent(buttons [i], 2);
					break;
				case "AnswerD":
					btnOptions[3] = SetListenEvent(buttons [i], 3);
					break;
				}
			}
			timeOut.text = "15";
        }

		Button SetListenEvent(Button btn, int index){
			btn.onClick.AddListener(() => {
				Answer(index);
			});
			return btn;
		}

		public void LoadQuestionContent(Question question){
			questionContent = question;
			if (questionContent.Options.Length != 4) {
				return;
			}
			questionContentUI.text = questionContent.Describe;
			for (int i = 0; i < 4; i++)
				options [i].text = questionContent.Options [i];
            StartCoroutine(SetTimeOut());
        }

		public void Answer(int pos){
            var gameManager = GameManager.Instance;
            Result = pos == questionContent.Answer;
            ShowResult(pos);
            isPending = false;
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
			Answer (-1);
		}
        void ShowResult(int choose)
        {
            DisableButton();
            SetColor(btnOptions[questionContent.Answer], Color.green);
            if(choose >= 0 && choose != questionContent.Answer)
                SetColor(btnOptions[choose], Color.red);
        }

        void SetColor(Button btn, Color color)
        {
            var buttonColor = btn.colors;
            buttonColor.disabledColor = color;
            btn.colors = buttonColor;
        }

        void DisableButton()
        {
            for (int i = 0; i < 4; i++)
                btnOptions[i].interactable = false;
        }
        public void ClosePopup()
        {
            Destroy(gameObject);
        }
    }
}