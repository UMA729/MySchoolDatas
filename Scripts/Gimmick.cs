using UnityEngine;

public class Gimmick : MonoBehaviour
{
    [SerializeField]private GameObject Gummick;

    float gimmick_time = 0;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        gimmick_time += Time.deltaTime;
        if (gimmick_time > 0.5)
        {
            Instantiate(Gummick, transform.position, transform.rotation);
            gimmick_time = 0;
        }
    }
}