using UnityEngine;

namespace FootballLegends.Match
{
    public sealed class MatchPlayer : MonoBehaviour
    {
        public int TeamId { get; private set; }
        public int ShirtNumber { get; private set; }
        public bool IsGoalkeeper { get; private set; }
        public bool IsSentOff { get; private set; }
        public int YellowCards { get; private set; }
        public bool IsComplaining { get; private set; }

        private Transform torso, head, leftArm, rightArm, leftLeg, rightLeg, mouth;
        private Vector3 homePosition;
        private float animationClock;
        private float complaintUntil;
        private float lookUntil;
        private Vector3 lookDirection = Vector3.forward;
        private Renderer shirtRenderer;
        private GameObject cardVisual;
        private float cardUntil;
        private float kickUntil;

        public void Initialize(int teamId, int shirtNumber, bool goalkeeper = false)
        {
            TeamId = teamId;
            ShirtNumber = shirtNumber;
            IsGoalkeeper = goalkeeper;
            homePosition = transform.position;
            BuildHumanoid(teamId == 0 ? new Color(0.08f, 0.28f, 0.88f) : new Color(0.82f, 0.08f, 0.08f));
        }

        private void BuildHumanoid(Color kit)
        {
            foreach (Transform child in transform) Destroy(child.gameObject);
            transform.localScale = Vector3.one;
            torso = Part("Torso", PrimitiveType.Capsule, new Vector3(0f, 0.15f, 0f),
                new Vector3(0.48f, 0.62f, 0.30f), kit);
            shirtRenderer = torso.GetComponent<Renderer>();
            Part("Shorts", PrimitiveType.Cube, new Vector3(0f, -0.30f, 0f),
                new Vector3(0.43f, 0.23f, 0.28f), new Color(0.08f, 0.09f, 0.12f));
            head = Part("Head", PrimitiveType.Sphere, new Vector3(0f, 0.70f, 0f),
                new Vector3(0.27f, 0.30f, 0.27f), new Color(0.72f, 0.53f, 0.39f));
            leftArm = Part("Left Arm", PrimitiveType.Capsule, new Vector3(-0.31f, 0.12f, 0f),
                new Vector3(0.13f, 0.43f, 0.13f), kit);
            rightArm = Part("Right Arm", PrimitiveType.Capsule, new Vector3(0.31f, 0.12f, 0f),
                new Vector3(0.13f, 0.43f, 0.13f), kit);
            leftLeg = Part("Left Leg", PrimitiveType.Capsule, new Vector3(-0.13f, -0.61f, 0f),
                new Vector3(0.15f, 0.48f, 0.16f), new Color(0.12f, 0.13f, 0.16f));
            rightLeg = Part("Right Leg", PrimitiveType.Capsule, new Vector3(0.13f, -0.61f, 0f),
                new Vector3(0.15f, 0.48f, 0.16f), new Color(0.12f, 0.13f, 0.16f));
            mouth = Part("Mouth Animation", PrimitiveType.Cube, new Vector3(0f, 0.62f, 0.245f),
                new Vector3(0.10f, 0.025f, 0.018f), new Color(0.18f, 0.035f, 0.025f));
            Part("Boot L", PrimitiveType.Cube, new Vector3(-0.13f, -0.88f, 0.08f),
                new Vector3(0.18f, 0.09f, 0.30f), Color.black);
            Part("Boot R", PrimitiveType.Cube, new Vector3(0.13f, -0.88f, 0.08f),
                new Vector3(0.18f, 0.09f, 0.30f), Color.black);
            if (IsGoalkeeper)
            {
                Part("Keeper Gloves L", PrimitiveType.Sphere, new Vector3(-0.40f, -0.06f, 0.02f),
                    new Vector3(0.16f, 0.13f, 0.13f), new Color(1f, 0.75f, 0.05f));
                Part("Keeper Gloves R", PrimitiveType.Sphere, new Vector3(0.40f, -0.06f, 0.02f),
                    new Vector3(0.16f, 0.13f, 0.13f), new Color(1f, 0.75f, 0.05f));
            }
        }

        private Transform Part(string label, PrimitiveType type, Vector3 localPosition, Vector3 scale, Color color)
        {
            GameObject part = GameObject.CreatePrimitive(type);
            part.name = label;
            part.transform.SetParent(transform, false);
            part.transform.localPosition = localPosition;
            part.transform.localScale = scale;
            Collider collider = part.GetComponent<Collider>();
            if (collider != null) Destroy(collider);
            Renderer renderer = part.GetComponent<Renderer>();
            Shader shader = Shader.Find("Universal Render Pipeline/Lit");
            if (shader == null) shader = Shader.Find("Standard");
            renderer.material = new Material(shader);
            renderer.material.color = color;
            return part.transform;
        }

