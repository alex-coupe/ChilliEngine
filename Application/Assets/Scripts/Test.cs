using Chilli;

namespace Application
{
    public class Test : ChilliScript
    {
        public override void OnCreate()
        {
            if (Entity.HasComponent<TransformComponent>())
                Core.Log("Entity has transform component");
        }

        public override void OnDestroy()
        {
            Core.Log("Destroy function called");
        }

        public override void OnUpdate(float dt)
        {
            if (Input.IsKeyDown(KeyCode.A))
            {
                Core.Log("A pressed in update function");
            }
        }
    }
}
