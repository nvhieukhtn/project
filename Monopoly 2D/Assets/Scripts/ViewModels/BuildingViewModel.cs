using UnityEngine;
using System.Collections;

namespace AssemblyCSharp
{
    public class BuildingViewModel : MonoBehaviour
    {
        public BaseBuilding Building;

        public int Position { get; set; }

        public BuildingType Type;

        void Start()
        {
            if (Building != null)
            {
                Material mat = Building.LoadMaterial();
                GetComponent<Renderer>().material = mat;
                var position2D = Shared.ConvertPosition(this.Position);
                transform.position = new Vector3(position2D.x, position2D.y, -9f);
            }
        }

        public void Reset()
        {
            Building = null;
            var mat = GetComponent<Renderer>().material;
            var postion = transform.position;
            transform.position = new Vector3(postion.x, postion.y, 0);
            Destroy(mat);
        }

        public bool CanUpgrade()
        {
            return Building == null || (Type == BuildingType.UPGRADE && ((UpgradableBuilding)Building).Level < 4);
        }
        private void UpdateBuilding()
        {
            if (Building != null)
            {
                Material mat = Building.LoadMaterial();
                GetComponent<Renderer>().material = mat;
                var position2D = Shared.ConvertPosition(this.Position);
                transform.position = new Vector3(position2D.x, position2D.y, -9f);
            }
        }

        public void Upgrade()
        {
            if (Building == null)
            {
                Building = BuildingFactory.GetBuilding(Type);
            }
            else
                ((UpgradableBuilding)Building).Upgrade();
            UpdateBuilding();
        }
    }
}