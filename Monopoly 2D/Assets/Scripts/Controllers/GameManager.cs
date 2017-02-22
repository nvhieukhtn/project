using System.Collections;
using UnityEngine;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace AssemblyCSharp
{
	public class GameManager:MonoBehaviour
	{
        public ListQuestion listQuestion;
		public static GameManager _instance = null;
		public static GameManager Instance{
			get{
				if (_instance == null)
					_instance = new GameManager ();
				return _instance;
			}
		}
        private GameManager()
        {

        }
        public Dice[] dice = new Dice[2];
        private int currentTurn;
        public Player[] ListPlayer = new Player[2];
        public PlayerInfomationViewModel[] ListPlayerInfo = new PlayerInfomationViewModel[2];
        private bool isRotate = false, isStop = false;
        public PlaceViewModel place;
        private PlaceViewModel[] listPlaces = new PlaceViewModel[24];
        //Popup Question
        public QuestionViewModel originQuestionPopup;
        //Popup choose options
        public DiceOptionViewModel originDiceOptionPopup;

        //Popup Confirm
        public ChooseBuildingViewModel originConfirmBuyPopup;
        public ChooseBuildingViewModel originConfirmUpgradePopup;
        //Popup show Utility
        public PopupUtility popupUtility;
        //Button Rotate
        public ButtonRotateViewModel buttonRotateViewModel;

        //Popup show Notify
        public PopupUtility popupNotify;
        //Popup show GameOver
        public PopupGameOver popupGameOver;
        public void Start()
        {
            var audioManager = AudioManager.GetInstance();
            if(audioManager.IsBackground)
            {
                var resource = Resources.Load("Audio/background", typeof(AudioClip)) as AudioClip;
                var audioResource = GetComponents<AudioSource>();
                audioResource[1].clip = resource;
                audioResource[1].Play();
            }
            _instance = this;
            StartCoroutine("LoadMap");
        }
        public void ResetGame()
        {
            for (var i = 0; i < 2; i++)
            {
                ListPlayer[i].Reset();
                ListPlayerInfo[i].UpdatePlayerInfo();
            }
            for (int i = 0;i< 24;i++)
            {
                listPlaces[i].Reset();
            }

            StartCoroutine(ShowButtonRotate());
        }
        private void Init()
        {
            dice[0] = (Dice)Instantiate(dice[0], new Vector3(320f, 360f, 0), Quaternion.identity);
            dice[1] = (Dice)Instantiate(dice[1], new Vector3(400f, 360f, 0), Quaternion.identity);

            for (var i = 0; i < 2; i++)
            {
                ListPlayer[i] = (Player)Instantiate(ListPlayer[i], new Vector3(360f, 360f, 0), Quaternion.identity);
                ListPlayer[i].PlayerName = "Player " + (i + 1);
                ListPlayer[i].Money = 2000;
            }

            ListPlayerInfo[0] = (PlayerInfomationViewModel)Instantiate(ListPlayerInfo[0], new Vector3(226f, 531f, 0f), Quaternion.identity);
            ListPlayerInfo[1] = (PlayerInfomationViewModel)Instantiate(ListPlayerInfo[1], new Vector3(475f, 531f, 0f), Quaternion.identity);
            for (int i = 0; i < 2; i++)
            {
                ListPlayerInfo[i].player = ListPlayer[i];
                ListPlayerInfo[i].Init();
                ListPlayerInfo[i].UpdateSymbol(i);
                ListPlayerInfo[i].UpdatePlayerInfo();
                ListPlayerInfo[i].UpdateColor(PlayerState.WAITING);
            }
            listQuestion = ListQuestion.GetQuestions();
            StartCoroutine(ShowButtonRotate());
        }

        public void Update()
        {
            if(isRotate)
                StartCoroutine(WaitForDice());
            if (isRotate && isStop)
            {
                for (var i = 0; i < 2; i++)
                    dice[i].StopDice();
                ListPlayer[currentTurn].Forwalk((float)(dice[0].result + dice[1].result) * 100f);
                isRotate = false;
                StartCoroutine(ProcessAfterWalk());
            }
        }

        IEnumerator ProcessAfterWalk()
        {
            yield return new WaitWhile(() => ListPlayer[currentTurn].isUpdate);
            ListPlayerInfo[currentTurn].UpdateColor(PlayerState.WAITING);
            if (listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].CanBuilding())
            {
                NormalPlace normalPlace = (NormalPlace)listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].PlaceInstance;
                ShowConfirm(normalPlace);
            }
            else if (listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].Type == PlaceType.Utility)
                StartCoroutine(ShowUtilityPopup());
            else if (listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].IsJail())
                StartCoroutine(GoToJail());
            else
            {
                ListPlayerInfo[currentTurn].UpdatePlayerInfo();
                currentTurn = (currentTurn + 1) % 2;
                StartCoroutine(ShowButtonRotate());
            }
        }

        

        IEnumerator ShowUtilityPopup()
        {
            var audioManager = AudioManager.GetInstance();
            if (audioManager.IsEffectSound)
            {
                var resource = Resources.Load("Audio/upgrade", typeof(AudioClip)) as AudioClip;
                var audioResource = GetComponent<AudioSource>();
                audioResource.clip = resource;
                audioResource.Play();
            }
            int tmp = (int)Random.Range(1f, 3.9f);
            BaseAction action = null; 
            switch (tmp)
            {
                case 1:
                    action = new IncreaseMoney();
                    break;
                case 2:
                    action = new DecreaseMoney();
                    break;
                case 3:
                    action = new TransferMoney();
                    break;
                default:
                    break;
            }
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = false;
            inputManager.NumberOfPopupExist++;
            var clonePopupUtility = Instantiate(popupUtility);
            clonePopupUtility.Init("Nội dung", action.Describe);
            yield return new WaitWhile(() => clonePopupUtility.isPending);
            
            var listPlaceModels = listPlaces.Select(x => x.PlaceInstance).ToArray();
            action.Action(ref ListPlayer[currentTurn],ref ListPlayer[1 - currentTurn], listPlaceModels);
            clonePopupUtility.ClosePopup();
            if(--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;
            ListPlayerInfo[0].UpdatePlayerInfo();
            ListPlayerInfo[1].UpdatePlayerInfo();
            currentTurn = (currentTurn + 1) % 2;
            StartCoroutine(ShowButtonRotate());
        }

        IEnumerator GoToJail()
        {
            var audioManager = AudioManager.GetInstance();
            if (audioManager.IsEffectSound)
            {
                var resource = Resources.Load("Audio/loss", typeof(AudioClip)) as AudioClip;
                var audioResource = GetComponent<AudioSource>();
                audioResource.clip = resource;
                audioResource.Play();
            }
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = false;
            inputManager.NumberOfPopupExist++;
            var clonePopupNotify = Instantiate(popupNotify);
            clonePopupNotify.Init("Nội dung", "Bạn đã đi vào tù. Bị cấm đi 3 lượt.");
            yield return new WaitWhile(() => clonePopupNotify.isPending);
            
            clonePopupNotify.ClosePopup();
            if(--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;
            ListPlayer[currentTurn].GoToJail();
            
            currentTurn = (currentTurn + 1) % 2;
            StartCoroutine(ShowButtonRotate());
        }

        IEnumerator ShowNotifyPopup()
        {
            var audioManager = AudioManager.GetInstance();
            if (audioManager.IsEffectSound)
            {
                var resource = Resources.Load("Audio/loss", typeof(AudioClip)) as AudioClip;
                var audioResource = GetComponent<AudioSource>();
                audioResource.clip = resource;
                audioResource.Play();
            }
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = false;
            inputManager.NumberOfPopupExist++;
            var clonePopupNotify = Instantiate(popupNotify);
            var money = ((NormalPlace)listPlaces[ListPlayer[currentTurn].CurrentPosition - 1]
                                .PlaceInstance).TaxPrice;
            clonePopupNotify.Init("Nội dung", "Bạn đã đi vào ô đất của đối phương. Tiền thuế cần đóng là " + money + " $");
            yield return new WaitWhile(() => clonePopupNotify.isPending);
            clonePopupNotify.ClosePopup();
            if(--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;


            listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].Notify();
            ListPlayer[currentTurn].PayTax(money);
            ListPlayerInfo[0].UpdatePlayerInfo();
            ListPlayerInfo[1].UpdatePlayerInfo();
            currentTurn = (currentTurn + 1) % 2;
            StartCoroutine(ShowButtonRotate());
        }

        IEnumerator ShowSkipStep()
        {
            var audioManager = AudioManager.GetInstance();
            if (audioManager.IsEffectSound)
            {
                var resource = Resources.Load("Audio/upgrade", typeof(AudioClip)) as AudioClip;
                var audioResource = GetComponent<AudioSource>();
                audioResource.clip = resource;
                audioResource.Play();
            }
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = false;
            inputManager.NumberOfPopupExist++;
            var clonePopupNotify = Instantiate(popupNotify);
            var banSteps = ListPlayer[currentTurn].BanSteps;
            clonePopupNotify.Init("Nội dung", "Bạn đang ở trong tù. Còn " + banSteps + " lượt.");
            yield return new WaitWhile(() => clonePopupNotify.isPending);
            
            clonePopupNotify.ClosePopup();
            if(--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;

            ListPlayer[currentTurn].BanSteps--;
            currentTurn = (currentTurn + 1) % 2;
            StartCoroutine(ShowButtonRotate());
        }


        IEnumerator WaitForDice()
        {
            yield return new WaitForSeconds(3f);
            isStop = true;
        }

        IEnumerator GameOver()
        {
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = false;
            inputManager.NumberOfPopupExist++;
            var clonePopupGameOver = Instantiate(popupGameOver);
            clonePopupGameOver.Init();

            var winner = (ListPlayer[0].Money < 0) ? ListPlayer[1] : ListPlayer[0];
            clonePopupGameOver.SetWinnerName(winner.PlayerName);
            yield return new WaitWhile(() => clonePopupGameOver.isPending);

            clonePopupGameOver.ClosePopup();
            if (--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;
        }

        private bool IsGameOver()
        {
            for (int i = 0; i < 2; i++)
                if (ListPlayer[i].Money < 0)
                    return true;
            return false;
        }

        IEnumerator ShowButtonRotate()
        {
            if (IsGameOver())
            {
                StartCoroutine(GameOver());
                yield break;
            }
            else if (ListPlayer[currentTurn].BanSteps > 0)
                StartCoroutine(ShowSkipStep());
            else
            {
                ButtonRotateViewModel btnRotate = Instantiate(buttonRotateViewModel) as ButtonRotateViewModel;
                btnRotate.Init();
                yield return new WaitWhile(() => btnRotate.IsPending);
                Rotate();
                btnRotate.ClosePopup();
            }
        }

        public void Rotate()
        {
            ListPlayerInfo[currentTurn].UpdateColor(PlayerState.INTURN);
            ListPlayerInfo[1 - currentTurn].UpdateColor(PlayerState.WAITING);
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = false;
            StartCoroutine(WaitForQuestion());
        }

        IEnumerator WaitForQuestion()
        {
            var cloneQuestionPopup = Instantiate(originQuestionPopup) as QuestionViewModel;
            cloneQuestionPopup.Init();
            cloneQuestionPopup.LoadQuestionContent(listQuestion.GetRandQuestion());
            yield return new WaitWhile(()=>cloneQuestionPopup.isPending);
            yield return new WaitForSeconds(1f);
            var result = cloneQuestionPopup.Result;
            cloneQuestionPopup.ClosePopup();
            if (result)
                StartCoroutine(ChooseOpportunity());
            else
                StartTurn();
        }

        IEnumerator ChooseOpportunity()
        {
            var cloneDiceOptionPopup = Instantiate(originDiceOptionPopup) as DiceOptionViewModel;
            yield return new WaitWhile(() => cloneDiceOptionPopup.isPending);
            switch (cloneDiceOptionPopup.Oppotunity)
            {
                case OpportunityOptions.EVEN:
                    {
                        int posOdd = (int)Random.Range(0.5f, 1.5f);
                        dice[1 - posOdd].Option = DiceOption.EVEN;
                        dice[posOdd].Option = DiceOption.ODD;
                    }
                    break;
                case OpportunityOptions.ODD:
                    {
                        int even = (int)Random.Range(0f, 1f);
                        var style = (even == 0) ? DiceOption.EVEN : DiceOption.ODD;
                        dice[0].Option = style;
                        dice[1].Option = style;
                    }
                    break;
                case OpportunityOptions.MONEY:
                    {
                        ListPlayer[currentTurn].ReceiveSalary(200);
                        ListPlayerInfo[currentTurn].UpdateMoneyPlayerInfo();
                        var style = DiceOption.NORMAL;
                        dice[0].Option = style;
                        dice[1].Option = style;
                    }
                    break;
                case OpportunityOptions.DEFAULT:
                    {
                        var style = DiceOption.NORMAL;
                        dice[0].Option = style;
                        dice[1].Option = style;
                    }
                    break;
                default:
                    break;
            }
            cloneDiceOptionPopup.ClosePopup();
            StartTurn();
        }

        private void StartTurn()
        {
            for (var i = 0; i < 2; i++)
                dice[i].StartDice();
            isRotate = true;
            isStop = false;
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = true;
        }

        private void LoadMap()
        {
            var map = ListPlaceJson.LoadFromJson();
            for (int i = 0; i < 24; i++)
            {
                var localPlace = PlaceFactory.GetPlace(map.ListPlaces[i]);
                listPlaces[i] = (PlaceViewModel)Instantiate(place);
                listPlaces[i].Position = i + 1;
                listPlaces[i].PlaceInstance = localPlace;
                listPlaces[i].GetComponent<Renderer>().material.color = localPlace.PlaceColor;
                listPlaces[i].Type = PlaceFactory.GetPlaceType(map.ListPlaces[i]);
            }
            Init();
        }


        //Popup Choose Building
        void ShowConfirm(NormalPlace normalPlace)
        {

            if (listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].HasOwner())
            {
                if(listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].IsOwner(ListPlayer[currentTurn]) &&
                    listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].CanUpgrade())
                    StartCoroutine(ShowUpgradePlaceConfirm(normalPlace));
                else
                    StartCoroutine(ShowNotifyPopup());
            }
            else
                StartCoroutine(ShowBuyPlaceConfirm(normalPlace));
        }

        private IEnumerator ShowBuyPlaceConfirm(NormalPlace normalPlace)
        {
            var audioResource = GetComponent<AudioSource>();

            var inputManager = InputManager.GetInstance();
            var audioManager = AudioManager.GetInstance();
            inputManager.IsAllowed = false;
            inputManager.NumberOfPopupExist++;
            if (ListPlayer[currentTurn].Money >= normalPlace.SellingPrice)
            {
                
                if (audioManager.IsEffectSound)
                {
                    var audioPath1 = Resources.Load("Audio/upgrade", typeof(AudioClip)) as AudioClip;
                    audioResource.clip = audioPath1;
                    audioResource.Play();
                }
                var cloneConfirmPopup = Instantiate(originConfirmBuyPopup) as ChooseBuildingViewModel;
                cloneConfirmPopup.Init(normalPlace);
                yield return new WaitWhile(() => cloneConfirmPopup.isPending);
                if (cloneConfirmPopup.Result)
                {
                    if (audioManager.IsEffectSound)
                    {
                        var audioPath2 = Resources.Load("Audio/recieveMoney", typeof(AudioClip)) as AudioClip;
                        audioResource.clip = audioPath2;
                        audioResource.Play();
                    }
                    ListPlayer[currentTurn].BuyPlace(normalPlace);
                    var resource = "Star/star" + currentTurn;
                    listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].ChangeSymbol(resource);
                    ListPlayerInfo[currentTurn].UpdatePlayerInfo();
                }
                cloneConfirmPopup.ClosePopup();
            }
            else
            {
                if (audioManager.IsEffectSound)
                {
                    var audioPath = Resources.Load("Audio/loss", typeof(AudioClip)) as AudioClip;
                    audioResource.clip = audioPath;
                    audioResource.Play();
                }
                var clonePopupNotify = Instantiate(popupNotify);
                clonePopupNotify.Init("Nội dung", "Bạn không đủ tiền để thực hiện mua ô đất này.");
                yield return new WaitWhile(() => clonePopupNotify.isPending);
                clonePopupNotify.ClosePopup();
            }
            if(--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;
            currentTurn = (currentTurn + 1) % 2;
            StartCoroutine(ShowButtonRotate());
        }

        private IEnumerator ShowUpgradePlaceConfirm(NormalPlace normalPlace)
        {
            var audioResource = GetComponent<AudioSource>();
            var audioManager = AudioManager.GetInstance();
            if (audioManager.IsEffectSound)
            {
                var resource1 = Resources.Load("Audio/upgrade", typeof(AudioClip)) as AudioClip;
                audioResource.clip = resource1;
                audioResource.Play();
            }
            var inputManager = InputManager.GetInstance();
            inputManager.IsAllowed = false;
            inputManager.NumberOfPopupExist++;
            var cloneConfirmPopup = Instantiate(originConfirmUpgradePopup) as ChooseBuildingViewModel;
            cloneConfirmPopup.Init(normalPlace);
            yield return new WaitWhile(() => cloneConfirmPopup.isPending);
            if (cloneConfirmPopup.Result)
            {
                if (audioManager.IsEffectSound)
                {
                    var resource2 = Resources.Load("Audio/recieveMoney", typeof(AudioClip)) as AudioClip;
                    audioResource.clip = resource2;
                    audioResource.Play();
                }
                ListPlayer[currentTurn].UpgradePlace(normalPlace);
                listPlaces[ListPlayer[currentTurn].CurrentPosition - 1].Upgrade();
                ListPlayerInfo[currentTurn].UpdatePlayerInfo();
            }
           
            cloneConfirmPopup.ClosePopup();
            ListPlayerInfo[0].UpdatePlayerInfo();
            ListPlayerInfo[1].UpdatePlayerInfo();
            currentTurn = (currentTurn + 1) % 2;
            if(--inputManager.NumberOfPopupExist == 0)
                inputManager.IsAllowed = true;
            StartCoroutine(ShowButtonRotate());
        }
    }
}

