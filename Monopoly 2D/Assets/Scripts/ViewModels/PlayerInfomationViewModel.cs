using UnityEngine;
using UnityEngine.UI;
using System.Collections;
namespace AssemblyCSharp
{
    public class PlayerInfomationViewModel : MonoBehaviour
    {
        public Player player { get; set; }
        Text PlayerNameText;
        Text NumberOfPlacesText;
        Text MoneyText;
        Image Background;
        Image Symbol;
        // Use this for initialization
        void Start()
        {
            
        }

        public void Init()
        {
            Text[] controllUIs = gameObject.GetComponentsInChildren<Text>();
            for (int i = 0; i < controllUIs.Length; i++)
            {
                switch (controllUIs[i].name)
                {
                    case "Name":
                        PlayerNameText = controllUIs[i];
                        break;
                    case "NumPlace":
                        NumberOfPlacesText = controllUIs[i];
                        break;
                    case "Money":
                        MoneyText = controllUIs[i];
                        break;
                }
            }

            var images = gameObject.GetComponentsInChildren<Image>();
            for (int i = 0;i < images.Length;i++)
            {
                if (images[i].name == "Background")
                    Background = images[i];
                else if (images[i].name == "Symbol")
                    Symbol = images[i];
            }
        }
        
        public void UpdateSymbol(int type)
        {
            string resource = "PlayerInfo/star" + type;
            Symbol.sprite = (Sprite)Resources.Load(resource, typeof(Sprite));
        }
        public void UpdateColor(PlayerState state)
        {
            switch (state)
            {
                case PlayerState.INTURN:
                    Background.color = new Color(0, 105, 92);
                    break;
                case PlayerState.WAITING:
                    Background.color = new Color(38, 166, 154);
                    break;
                default:
                    break;
            }
        }
        public void UpdatePlayerInfo()
        {
            UpdatePlayerName();
            UpdateMoneyPlayerInfo();
            UpdateNumberOfPlaces();
        }
        public void UpdatePlayerName()
        {
            PlayerNameText.text = player.PlayerName;
        }
        public void UpdateMoneyPlayerInfo()
        {
            MoneyText.text = player.Money.ToString() + " $";
        }

        public void UpdateNumberOfPlaces()
        {
            NumberOfPlacesText.text = player.NumberOfPlace.ToString();
        }

        void Update()
        {

        }
    }
}
