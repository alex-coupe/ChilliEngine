using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Chilli
{
    public class Engine
    {
        public static void Log(string message)
        {
            InternalCalls.Log(message);
        }
    }
}
