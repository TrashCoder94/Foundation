#include "fdpch.h"
#include "SceneSerializer.h"

#include "Foundation/Core/Types.h"
#include "Foundation/Components/Component.h"
#include "Foundation/Components/CameraComponent.h"
#include "Foundation/Components/IDComponent.h"
#include "Foundation/Components/SpriteComponent.h"
#include "Foundation/Components/ModelComponent.h"
#include "Foundation/Components/TagComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Objects/Object.h"
#include "Foundation/Renderer/Texture.h"
#include "Foundation/Renderer/Model.h"
#include "Reflect.h"
#include "Scene.h"

#include <fifo_map.hpp>
#include <json.hpp>
#include <ostream>
#include <xhash>

// A workaround to give to use fifo_map as map, we are just ignoring the 'less' compare
// Thanks to: https://github.com/nlohmann/fifo_map + https://github.com/nlohmann/json/issues/485#issuecomment-333652309
template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using json = nlohmann::basic_json<my_workaround_fifo_map>;

namespace glm
{
	void to_json(json& j, const glm::vec2& float2)
	{
		j.emplace("X", float2.x);
		j.emplace("Y", float2.y);
	}

	void to_json(json& j, const glm::vec3& float3)
	{
		j.emplace("X", float3.x);
		j.emplace("Y", float3.y);
		j.emplace("Z", float3.z);
	}

	void to_json(json& j, const glm::vec4& float4)
	{
		j.emplace("X", float4.x);
		j.emplace("Y", float4.y);
		j.emplace("Z", float4.z);
		j.emplace("W", float4.w);
	}

	void to_json(json& j, const glm::mat3& mat3)
	{
		j.emplace("[0][0]", mat3[0][0]);	j.emplace("[1][0]", mat3[1][0]);	j.emplace("[2][0]", mat3[2][0]);
		j.emplace("[0][1]", mat3[0][1]);	j.emplace("[1][1]", mat3[1][1]);	j.emplace("[2][1]", mat3[2][1]);
		j.emplace("[0][2]", mat3[0][2]);	j.emplace("[1][2]", mat3[1][2]);	j.emplace("[2][2]", mat3[2][2]);
	}

	void to_json(json& j, const glm::mat4& mat4)
	{
		j.emplace("[0][0]", mat4[0][0]);	j.emplace("[1][0]", mat4[1][0]);	j.emplace("[2][0]", mat4[2][0]);	j.emplace("[3][0]", mat4[3][0]);
		j.emplace("[0][1]", mat4[0][1]);	j.emplace("[1][1]", mat4[1][1]);	j.emplace("[2][1]", mat4[2][1]);	j.emplace("[3][1]", mat4[3][1]);
		j.emplace("[0][2]", mat4[0][2]);	j.emplace("[1][2]", mat4[1][2]);	j.emplace("[2][2]", mat4[2][2]);	j.emplace("[3][2]", mat4[3][2]);
		j.emplace("[0][3]", mat4[0][3]);	j.emplace("[1][3]", mat4[1][3]);	j.emplace("[2][3]", mat4[2][3]);	j.emplace("[3][3]", mat4[3][3]);
	}

	void to_json(json& j, const glm::ivec2& int2)
	{
		j.emplace("X", int2.x);
		j.emplace("Y", int2.y);
	}

	void to_json(json& j, const glm::ivec3& int3)
	{
		j.emplace("X", int3.x);
		j.emplace("Y", int3.y);
		j.emplace("Z", int3.z);
	}

	void to_json(json& j, const glm::ivec4& int4)
	{
		j.emplace("X", int4.x);
		j.emplace("Y", int4.y);
		j.emplace("Z", int4.z);
		j.emplace("W", int4.w);
	}

	void to_json(json& j, const std::vector<glm::vec2>& float2Vec)
	{
		for (const glm::vec2& vec2Val : float2Vec)
		{
			j.push_back(vec2Val);
		}
	}
	
	void to_json(json& j, const std::vector<glm::vec3>& float3Vec)
	{
		for (const glm::vec3& vec3Val : float3Vec)
		{
			j.push_back(vec3Val);
		}
	}

	void to_json(json& j, const std::vector<glm::vec4>& float4Vec)
	{
		for (const glm::vec4& vec4Val : float4Vec)
		{
			j.push_back(vec4Val);
		}
	}

