using System;
using System.Runtime.CompilerServices;

namespace Chilli
{
    internal static class InternalCalls
    {
        #region Core
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Log(string message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_FindByName(string name);
        #endregion

        #region Transform
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong entityId, out Vector3 translation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong entityId, ref Vector3 translation);
        #endregion

        #region RigidBody2D
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_ApplyLinearImpulse(ulong entityID, ref Vector2 impulse, ref Vector2 point, bool wake);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_ApplyLinearImpulseToCenter(ulong entityID, ref Vector2 impulse, bool wake);
        #endregion

        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keyCode);
        #endregion

    }
}
