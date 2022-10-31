using Chilli;

namespace Application
{
    public class TestTwo : ChilliScript
    {
        private TransformComponent TransformComponent;
        private RigidBody2DComponent RigidBody2DComponent;

        public float Speed;
        public float Time = 0.0f;

        public override void OnCreate()
        {
            TransformComponent = Entity.GetComponent<TransformComponent>();
            RigidBody2DComponent = Entity.GetComponent<RigidBody2DComponent>();
        }

        public override void OnDestroy()
        {
            throw new System.NotImplementedException();
        }

        public override void OnUpdate(float dt)
        {
            Time += dt;
            Vector2 velocity = new Vector2(0.0f, 0.0f);

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= 1.0f * dt;

            RigidBody2DComponent.ApplyLinearImpulse(velocity, true);
        }
    }
}
