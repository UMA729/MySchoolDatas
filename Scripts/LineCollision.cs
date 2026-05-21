using UnityEngine;

public class LineCollision : MonoBehaviour
{
    public StrokeController.LineData line_data;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        Debug.Log("‚ ‚½‚½");
        if(collision.gameObject.CompareTag("Gimmick"))
        {
            line_data.life--;
            Destroy(collision.gameObject);
        }
    }

}
