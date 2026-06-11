using UnityEditor.Rendering;
using UnityEngine;

public class Gimmick : MonoBehaviour
{
    [SerializeField]private GameObject Gummick;
    [SerializeField] private float press_limmit = 0;

    public bool isPressing = true;

    float gimmick_time = 0;
    float press_speed = 0;
    float origine_pos;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        press_limmit += transform.position.y;
        origine_pos = transform.position.y;
    }

    // Update is called once per frame
    void Update()
    {
        if (gameObject.CompareTag("Arrow"))
        {
            Arrow();
        }
        else if (gameObject.CompareTag("Press"))
        {
            Press();
        }
    }

    void Arrow()
    { 
        gimmick_time += Time.deltaTime;
        if (gimmick_time > 0.5)
        {
            Instantiate(Gummick, transform.position, transform.rotation);
            gimmick_time = 0;
        }
    }

    void Press()
    {
        gimmick_time += Time.deltaTime;
        
        if (gimmick_time > 3)
        {
            //プレス前
            if (isPressing)
            {
                press_speed = 5.0f;
                transform.position -= transform.up * press_speed * Time.deltaTime;
                if (transform.position.y < press_limmit)
                {
                    isPressing = false;
                }
            }
            //プレス後
            if (!isPressing)
            {
                press_speed = 1.0f;
                transform.position += transform.up * press_speed * Time.deltaTime;
                
                if(transform.position.y > origine_pos)
                { 
                    isPressing = true;
                    gimmick_time = 0;
                }
            }
        }
    }

    void Hole()
    {

    }
}