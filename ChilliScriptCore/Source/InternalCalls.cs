using System;
using System.Runtime.CompilerServices;

namespace Chilli
{
    internal static class InternalCalls
    {
        #region Core
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Log(string message);
        #endregion

        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keyCode);
        #endregion

    }
}