	void to_json(json& j, const std::vector<glm::mat3>& mat3Vec)
	{
		for (const glm::mat3& mat3Val : mat3Vec)
		{
			j.push_back(mat3Val);
		}
	}

	void to_json(json& j, const std::vector<glm::mat4>& mat4Vec)
	{
		for (const glm::mat4& mat4Val : mat4Vec)
		{
			j.push_back(mat4Val);
		}
	}

	void to_json(json& j, const std::vector<glm::ivec2>& int2Vec)
	{
		for (const glm::ivec2& vec2Val : int2Vec)
		{
			j.push_back(vec2Val);
		}
	}

	void to_json(json& j, const std::vector<glm::ivec3>& int3Vec)
	{
		for (const glm::ivec3& vec3Val : int3Vec)
		{
			j.push_back(vec3Val);
		}
	}

	void to_json(json& j, const std::vector<glm::ivec4>& int4Vec)
	{
		for (const glm::ivec4& vec4Val : int4Vec)
		{
			j.push_back(vec4Val);
		}
	}

	void from_json(const json& j, glm::vec2& float2)
	{
		j.at("X").get_to(float2.x);
		j.at("Y").get_to(float2.y);
	}

	void from_json(const json& j, std::vector<glm::vec2>& float2Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			float2Vec.push_back(j[i]);
		}
	}

	void from_json(const json& j, glm::vec3& float3)
	{
		j.at("X").get_to(float3.x);
		j.at("Y").get_to(float3.y);
		j.at("Z").get_to(float3.z);
	}

	void from_json(const json& j, std::vector<glm::vec3>& float3Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			float3Vec.push_back(j[i]);
		}
	}

	void from_json(const json& j, glm::vec4& float4)
	{
		j.at("X").get_to(float4.x);
		j.at("Y").get_to(float4.y);
		j.at("Z").get_to(float4.z);
		j.at("W").get_to(float4.w);
	}

	void from_json(const json& j, std::vector<glm::vec4>& float4Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			float4Vec.push_back(j[i]);
		}
	}

	void from_json(const json& j, glm::mat3& mat3)
	{
		j.at("[0][0]").get_to(mat3[0][0]);	j.at("[1][0]").get_to(mat3[1][0]);	j.at("[2][0]").get_to(mat3[2][0]);
		j.at("[0][1]").get_to(mat3[0][1]);	j.at("[1][1]").get_to(mat3[1][1]);	j.at("[2][1]").get_to(mat3[2][1]);
		j.at("[0][2]").get_to(mat3[0][2]);	j.at("[1][2]").get_to(mat3[1][2]);	j.at("[2][2]").get_to(mat3[2][2]);
	}

	void from_json(const json& j, std::vector<glm::mat3>& mat3Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			mat3Vec.push_back(j[i]);
		}
	}

	void from_json(const json& j, glm::mat4& mat4)
	{
		j.at("[0][0]").get_to(mat4[0][0]);	j.at("[1][0]").get_to(mat4[1][0]);	j.at("[2][0]").get_to(mat4[2][0]);	j.at("[3][0]").get_to(mat4[3][0]);
		j.at("[0][1]").get_to(mat4[0][1]);	j.at("[1][1]").get_to(mat4[1][1]);	j.at("[2][1]").get_to(mat4[2][1]);	j.at("[3][1]").get_to(mat4[3][1]);
		j.at("[0][2]").get_to(mat4[0][2]);	j.at("[1][2]").get_to(mat4[1][2]);	j.at("[2][2]").get_to(mat4[2][2]);	j.at("[3][2]").get_to(mat4[3][2]);
		j.at("[0][3]").get_to(mat4[0][3]);	j.at("[1][3]").get_to(mat4[1][3]);	j.at("[2][3]").get_to(mat4[2][3]);	j.at("[3][3]").get_to(mat4[3][3]);
	}

	void from_json(const json& j, std::vector<glm::mat4>& mat4Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			mat4Vec.push_back(j[i]);
		}
	}

	void from_json(const json& j, glm::ivec2& int2)
	{
		j.at("X").get_to(int2.x);
		j.at("Y").get_to(int2.y);
	}

	void from_json(const json& j, std::vector<glm::ivec2>& int2Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			int2Vec.push_back(j[i]);
		}
	}

	void from_json(const json& j, glm::ivec3& int3)
	{
		j.at("X").get_to(int3.x);
		j.at("Y").get_to(int3.y);
		j.at("Z").get_to(int3.z);
	}

	void from_json(const json& j, std::vector<glm::ivec3>& int3Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			int3Vec.push_back(j[i]);
		}
	}

	void from_json(const json& j, glm::ivec4& int4)
	{
		j.at("X").get_to(int4.x);
		j.at("Y").get_to(int4.y);
		j.at("Z").get_to(int4.z);
		j.at("W").get_to(int4.w);
	}

	void from_json(const json& j, std::vector<glm::ivec4>& int4Vec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			int4Vec.push_back(j[i]);
		}
	}
}