        public void MoveTo(Vector3 destination, float speed)
        {
            if (IsSentOff || IsComplaining) return;
            Vector3 flat = new Vector3(destination.x, transform.position.y, destination.z);
            Vector3 before = transform.position;
            transform.position = Vector3.MoveTowards(before, flat, speed * Time.deltaTime);
            Vector3 delta = transform.position - before;
            if (delta.sqrMagnitude > 0.0005f)
                transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(delta), 10f * Time.deltaTime);
        }

        public void Face(Vector3 point)
        {
            Vector3 direction = point - transform.position;
            direction.y = 0f;
            if (direction.sqrMagnitude > 0.01f)
                transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(direction), 8f * Time.deltaTime);
        }

        public void TriggerKick()
        {
            kickUntil = Time.time + 0.28f;
        }

        public void UpdateAnimation(float speed)
        {
            animationClock += Time.deltaTime * (speed > 0.2f ? 10f : 2f);
            if (Time.time < kickUntil && !IsComplaining)
            {
                rightLeg.localRotation = Quaternion.Euler(-72f, 0f, 0f);
                leftLeg.localRotation = Quaternion.Euler(18f, 0f, 0f);
                rightArm.localRotation = Quaternion.Euler(-18f, 0f, -12f);
                leftArm.localRotation = Quaternion.Euler(18f, 0f, 12f);
                return;
            }
            float swing = speed > 0.2f ? Mathf.Sin(animationClock) * 32f : Mathf.Sin(animationClock) * 2f;
            if (IsComplaining)
            {
                float complaintWave = Mathf.Sin(Time.time * 8f);
                leftArm.localRotation = Quaternion.Euler(0f, 0f, -65f - complaintWave * 24f);
                rightArm.localRotation = Quaternion.Euler(0f, 0f, 65f + complaintWave * 24f);
                head.localRotation = Quaternion.Euler(Mathf.Sin(Time.time * 3f) * 8f,
                    Mathf.Sin(Time.time * 2.4f) * 28f, 0f);
                mouth.localScale = new Vector3(0.10f, 0.025f + Mathf.Abs(complaintWave) * 0.065f, 0.018f);
                return;
            }
            leftLeg.localRotation = Quaternion.Euler(swing, 0f, 0f);
            rightLeg.localRotation = Quaternion.Euler(-swing, 0f, 0f);
            leftArm.localRotation = Quaternion.Euler(-swing * 0.7f, 0f, 0f);
            rightArm.localRotation = Quaternion.Euler(swing * 0.7f, 0f, 0f);
            if (speed <= 0.2f)
                head.localRotation = Quaternion.Euler(0f, Mathf.Sin(Time.time * 1.4f) * 18f, 0f);
            else
                head.localRotation = Quaternion.Slerp(head.localRotation, Quaternion.identity, Time.deltaTime * 5f);
            mouth.localScale = new Vector3(0.10f, 0.025f, 0.018f);
        }

        public void ReceiveCard(bool red)
        {
            if (red) IsSentOff = true;
            else
            {
                YellowCards++;
                if (YellowCards >= 2) IsSentOff = true;
            }
            IsComplaining = true;
            complaintUntil = Time.time + 3.8f;
            if (cardVisual != null) Destroy(cardVisual);
            cardVisual = GameObject.CreatePrimitive(PrimitiveType.Cube);
            cardVisual.name = red ? "Red Card Visual" : "Yellow Card Visual";
            cardVisual.transform.SetParent(transform, false);
            cardVisual.transform.localPosition = new Vector3(0.42f, 1.15f, 0.05f);
            cardVisual.transform.localScale = new Vector3(0.025f, 0.34f, 0.22f);
            Collider cardCollider = cardVisual.GetComponent<Collider>();
            if (cardCollider != null) Destroy(cardCollider);
            Renderer cardRenderer = cardVisual.GetComponent<Renderer>();
            cardRenderer.material.color = red ? Color.red : Color.yellow;
            cardUntil = Time.time + 3.8f;
        }

        public void TriggerComplaint()
        {
            IsComplaining = true;
            complaintUntil = Time.time + 3.8f;
        }

        private void Update()
        {
            if (cardVisual != null)
            {
                cardVisual.transform.localRotation = Quaternion.Euler(0f, Mathf.Sin(Time.time * 2f) * 10f, 0f);
                if (Time.time >= cardUntil) Destroy(cardVisual);
            }
            if (IsComplaining)
            {
                UpdateAnimation(0f);
                if (Time.time >= complaintUntil)
                {
                    IsComplaining = false;
                    if (mouth != null) mouth.localScale = new Vector3(0.10f, 0.025f, 0.018f);
                    if (leftArm != null) leftArm.localRotation = Quaternion.identity;
                    if (rightArm != null) rightArm.localRotation = Quaternion.identity;
                    if (head != null) head.localRotation = Quaternion.identity;
                }
            }
        }
    }
}