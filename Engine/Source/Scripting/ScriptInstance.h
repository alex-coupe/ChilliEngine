#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Script.h"

namespace Chilli {

	enum class FieldType : int {
		Unknown, Float, Double, Bool, Char, Short, Int, Long, Byte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4, Entity, TransformComponent, RigidBody2DComponent
	};

	static std::unordered_map<std::string, FieldType> s_fieldTypeMap =
	{
		{"System.Single",FieldType::Float},
		{"System.Double",FieldType::Double},
		{"System.Boolean",FieldType::Bool},
		{"System.Char", FieldType::Char},
		{"System.Int16", FieldType::Short},
		{"System.Int32", FieldType::Int},
		{"System.Int64", FieldType::Long},
		{"System.Byte", FieldType::Byte},
		{"System.UInt16", FieldType::UShort},
		{"System.UInt32", FieldType::UInt},
		{"System.UInt64", FieldType::ULong},
		{"Chilli.Vector2", FieldType::Vector2},
		{"Chilli.Vector3", FieldType::Vector3},
		{"Chilli.Vector4", FieldType::Vector4},
		{"Chilli.Entity", FieldType::Entity},
		{"Chilli.TransformComponent", FieldType::TransformComponent},
		{"Chilli.RigidBody2DComponent", FieldType::RigidBody2DComponent}
	};

	struct Field {
		FieldType Type;
		std::string Name;
		MonoClassField* ClassField;
		std::string ScriptName;
	};

	class CHILLI_API ScriptInstance {
	public:
		ScriptInstance(MonoClass* scriptClass, uint64_t entityId);
		~ScriptInstance();
		MonoObject* GetMonoObject()const;
		MonoMethod* GetDestroyMethod()const;
		MonoMethod* GetCreateMethod()const;
		MonoMethod* GetUpdateMethod()const;
		void GetFieldValue(const std::string& fieldName, void* buffer)const;
		void SetFieldValue(const std::string& fieldName, void* buffer)const;
		const std::unordered_map<std::string, Field>& GetFields()const;
		inline static char s_FieldValueBuffer[16];
	private:
		const FieldType MonoTypeToFieldType(MonoType* type)const;
		const char* FieldTypeToString(FieldType type)const;
		MonoObject* m_monoObject = nullptr;
		MonoMethod* m_createMethod = nullptr;
		MonoMethod* m_updateMethod = nullptr;
		MonoMethod* m_destroyMethod = nullptr;
		std::string m_className;
		std::unordered_map<std::string, Field> m_fields;
	};
}