namespace Foundation
{
	void to_json(json& j, const UUID& id)
	{
		uint64_t idAsInt = id;
		j.emplace("m_UUID", idAsInt);
	}

	void to_json(json& j, const std::vector<UUID>& idVec)
	{
		for (const UUID& id : idVec)
		{
			j.push_back(id);
		}
	}

	void to_json(json& j, const Colour& colour)
	{
		j.emplace("r", colour.r());
		j.emplace("g", colour.g());
		j.emplace("b", colour.b());
		j.emplace("a", colour.a());
	}

	void to_json(json& j, const std::vector<Colour>& colourVec)
	{
		for (const Colour& colour : colourVec)
		{
			j.push_back(colour);
		}
	}

	void to_json(json& j, const SharedPtr<Texture2D>& texture)
	{
		j.emplace("m_Filepath", texture->GetFilepath());
	}

	void to_json(json& j, const std::vector<SharedPtr<Texture2D>>& textureVec)
	{
		for (const SharedPtr<Texture2D>& texture : textureVec)
		{
			j.push_back(texture);
		}
	}

	void to_json(json& j, const SharedPtr<Model>& model)
	{
		j.emplace("m_Filepath", model->GetFilepath());
	}

	void to_json(json& j, const std::vector<SharedPtr<Model>>& modelVec)
	{
		for (const SharedPtr<Model>& pModel : modelVec)
		{
			j.push_back(pModel);
		}
	}

