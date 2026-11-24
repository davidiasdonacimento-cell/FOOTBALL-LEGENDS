using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace FootballLegends.Match
{
    public sealed class VirtualJoystick : MonoBehaviour, IPointerDownHandler, IDragHandler, IPointerUpHandler
    {
        [SerializeField] private RectTransform handle;
        [SerializeField] private float radius = 70f;
        public Vector2 Value { get; private set; }

        public void Configure(RectTransform knob, float movementRadius)
        {
            handle = knob;
            radius = Mathf.Max(20f, movementRadius);
        }

        public void OnPointerDown(PointerEventData eventData) { OnDrag(eventData); }

        public void OnDrag(PointerEventData eventData)
        {
            RectTransform rect = (RectTransform)transform;
            if (!RectTransformUtility.ScreenPointToLocalPointInRectangle(
                rect, eventData.position, eventData.pressEventCamera, out Vector2 localPoint))
                return;

            Value = Vector2.ClampMagnitude(localPoint / radius, 1f);
            if (handle != null) handle.anchoredPosition = Value * radius;
        }

        public void OnPointerUp(PointerEventData eventData)
        {
            Value = Vector2.zero;
            if (handle != null) handle.anchoredPosition = Vector2.zero;
        }
    }
}