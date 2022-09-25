using Chilli;

namespace Application
{
    public class Test
    {
        public void OnCreate()
        {
                Core.Log("Entity has transform component");
        }

        public  void OnDestroy()
        {
            Core.Log("Destroy function called");
        }

        public void OnUpdate(float dt)
        {
            if (Input.IsKeyDown(KeyCode.A))
            {
                Core.Log("A pressed in update function");
            }
        }
    }
}