	void to_json(json& j, const Scene& scene)
	{
		j = json{};
		const std::vector<Object*>& pSceneObjects{ scene.GetObjects() };
		j["Level"] = json::object();
		j["Level"]["Name"] = scene.GetName();

		std::function<void(BaseObject* pBaseObject, const reflect::TypeDescriptor_Struct::Member& member, json& j)> serializeMember = [&](BaseObject* pBaseObject, const reflect::TypeDescriptor_Struct::Member& member, json& j)
		{
			const std::string& memberName = member.name;
			void* memberPtr = (void*)((char*)pBaseObject + member.offset);
			const reflect::FieldType& fieldType = member.type->getFieldType();

			switch (fieldType)
			{
				case reflect::FieldType::Float:
				{
					float floatValue = *((float*)memberPtr);
					j = floatValue;
					break;
				}
				case reflect::FieldType::Float2:
				{
					glm::vec2 float2Value = *((glm::vec2*)memberPtr);
					j = float2Value;
					break;
				}
				case reflect::FieldType::Float3:
				{
					glm::vec3 float3Value = *((glm::vec3*)memberPtr);
					j = float3Value;
					break;
				}
				case reflect::FieldType::Float4:
				{
					glm::vec4 float4Value = *((glm::vec4*)memberPtr);
					j = float4Value;
					break;
				}
				case reflect::FieldType::Mat3:
				{
					glm::mat3 mat3Value = *((glm::mat3*)memberPtr);
					j = mat3Value;
					break;
				}
				case reflect::FieldType::Mat4:
				{
					glm::mat4 mat4Value = *((glm::mat4*)memberPtr);
					j = mat4Value;
					break;
				}
				case reflect::FieldType::Int:
				{
					int intValue = *((int*)memberPtr);
					j = intValue;
					break;
				}
				case reflect::FieldType::Int2:
				{
					glm::ivec2 int2Value = *((glm::ivec2*)memberPtr);
					j = int2Value;
					break;
				}
				case reflect::FieldType::Int3:
				{
					glm::ivec3 int3Value = *((glm::ivec3*)memberPtr);
					j = int3Value;
					break;
				}
				case reflect::FieldType::Int4:
				{
					glm::ivec4 int4Value = *((glm::ivec4*)memberPtr);
					j = int4Value;
					break;
				}
				case reflect::FieldType::Bool:
				{
					bool boolValue = *((bool*)memberPtr);
					j = boolValue;
					break;
				}
				case reflect::FieldType::String:
				{
					std::string stringValue = *((std::string*)memberPtr);
					j = stringValue;
					break;
				}
				case reflect::FieldType::Colour:
				{
					Colour colourValue = *(Colour*)memberPtr;
					j = colourValue;
					break;
				}
				case reflect::FieldType::Texture:
				{
					SharedPtr<Texture2D> pSharedTextureValue = *((SharedPtr<Texture2D>*)memberPtr);
					if (pSharedTextureValue)
					{
						j = pSharedTextureValue;
					}

					break;
				}
				case reflect::FieldType::Model:
				{
					SharedPtr<Model> pSharedModel = *((SharedPtr<Model>*)memberPtr);
					if (pSharedModel)
					{
						j = pSharedModel;
					}
					break;
				}
				case reflect::FieldType::Class:
				{
					if (BaseObject* pClass = ((BaseObject*)memberPtr))
					{
						const reflect::TypeDescriptor_Struct& classTypeDescriptor{ pClass->GetTypeDescription() };

						json classArray = json::array();
						for (const reflect::TypeDescriptor_Struct::Member& classMember : classTypeDescriptor.members)
						{
							serializeMember(pClass, classMember, classArray);
						}
						j = classArray;
					}
					break;
				}
				case reflect::FieldType::Vector:
				{
					reflect::TypeDescriptor_StdVector* vectorType = (reflect::TypeDescriptor_StdVector*)member.type;
					reflect::TypeDescriptor* vectorItemTypeDescriptor = vectorType->itemType;
					const reflect::FieldType vectorItemFieldType = vectorType->itemType->getFieldType();

					switch (vectorItemFieldType)
					{
						case reflect::FieldType::Float:
						{
							std::vector<float> floatVector = *(std::vector<float>*)memberPtr;
							j = floatVector;
							break;
						}
						case reflect::FieldType::Float2:
						{
							std::vector<glm::vec2> float2Vector = *(std::vector<glm::vec2>*)memberPtr;
							j = float2Vector;
							break;
						}
						case reflect::FieldType::Float3:
						{
							std::vector<glm::vec3> float3Vector = *(std::vector<glm::vec3>*)memberPtr;
							j = float3Vector;
							break;
						}
						case reflect::FieldType::Float4:
						{
							std::vector<glm::vec4> float4Vector = *(std::vector<glm::vec4>*)memberPtr;
							j = float4Vector;
							break;
						}
						case reflect::FieldType::Mat3:
						{
							std::vector<glm::mat3> mat3Vec = *(std::vector<glm::mat3>*)memberPtr;
							j = mat3Vec;
							break;
						}
						case reflect::FieldType::Mat4:
						{
							std::vector<glm::mat4> mat4Vec = *(std::vector<glm::mat4>*)memberPtr;
							j = mat4Vec;
							break;
						}
						case reflect::FieldType::Int:
						{
							std::vector<int> intVec = *(std::vector<int>*)memberPtr;
							j = intVec;
							break;
						}
						case reflect::FieldType::Int2:
						{
							std::vector<glm::ivec2> int2Vec = *(std::vector<glm::ivec2>*)memberPtr;
							j = int2Vec;
							break;
						}
						case reflect::FieldType::Int3:
						{
							std::vector<glm::ivec3> int3Vec = *(std::vector<glm::ivec3>*)memberPtr;
							j = int3Vec;
							break;
						}
						case reflect::FieldType::Int4:
						{
							std::vector<glm::ivec4> int4Vec = *(std::vector<glm::ivec4>*)memberPtr;
							j = int4Vec;
							break;
						}
						case reflect::FieldType::Bool:
						{
							std::vector<bool> boolVec = *(std::vector<bool>*)memberPtr;
							j = boolVec;
							break;
						}
						case reflect::FieldType::String:
						{
							std::vector<std::string> stringVec = *(std::vector<std::string>*)memberPtr;
							j = stringVec;
							break;
						}
						case reflect::FieldType::Colour:
						{
							std::vector<Colour> colourVec = *(std::vector<Colour>*)memberPtr;
							j = colourVec;
							break;
						}
						case reflect::FieldType::Texture:
						{
							std::vector<Foundation::SharedPtr<Foundation::Texture2D>> textureVec = *(std::vector<Foundation::SharedPtr<Foundation::Texture2D>>*)memberPtr;
							j = textureVec;
							break;
						}
						case reflect::FieldType::Model:
						{
							std::vector<Foundation::SharedPtr<Foundation::Model>> modelVec = *(std::vector<Foundation::SharedPtr<Foundation::Model>>*)memberPtr;
							j = modelVec;
							break;
						}
						case reflect::FieldType::Class:
						{
							std::vector<BaseObject*> classVector = *(std::vector<BaseObject*>*)memberPtr;
							for(BaseObject* pClassVecObject : classVector)
							{
								const reflect::TypeDescriptor_Struct& classVectorTypeDescriptor{ pClassVecObject->GetTypeDescription() };
								const std::string& classVectorObjectName{ classVectorTypeDescriptor.getFullName() };

								for (const reflect::TypeDescriptor_Struct::Member& classVectorMember : classVectorTypeDescriptor.members)
								{
									serializeMember(pClassVecObject, classVectorMember, j[classVectorObjectName][classVectorMember.name]);
								}
							}

							//j = classVectorObjectArray;
							break;
						}
						default:
						{
							FD_CORE_ASSERT(false, "Vector field type not supported!");
							break;
						}
					}

					break;
				}
				default:
				{
					FD_CORE_ASSERT(false, "Field type not implemented!");
					break;
				}
			}
		};
		
		for (size_t iO = 0; iO < pSceneObjects.size(); ++iO)
		{
			Object* pObject = pSceneObjects[iO];

			if (!pObject)
				continue;

			const reflect::TypeDescriptor_Struct& typeDescriptor{ pObject->GetTypeDescription() };
			const std::string& objectName{ pObject->GetComponent<TagComponent>()->m_Tag };
			BaseObject* pBaseObject = static_cast<BaseObject*>(pObject);

			for (const reflect::TypeDescriptor_Struct::Member& member : typeDescriptor.members)
			{
				serializeMember(pBaseObject, member, j["Level"]["Objects"][objectName][member.name]);
			}
		}
	}

