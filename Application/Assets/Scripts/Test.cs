using Chilli;

namespace Application
{
    public class Test
    {
        public void OnCreate()
        {
            Engine.Log("Create function called");
        }

        public void OnUpdate(float dt)
        {
            if (Input.IsKeyDown(KeyCode.A))
            {
                Engine.Log("A pressed in update function");
            }
        }
    }
}
