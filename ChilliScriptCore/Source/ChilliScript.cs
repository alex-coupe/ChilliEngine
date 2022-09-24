using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Chilli
{
    public abstract class ChilliScript
    {
        protected Entity Entity;
        protected ChilliScript() { }

        internal ChilliScript(Entity owningEntity)
        {
            Entity = owningEntity;
        }
        
        public abstract void OnCreate();

        public abstract void OnUpdate(float dt);

        public abstract void OnDestroy();

    }
}
