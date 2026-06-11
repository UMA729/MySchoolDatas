using UnityEngine;

public class LineCollision : MonoBehaviour
{
    public StrokeController.LineData line_data;
    Gimmick gimmick;

    private void Start()
    {
        gimmick = FindAnyObjectByType<Gimmick>();
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("AttackGimmick"))
        {
            line_data.life--;
            Destroy(collision.gameObject);
        }
        if (collision.gameObject.CompareTag("Press") && gimmick.isPressing)
        {
            if(line_data.totalLength > 2.0f)
            {
                gimmick.isPressing = false;
            }
            else if (line_data.totalLength > 1.0f)
            {
                line_data.life -= 3;
                gimmick.isPressing = false;
            }
            else
            {
                line_data.life -= 3;
            }
        }
    }
}