	void from_json(const json& j, Colour& colour)
	{
		glm::vec4 newColour = glm::vec4(
			j.at("r").get<float>(),
			j.at("g").get<float>(),
			j.at("b").get<float>(),
			j.at("a").get<float>()
		);
		colour.SetColour(newColour);
	}

	void from_json(const json& j, std::vector<Colour>& colourVec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			colourVec.push_back(j[i]);
		}
	}

	void from_json(const json& j, SharedPtr<Texture2D>& texture)
	{
		const std::string filepath = j.at("m_Filepath").get<std::string>();
		texture = Texture2D::Create(filepath);
	}

	void from_json(const json& j, std::vector<SharedPtr<Texture2D>>& textureVec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			const std::string filepath = j[i].get<std::string>();
			SharedPtr<Texture2D> pTex = Texture2D::Create(filepath);
			textureVec.push_back(pTex);
		}
	}

	void from_json(const json& j, SharedPtr<Model>& pModel)
	{
		const std::string filepath = j.at("m_Filepath").get<std::string>();
		pModel = Model::Create(filepath);
	}

	void from_json(const json& j, std::vector<SharedPtr<Model>>& modelVec)
	{
		for (size_t i = 0; i < j.size(); ++i)
		{
			const std::string filepath = j[i].get<std::string>();
			SharedPtr<Model> pModel = Model::Create(filepath);
			modelVec.push_back(pModel);
		}
	}

	void from_json(const json& j, Scene& scene)
	{
		std::function<void(BaseObject* pBaseObject, const reflect::TypeDescriptor_Struct::Member& member, const json& j)> deserializeMember = [&](BaseObject* pBaseObject, const reflect::TypeDescriptor_Struct::Member& member, const json& j)
		{
			const std::string& memberName = member.name;
			void* memberPtr = (void*)((char*)pBaseObject + member.offset);
			const reflect::FieldType& fieldType = member.type->getFieldType();
			
			switch (fieldType)
			{
				case reflect::FieldType::Float:
				{
					*(float*)memberPtr = j.get<float>();
					break;
				}
				case reflect::FieldType::Float2:
				{
					*(glm::vec2*)memberPtr = j.get<glm::vec2>();
					break;
				}
				case reflect::FieldType::Float3:
				{
					*(glm::vec3*)memberPtr = j.get<glm::vec3>();
					break;
				}
				case reflect::FieldType::Float4:
				{
					*(glm::vec4*)memberPtr = j.get<glm::vec4>();
					break;
				}
				case reflect::FieldType::Mat3:
				{
					*(glm::mat3*)memberPtr = j.get<glm::mat3>();
					break;
				}
				case reflect::FieldType::Mat4:
				{
					*(glm::mat4*)memberPtr = j.get<glm::mat4>();
					break;
				}
				case reflect::FieldType::Int:
				{
					*(int64_t*)memberPtr = j.get<int64_t>();
					break;
				}
				case reflect::FieldType::Int2:
				{
					*(glm::ivec2*)memberPtr = j.get<glm::ivec2>();
					break;
				}
				case reflect::FieldType::Int3:
				{
					*(glm::ivec3*)memberPtr = j.get<glm::ivec3>();
					break;
				}
				case reflect::FieldType::Int4:
				{
					*(glm::ivec4*)memberPtr = j.get<glm::ivec4>();
					break;
				}
				case reflect::FieldType::Bool:
				{
					*(bool*)memberPtr = j.get<bool>();
					break;
				}
				case reflect::FieldType::String:
				{
					*(std::string*)memberPtr = j.get<std::string>();
					break;
				}
				case reflect::FieldType::Colour:
				{
					*(Colour*)memberPtr = j.get<Colour>();
					break;
				}
				case reflect::FieldType::Texture:
				{
					if (!j.is_null())
					{
						*(SharedPtr<Texture2D>*)memberPtr = j.get<SharedPtr<Texture2D>>();
					}
					break;
				}
				case reflect::FieldType::Model:
				{
					if (!j.is_null())
					{
						*(SharedPtr<Model>*)memberPtr = j.get<SharedPtr<Model>>();
					}
					break;
				}
				case reflect::FieldType::Class:
				{
					if (BaseObject* pClass = ((BaseObject*)memberPtr))
					{
						pClass->SetOwner(pBaseObject);

						const reflect::TypeDescriptor_Struct& classTypeDescriptor{ pClass->GetTypeDescription() };
						const std::string& classTypeName{ classTypeDescriptor.getFullName() };
						for (const reflect::TypeDescriptor_Struct::Member& classMember : classTypeDescriptor.members)
						{
							deserializeMember(pClass, classMember, j);
						}
					}
					break;
				}
				case reflect::FieldType::Vector:
				{
					reflect::TypeDescriptor_StdVector* vectorType = (reflect::TypeDescriptor_StdVector*)member.type;
					reflect::TypeDescriptor* vectorItemTypeDescriptor = vectorType->itemType;
					const reflect::FieldType vectorItemFieldType = vectorType->itemType->getFieldType();

					switch (vectorItemFieldType)
					{
						case reflect::FieldType::Float:
						{
							*(std::vector<float>*)memberPtr = j.get<std::vector<float>>();
							break;
						}
						case reflect::FieldType::Float2:
						{
							*(std::vector<glm::vec2>*)memberPtr = j.get<std::vector<glm::vec2>>();
							break;
						}
						case reflect::FieldType::Float3:
						{
							*(std::vector<glm::vec3>*)memberPtr = j.get<std::vector<glm::vec3>>();
							break;
						}
						case reflect::FieldType::Float4:
						{
							*(std::vector<glm::vec4>*)memberPtr = j.get<std::vector<glm::vec4>>();
							break;
						}
						case reflect::FieldType::Mat3:
						{
							*(std::vector<glm::mat3>*)memberPtr = j.get<std::vector<glm::mat3>>();
							break;
						}
						case reflect::FieldType::Mat4:
						{
							*(std::vector<glm::mat4>*)memberPtr = j.get<std::vector<glm::mat4>>();
							break;
						}
						case reflect::FieldType::Int:
						{
							*(std::vector<int>*)memberPtr = j.get<std::vector<int>>();
							break;
						}
						case reflect::FieldType::Int2:
						{
							*(std::vector<glm::ivec2>*)memberPtr = j.get<std::vector<glm::ivec2>>();
							break;
						}
						case reflect::FieldType::Int3:
						{
							*(std::vector<glm::ivec3>*)memberPtr = j.get<std::vector<glm::ivec3>>();
							break;
						}
						case reflect::FieldType::Int4:
						{
							*(std::vector<glm::ivec4>*)memberPtr = j.get<std::vector<glm::ivec4>>();
							break;
						}
						case reflect::FieldType::Bool:
						{
							*(std::vector<bool>*)memberPtr = j.get<std::vector<bool>>();
							break;
						}
						case reflect::FieldType::String:
						{
							*(std::vector<std::string>*)memberPtr = j.get<std::vector<std::string>>();
							break;
						}
						case reflect::FieldType::Colour:
						{
							*(std::vector<Colour>*)memberPtr = j.get<std::vector<Colour>>();
							break;
						}
						case reflect::FieldType::Texture:
						{
							*(std::vector<SharedPtr<Texture2D>>*)memberPtr = j.get<std::vector<SharedPtr<Texture2D>>>();
							break;
						}
						case reflect::FieldType::Model:
						{
							*(std::vector<SharedPtr<Model>>*)memberPtr = j.get<std::vector<SharedPtr<Model>>>();
							break;
						}
						case reflect::FieldType::Class:
						{							
							std::vector<BaseObject*>& deserializedBaseObjects = (*(std::vector<BaseObject*>*)memberPtr);
			
							// We need to clear this out because of any previous data that might have been added by default
							// We want to override any defaults with the new serialized data we have just found.
							deserializedBaseObjects.clear();

							for (json::const_iterator it = j.begin(); it != j.end(); ++it)
							{
								const json& deserializedClassJson = (*it);
								const std::string& key = it.key();

								if (void* pConstructedClassObject = reflect::ClassRegistry::Get().Construct(key))
								{
									if (BaseObject* pDeserializedClassObject = (BaseObject*)pConstructedClassObject)
									{
										pDeserializedClassObject->SetOwner(pBaseObject);

										// Deserialize all of the members of this new base object now
										const reflect::TypeDescriptor_Struct& deserializedClassTypeDescription = pDeserializedClassObject->GetTypeDescription();
										for (const reflect::TypeDescriptor_Struct::Member& deserializedClassMember : deserializedClassTypeDescription.members)
										{
											const std::string& deserializedClassMemberName = deserializedClassMember.name;
											const json& deserializedMemberJson = deserializedClassJson[deserializedClassMemberName];
											deserializeMember(pDeserializedClassObject, deserializedClassMember, deserializedMemberJson);
										}

										deserializedBaseObjects.push_back(pDeserializedClassObject);
									}
								}
							}

							break;
						}
						default:
						{
							FD_CORE_ASSERT(false, "Vector field type not supported!");
							break;
						}
					}

					break;
				}
				default:
				{
					FD_CORE_ASSERT(false, "Field type not implemented!");
					break;
				}
			}
		};

		const json& jsonObjectsArray = j["Level"]["Objects"];
		for (json::const_iterator it = jsonObjectsArray.begin(); it != jsonObjectsArray.end(); ++it)
		{
			const std::string& objectName = it.key();
			if (BaseObject* pBaseObject = scene.CreateObject(objectName))
			{
				const reflect::TypeDescriptor_Struct& typeDescriptor = pBaseObject->GetTypeDescription();

				for (const reflect::TypeDescriptor_Struct::Member& member : typeDescriptor.members)
				{
					deserializeMember(pBaseObject, member, j["Level"]["Objects"][objectName][member.name]);
				}
			}
		}
	}

	SceneSerializer::SceneSerializer(const SharedPtr<Scene>& pScene) :
		m_pLevel(pScene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		json j = *m_pLevel;

		std::ofstream outputFile{ filepath };
		if (outputFile.is_open())
		{
			outputFile << std::setw(4) << j;
			outputFile.close();
		}
		//m_pLevel->IterateObjects(func);
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		FD_CORE_ASSERT(false, "Not implemented yet!");
	}

	void SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream levelFile(filepath);
		json j;
		levelFile >> j;

		// TODO: Should we be creating the scene here?

		// TODO: Cleanup level?
		const std::string levelName = j["Level"]["Name"].get<std::string>();
		m_pLevel->m_Name = levelName;
		FD_CORE_LOG_TRACE("Deserializing  '{0}'", levelName);
		
		//SharedPtr<Scene> pJsonScene = CreateSharedPtr<Scene>();
		*m_pLevel = j;

		levelFile.close();
	}

	void SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		FD_CORE_ASSERT(false, "Not implemented yet!");
	}
}