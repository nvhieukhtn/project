using UnityEngine;
using System.Collections;
namespace AssemblyCSharp
{
	public class Dice : MonoBehaviour {

		// Use this for initialization
		public DiceOption Option = DiceOption.NORMAL;
		public int result;
		public Animator anim;

		void Start () {
			anim = GetComponent<Animator> ();
            anim.SetInteger("NumOfDice", 6);
        }
		public void SetOption(DiceOption option){
			Option = option;
		}

        private void GenerateResult()		{
			switch (Option) {
			case DiceOption.EVEN:
				result = (int)Random.Range (1f, 3f) * 2;
				break;
			case DiceOption.ODD:
				result = (int)Random.Range (1f, 3f) * 2 - 1;
				break;
			default:
				result = (int)Random.Range (1f, 6f);
				break;
			}
		}
        public void StartDice()
        {
            anim.SetInteger("NumOfDice", 0);
        }

		public void StopDice(){
			GenerateResult ();
			anim.SetInteger ("NumOfDice", result);
        }
	}
}