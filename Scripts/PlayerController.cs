using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerController : MonoBehaviour
{
    [SerializeField] private float speed = 3f;
    [SerializeField] private float jumpForce = 5f;
    [SerializeField] private int HP = 3;

    private Vector2 moveInput = Vector2.zero;

    private Rigidbody2D rb;
    public bool isGrounded;

    StrokeController SC;
    StartPosition SP;
    Cage cage;

    private void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        SC = FindAnyObjectByType<StrokeController>();
        SP = FindAnyObjectByType<StartPosition>();
        cage = FindAnyObjectByType<Cage>();
    }

    private void Update()
    {
        if (SC.now_stroke)
            return;
        var move = new Vector3(moveInput.x, 0f, moveInput.y) * speed * Time.deltaTime;
        transform.Translate(move);
    }

    private void FixedUpdate()
    {

        
    }

    public void OnMove(InputAction.CallbackContext context)
    {

        moveInput = context.ReadValue<Vector2>();
    }

    public void OnJump(InputAction.CallbackContext context)
    {
        if (context.started && isGrounded)
        {

            rb.AddForce(Vector3.up * jumpForce, ForceMode2D.Impulse);
            rb.linearVelocity = new Vector2(0f, rb.linearVelocity.y);
            Debug.Log("Jumping");
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("AttackGimmick"))
        {
            Destroy(collision.gameObject);
            --HP;
            if (HP == 0)
            {
                SP.PlayerSpawn();
                Destroy(this.gameObject);
                HP = 3;
            }
        }
        if (collision.gameObject.CompareTag("Key"))
        {
            if (!GameManager.instance.hasKey)
            {
                GameManager.instance.hasKey = true;
                Destroy(collision.gameObject);
            }
            else
            {
                Debug.Log("åÆÇ‡Ç¡ÇƒÇ‹Ç∑ÇÊÅ[");
            }
              
        }
    }

    private void OnCollisionStay2D(Collision2D collision)
    {
       
        if (collision.gameObject.CompareTag("Ground"))
        {
            isGrounded = true;
        }
        if (collision.gameObject.CompareTag("Cage") && GameManager.instance.hasKey)
        {
            GameManager.instance.hasKey = false;
            cage.isOpen = true;
        }
    }
    
    private void OnCollisionExit2D(Collision2D collision)
    {

        if (collision.gameObject.CompareTag("Ground"))
        {
            isGrounded = false;
        }
    }
}
