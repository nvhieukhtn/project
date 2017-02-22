using UnityEngine;
using System.Collections;

namespace AssemblyCSharp
{
    public class Player : MonoBehaviour {
        public float MovementSpeed = 200f;
        public string PlayerName { get; set; }
        public int Money { get; set; }
        public int NumberOfPlace { get; set; }
        public int CurrentPosition { get; set; }
        public int DestinationPosition { get; set; }
        
        public int BanSteps { get; set; }

        public float steps;
        public float stepsWalked;
        public bool isUpdate = false;
        private Vector3 destination;
        Animator anim;
        public void Start()
        {
            transform.position = new Vector3(650f, 50f, 0);
            anim = GetComponent<Animator>();
            CurrentPosition = 1;
        }

        public void ReceiveSalary(int money) {
            Money += money;
        }

        public void PayTax(int taxPrice) {
            Money -= taxPrice;
        }

        public void GoToJail() {
            BanSteps = 3;
        }

        public void BuyPlace(NormalPlace place)
        {
            if (Money >= place.SellingPrice)
            {
                NumberOfPlace++;
                Money -= place.SellingPrice;
                place.ChangeOwner(this);
            }
        }

        public void UpgradePlace(NormalPlace place)
        {
            if (Money >= place.UpgradePrice)
            {
                Money -= place.UpgradePrice;
            }
        }

		public void Forwalk(float stepsWalk){
            if (CurrentPosition + (int)stepsWalk / 100 > 24)
                Money += 500;
            DestinationPosition = (CurrentPosition + (int)stepsWalk / 100 - 1) % 24 + 1;
            this.steps = stepsWalk;
            Vector2 destPosition = Shared.ConvertPosition(this.DestinationPosition);
			destination = new Vector3 (destPosition.x, destPosition.y, 0);
			anim.SetInteger ("Speed", (int)MovementSpeed);
			isUpdate = true;
            stepsWalked = 0f;
		}
        public void Reset()
        {
            Money = 2000;
            NumberOfPlace = 0;
            CurrentPosition = 1;
            BanSteps = 0;
            Vector2 post = Shared.ConvertPosition(this.CurrentPosition);
            transform.position = post;
        }
        public void Update(){
			if (isUpdate) {
                Vector2 direct;
                Vector3 currentPosition = transform.position;

                if (CurrentPosition >= 1 && CurrentPosition < 7)
                {
                    currentPosition.y = 50f;
                    direct = new Vector2(-1f, 0);
                    anim.SetBool("IsLeft", true);
                }
                else if (CurrentPosition < 13)
                {
                    currentPosition.x = 50f;
                    direct = new Vector2(0, 1f);
                    anim.SetBool("IsLeft", false);
                }
                else if (CurrentPosition < 19)
                {
                    currentPosition.y = 650f;
                    direct = new Vector2(1f, 0);
                    anim.SetBool("IsLeft", false);
                }
                else
                {
                    currentPosition.x = 650f;
                    direct = new Vector2(0, -1f);
                    anim.SetBool("IsLeft", true);
                }
                
                currentPosition.x += MovementSpeed * Time.deltaTime * direct.x;
                currentPosition.y += MovementSpeed * Time.deltaTime * direct.y;

                transform.position = currentPosition;
				steps -= MovementSpeed * Time.deltaTime;
                stepsWalked += MovementSpeed * Time.deltaTime; 
                if(stepsWalked >= 100)
                {
                    CurrentPosition = CurrentPosition % 24 + 1;
                    stepsWalked = 0f;
                }
                if (steps <= 0) {
					transform.position = destination;
					isUpdate = false;
					anim.SetInteger ("Speed", 0);
                    CurrentPosition = DestinationPosition;
				}
			}
		}
	}
}
