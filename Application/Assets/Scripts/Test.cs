using Chilli;

namespace Application
{
    public class Test : ChilliScript
    {
        private TransformComponent TransformComponent;
       
        public float Speed = 3.0f;
        public float Time = 0.0f;
        public Vector2 Velocity = new Vector2(1.0f, 5.0f);
        public override void OnCreate()
        {
            
            TransformComponent = Entity.GetComponent<TransformComponent>();
        }

        public override void OnDestroy()
        {
            Core.Log("Destroy function called");
        }

        public override void OnUpdate(float dt)
        {
            Time += dt;
            Vector3 velocity = new Vector3(0.0f,0.0f,0.0f);

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Z = Speed;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Z = -Speed;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -Speed;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = Speed;

            Vector3 translation = TransformComponent.Translation;
            translation += velocity * dt;
            TransformComponent.Translation = translation;
        }
    }
}
