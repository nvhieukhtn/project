using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class ButtonRotateViewModel : MonoBehaviour {

	Button buttonRotate;
	Text timeOut;
    public bool IsPending = true;

	public void Init(){
		buttonRotate = gameObject.GetComponentInChildren<Button> ();
		buttonRotate.onClick.AddListener (() => Finish ());

		Text[] texts = gameObject.GetComponentsInChildren<Text> ();
		for (int i = 0; i < texts.Length; i++)
			if (texts [i].name == "TimeOut")
				timeOut = texts [i];
		timeOut.color = Color.black;
		StartCoroutine (SetTimeOut ());
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
			if (time <= 3)
				timeOut.color = Color.red;
		}
		Finish ();
	}

	void Finish(){
        IsPending = false;
	}

    public void ClosePopup()
    {
        Destroy(gameObject);
    }
}
