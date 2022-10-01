#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Script.h"

namespace Chilli {

	enum class FieldType : int {
		Unknown, Float, Double, Bool, Char, Short, Int, Long, Byte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4, Entity
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
		{"Chilli.Entity", FieldType::Entity}
	};

	struct Field {
		FieldType Type = FieldType::Unknown;
		std::string Name;
		MonoClassField* ClassField = nullptr;
		std::string ScriptName;
		char FieldValueBuffer[16] = {};
	};

	class CHILLI_API ScriptInstance {
	public:
		ScriptInstance(MonoClass* scriptClass, uint64_t entityId);
		~ScriptInstance();
		bool HasFields()const;
		MonoObject* GetMonoObject()const;
		MonoMethod* GetDestroyMethod()const;
		MonoMethod* GetCreateMethod()const;
		MonoMethod* GetUpdateMethod()const;
		template <typename T>
		T GetFieldValue(const std::string& fieldName)const
		{
			const auto& field = m_fields.find(fieldName);
			if (field != m_fields.end())
			{
				GetFieldValueInternal(fieldName, (void*)field->second.FieldValueBuffer);
				return *(T*)field->second.FieldValueBuffer;
			}
			return T();
		}
		template <typename T>
		void SetFieldValue(const std::string& fieldName, T value)const
		{
			SetFieldValueInternal(fieldName, &value);
		}
		const std::unordered_map<std::string, Field>& GetFields()const;
	private:
		void GetFieldValueInternal(const std::string& fieldName, void* buffer)const;
		void SetFieldValueInternal(const std::string& fieldName, void* buffer)const;
		const FieldType MonoTypeToFieldType(MonoType* type)const;
		MonoObject* m_monoObject = nullptr;
		MonoMethod* m_createMethod = nullptr;
		MonoMethod* m_updateMethod = nullptr;
		MonoMethod* m_destroyMethod = nullptr;
		std::string m_className;
		std::unordered_map<std::string, Field> m_fields;
	};
}