using System;
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;

namespace AssemblyCSharp
{
    public class PlaceViewModel : MonoBehaviour
    {
        public int Position { get; set; }
        public PlaceType Type;
        public BasePlace PlaceInstance { get; set; }
        public PlaceInfomationViewModel PlaceInfo;
        public BuildingViewModel buildingViewModel;
        public Symbol symbol;
        private Symbol cloneSymbol;

        void Start()
        {
            if (PlaceInstance == null)
            {
                Debug.Log("Can't load place");
                return;
            }
            Material mat = PlaceInstance.LoadMaterial();
            GetComponent<Renderer>().material = mat;
            transform.position = Shared.ConvertPosition(this.Position);
            buildingViewModel = (BuildingViewModel)Instantiate(buildingViewModel);
            buildingViewModel.Position = Position;
            buildingViewModel.transform.position = transform.position;
            if (Type == PlaceType.Service)
                buildingViewModel.Type = BuildingType.UNUPGRADE;
            else
                buildingViewModel.Type = BuildingType.UPGRADE;
        }
        public void Reset()
        {
            if (cloneSymbol != null)
            {
                Destroy(cloneSymbol.gameObject);
                cloneSymbol = null;
            }
            PlaceInstance.Reset();
            buildingViewModel.Reset();
        }
        public bool CanUpgrade()
        {
            return buildingViewModel.CanUpgrade();
        }

        public void Upgrade()
        {
            ((NormalPlace)PlaceInstance).TaxPrice += ((NormalPlace)PlaceInstance).UpgradePrice;
            if (buildingViewModel.CanUpgrade())
                buildingViewModel.Upgrade();
        }
        public PlaceViewModel(BasePlace place)
        {
            this.PlaceInstance = place;
        }
        public PlaceViewModel()
        {

        }
        private void Update()
        {
            
        }
        void OnMouseUp()
        {
            var inputManager = InputManager.GetInstance();
            if (inputManager.IsAllowed)
            {
                inputManager.IsAllowed = false;
                inputManager.NumberOfPopupExist++;
                var placeInfoInstance = (PlaceInfomationViewModel)Instantiate(PlaceInfo, new Vector3(365f, 358f, 0), Quaternion.identity);
                placeInfoInstance.SetPlaceName(PlaceInstance.Name);
                placeInfoInstance.SetContent(PlaceInstance.Describe);
            }
        }

        public bool CanBuilding()
        {
            return (Type == PlaceType.Normal || Type == PlaceType.Service);
        }

        public bool HasOwner()
        {
            return (Type == PlaceType.Normal || Type == PlaceType.Service) &&
                ((NormalPlace)PlaceInstance).HasOwner();
        }

        public bool IsOwner(Player player)
        {
            var owner = ((NormalPlace)PlaceInstance).Owner;
            return owner.PlayerName == player.PlayerName;
        }
        public void ChangeOwner(Player owner)
        {
            if (Type == PlaceType.Normal || Type == PlaceType.Service)
                ((NormalPlace)PlaceInstance).ChangeOwner(owner);
        }

        public void ChangeSymbol(string resource)
        {
            cloneSymbol = (Symbol)Instantiate(symbol);
            var material = (Material)Resources.Load(resource, typeof(Material));
            cloneSymbol.GetComponent<Renderer>().material = material;
            cloneSymbol.transform.position = new Vector3(transform.position.x - 40, transform.position.y - 40, -9f);
        }
        public void Notify()
        {
            if (Type == PlaceType.Normal ||Type == PlaceType.Service)
            {
                var money = ((NormalPlace)PlaceInstance).TaxPrice;
                if (((NormalPlace)PlaceInstance).Owner != null)
                    ((NormalPlace)PlaceInstance).NotifyOwnerReceiveMoney(money);
            }
        }
        public bool IsJail()
        {
            return ((SpecialPlace)PlaceInstance).GetPlaceType() == "Jail";
        }
        public bool IsGo()
        {
            return ((SpecialPlace)PlaceInstance).GetPlaceType() == "Go";
        }
    }
}

