using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class StrokeController : MonoBehaviour
{
    [SerializeField] Material lineMaterial;
    [SerializeField] Color lineColor;
    [Range(0.1f, 0.5f)]
    [SerializeField] float lineWidth;

    [SerializeField] PhysicsMaterial2D bounceMaterial;
    [SerializeField] LayerMask blockLayer;
    [SerializeField] float lifeTime = 3f;

    [SerializeField] float maxGauge = 100f;
    [SerializeField] float gaugeCostPerUnit = 10f;

    [SerializeField] Image gauge;

    float currentGauge;

    public enum LineType
    {
        Normal,
        Weight,
        Spring
    }

    [System.Serializable]
    public class TimedPoint
    {
        public Vector2 position;
        public float time;

        public TimedPoint(Vector2 pos, float t)
        {
            position = pos;
            time = t;
        }
    }

    [System.Serializable]
    public class LineData
    {
        public LineType type;

        public GameObject obj;
        public LineRenderer renderer;
        public EdgeCollider2D collider;

        public List<TimedPoint> points = new List<TimedPoint>();

        public float life = 3f;

        public bool released = false;

        public float recoverLockTime = 3f;

        public float releaseTime;

        public int removedCount = 0;
        public int initialPointCount;

        public int startIndex = 0;
        public bool colliderDirty = true;

        public float totalLength = 0;

        public float alpha = 1f;
    }

    List<LineData> lines = new List<LineData>();
    LineData currentLine;

    public bool now_stroke = false;

    void Start()
    {
        currentGauge = maxGauge;
        gauge.fillAmount = 1f;
    }

    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            now_stroke = true;
            _createLine();
        }

        if (Input.GetMouseButton(0))
        {
            _addPoint();
        }

        if (Input.GetMouseButtonUp(0))
        {
            Debug.Log(currentLine.totalLength);

            now_stroke = false;


            if (currentLine != null)
            {
                currentLine.released = true;
                currentLine.releaseTime = Time.time + 3f;
                currentLine.initialPointCount = currentLine.points.Count;
                currentLine.colliderDirty = true;

            }
        }

        if (currentLine != null)
        {
            _updateAllLines();
        }
    }

    // ===================== LINE CREATE =====================
    private void _createLine()
    {
        GameObject obj = new GameObject("Line");
        obj.tag = "Ground";
        obj.transform.SetParent(transform);

        LineRenderer lr = obj.AddComponent<LineRenderer>();
        EdgeCollider2D col = obj.AddComponent<EdgeCollider2D>();
        col.sharedMaterial = bounceMaterial;

        lr.material = lineMaterial;
        lr.material.color = lineColor;
        lr.startWidth = lineWidth;
        lr.endWidth = lineWidth;
        lr.positionCount = 0;

        Rigidbody2D rb = obj.AddComponent<Rigidbody2D>();
        rb.bodyType = RigidbodyType2D.Kinematic;

        currentLine = new LineData
        {
            obj = obj,
            renderer = lr,
            collider = col,
            colliderDirty = true
        };

        obj.AddComponent<LineCollision>().line_data = currentLine;

        lines.Add(currentLine);
    }

    // ===================== POINT ADD =====================
    private void _addPoint()
    {
        if (currentLine == null) return;
        if (currentGauge <= 0f) return;

        Vector3 mousePos = new Vector3(
            Input.mousePosition.x,
            Input.mousePosition.y,
            1f
        );

        Vector3 worldPos =
            Camera.main.ScreenToWorldPoint(mousePos);

        Collider2D hit =
            Physics2D.OverlapPoint(worldPos, blockLayer);

        if (hit != null) return;

        if (currentLine.points.Count > 0)
        {
            Vector2 lastPos =
                currentLine.points[currentLine.points.Count - 1].position;

            float dist =
                Vector2.Distance(lastPos, worldPos);


            float cost = dist * gaugeCostPerUnit;

            Debug.Log(currentLine.totalLength);

            if (currentGauge < cost)
            {
                now_stroke = false;
                return;
            }

            currentLine.totalLength += dist;

            currentGauge -= cost;
            currentGauge = Mathf.Clamp(currentGauge, 0f, maxGauge);
            gauge.fillAmount = currentGauge / maxGauge;
        }

        currentLine.points.Add(
            new TimedPoint(worldPos, Time.time)
        );

        currentLine.colliderDirty = true;
    }

    // ===================== UPDATE ALL =====================
    private void _updateAllLines()
    {
        float now = Time.time;
        bool isDrawing = Input.GetMouseButton(0);

        for (int l = lines.Count - 1; l >= 0; l--)
        {
            var line = lines[l];

            if (line.life <= 0f)
            {
                Destroy(line.obj);
                lines.RemoveAt(l);
                continue;
            }

            // ===== auto fade =====
            if (line.released)
            {
                float elapsed = now - line.releaseTime;

                float t = elapsed / lifeTime;

                int targetRemove =
                    Mathf.FloorToInt(t * line.initialPointCount);

                targetRemove = Mathf.Clamp(
                    targetRemove,
                    0,
                    line.initialPointCount
                );

                int removeNow =
                    targetRemove - line.removedCount;

                int visible =
                    line.points.Count - line.startIndex;

                removeNow = Mathf.Clamp(removeNow, 0, visible);

                if (removeNow > 0)
                {
                    float recovered = 0f;

                    for (int i = 0; i < removeNow; i++)
                    {
                        int idx = line.startIndex + i;

                        if (idx + 1 < line.points.Count)
                        {
                            Vector2 a = line.points[idx].position;
                            Vector2 b = line.points[idx + 1].position;

                            recovered +=
                                Vector2.Distance(a, b) * gaugeCostPerUnit;
                        }
                    }

                    if (Time.time >= line.recoverLockTime)
                    {
                        currentGauge += recovered;
                        currentGauge = Mathf.Clamp(currentGauge, 0f, maxGauge);
                        gauge.fillAmount = currentGauge / maxGauge;
                    }

                    line.startIndex += removeNow;
                    line.removedCount += removeNow;

                    line.colliderDirty = true;
                }
            }

            int remain = line.points.Count - line.startIndex;

            if (remain < 2 && !isDrawing)
            {
                Destroy(line.obj);
                lines.RemoveAt(l);
                continue;
            }

            // ===== LineRenderer =====
            line.renderer.positionCount = remain;

            for (int i = 0; i < remain; i++)
            {
                line.renderer.SetPosition(
                    i,
                    line.points[line.startIndex + i].position
                );
            }

            // ===== Collider（軽量化済み）=====
            if (line.colliderDirty)
            {
                List<Vector2> pts = new List<Vector2>(remain);

                for (int i = line.startIndex; i < line.points.Count; i++)
                {
                    pts.Add(line.points[i].position);
                }

                line.collider.SetPoints(pts);
                line.colliderDirty = false;
            }
        }
    }
}