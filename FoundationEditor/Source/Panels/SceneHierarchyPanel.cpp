#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <filesystem>

#include <Foundation/Core/Input.h>
#include <Foundation/Core/KeyCodes.h>
#include <Foundation/Components/CameraComponent.h>
#include <Foundation/Components/IDComponent.h>
#include <Foundation/Components/ModelComponent.h>
#include <Foundation/Components/SpriteComponent.h>
#include <Foundation/Components/TagComponent.h>
#include <Foundation/Components/TransformComponent.h>
#include <Foundation/Objects/Object.h>
#include <Foundation/Renderer/Texture.h>
#include <Foundation/Renderer/Model.h>
#include <Foundation/Scene/Scene.h>
#include <Foundation/Utilities/PlatformUtilities.h>

namespace Foundation
{
	namespace SceneHierarchyHelper
	{
		static std::vector<std::string> componentClassesToHide
		{
			"Component",
			"IDComponent",
			"TagComponent",
			"TransformComponent"
		};
	}

	SceneHierarchyPanel::SceneHierarchyPanel() :
		m_pScene(nullptr),
		m_pCheckerboardTexture(Texture2D::Create("Assets/Textures/Checkerboard.png")),
		m_pSelectedObject()
	{
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const SharedPtr<Scene>& pScene) :
		m_pScene(nullptr)
	{
		SetScene(pScene);
	}

	void SceneHierarchyPanel::SetScene(const SharedPtr<Scene>& pScene)
	{
		m_pScene = pScene;
		m_pSelectedObject = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		{
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 128, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 100, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 100, 0, 255));
			if (ImGui::Button("Add Object"))
			{
				ImGui::OpenPopup("AddObject");
			}
			ImGui::PopStyleColor(3);

			ImGui::Separator();

			if (ImGui::BeginPopup("AddObject"))
			{
				const reflect::RegisteredClassMap& classMap = reflect::ClassRegistry::Get().GetClassMap();
				for (const std::pair<std::string, reflect::Constructor_T>& classData : classMap)
				{
					const std::string& className = classData.first;

					if (className.find("Object") != std::string::npos)
					{
						if (ImGui::MenuItem(className.c_str()))
						{
							if (void* pConstructedObject = reflect::ClassRegistry::Get().Construct(className))
							{
								if (Object* pNewObject = static_cast<Object*>(pConstructedObject))
								{
									m_pScene->AddObject(pNewObject, className);
									m_pSelectedObject = pNewObject;
									ImGui::CloseCurrentPopup();
								}
							}
						}
					}
				}

				ImGui::EndPopup();
			}

			auto func = [this](Object* pObject)
			{
				DrawObjectNode(pObject);
			};
			m_pScene->IterateObjects(func);

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			{
				m_pSelectedObject = {};
			}

			//// Right-click on a blank space.
			//if (ImGui::BeginPopupContextWindow(0, 1, false))
			//{
			//	if (ImGui::MenuItem("Create New Object"))
			//	{
			//		/*const reflect::RegisteredClassMap& classMap = reflect::ClassRegistry::Get().GetClassMap();
			//		for (const std::pair<std::string, reflect::Constructor_T>& classData : classMap)
			//		{
			//			const std::string& className = classData.first;

			//			if (className.find("Object") != std::string::npos)
			//			{
			//				if (ImGui::MenuItem(className.c_str()))
			//				{
			//					if (void* pConstructedComponent = reflect::ClassRegistry::Get().Construct(className))
			//					{
			//						if (Component* pNewComponent = static_cast<Component*>(pConstructedComponent))
			//						{
			//							if (pObject->HasComponent(pNewComponent))
			//							{
			//								FD_CORE_LOG_WARN("Object '{0}' already has a {1} so this component won't be added", pObject->GetComponent<TagComponent>()->m_Tag, className);
			//							}
			//							else
			//							{
			//								pObject->AddComponent(pNewComponent);
			//							}
			//							ImGui::CloseCurrentPopup();
			//						}
			//					}
			//				}
			//			}
			//		}*/
			//		m_pSelectedObject = m_pScene->CreateObject("New Object");
			//	}
			//	ImGui::EndPopup();
			//}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		{
			if (m_pSelectedObject)
			{
				DrawComponentProperties(m_pSelectedObject);
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawObjectNode(Object* pObject)
	{
		std::string& tag = pObject->GetComponent<TagComponent>()->m_Tag;

		ImGuiTreeNodeFlags flags = ((m_pSelectedObject == pObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)pObject, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_pSelectedObject = pObject;
			// TODO: Selection changed callback?
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Object"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		// Defer deletion incase we need to do something with the object before we destroy it.
		if (entityDeleted)
		{
			if (m_pSelectedObject == pObject)
				m_pSelectedObject = {};
			m_pScene->DestroyObject(pObject);
		}
	}

	void SceneHierarchyPanel::DrawComponentProperties(Object* pObject)
	{
		// Tag Component
		if (pObject->HasComponent<TagComponent>())
		{
			std::string& tag = pObject->GetComponent<TagComponent>()->m_Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		// Add Component Button
		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);

		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 128, 0, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 100, 0, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 100, 0, 255));
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}
		ImGui::PopStyleColor(3);

		if (ImGui::BeginPopup("AddComponent"))
		{
			// This will find anything named component in the class registry
			// You can add new components to the class registry by using the reflection macro: FD_REFLECT
			const reflect::RegisteredClassMap& classMap = reflect::ClassRegistry::Get().GetClassMap();
			for (const std::pair<std::string, reflect::Constructor_T>& classData : classMap)
			{
				const std::string& className = classData.first;
				
				if (className.find("Component") != std::string::npos)
				{
					bool bHideThisComponent = false;
					for (const std::string& componentNameToHide : SceneHierarchyHelper::componentClassesToHide)
					{
						if (componentNameToHide.find(className) != std::string::npos)
						{
							bHideThisComponent = true;
							break;
						}
					}

					if (bHideThisComponent)
					{
						continue;
					}
				
					if (ImGui::MenuItem(className.c_str()))
					{
						if (void* pConstructedComponent = reflect::ClassRegistry::Get().Construct(className))
						{
							if (Component* pNewComponent = static_cast<Component*>(pConstructedComponent))
							{
								if (pObject->HasComponent(pNewComponent))
								{
									FD_CORE_LOG_WARN("Object '{0}' already has a {1} so this component won't be added", pObject->GetComponent<TagComponent>()->m_Tag, className);
								}
								else
								{
									pObject->AddComponent(pNewComponent);
								}
								ImGui::CloseCurrentPopup();
							}
						}
					}
				}
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();
		ImGui::Separator();

		const reflect::TypeDescriptor_Struct& objectReflectedData = pObject->GetTypeDescription();
		IterateAllMembers(pObject, objectReflectedData, pObject->GetID());
	}

	void SceneHierarchyPanel::IterateAllMembers(BaseObject* pClassObject, const reflect::TypeDescriptor_Struct& reflectedData, const UUID& uniqueObjectID)
	{
		for (const reflect::TypeDescriptor_Struct::Member& member : reflectedData.members)
		{
			DrawMember(pClassObject, member, uniqueObjectID);
		}
	}

	void SceneHierarchyPanel::DrawMember(BaseObject* pClassObject, const reflect::TypeDescriptor_Struct::Member& member, const UUID& uniqueObjectID)
	{
		void* memberPtr = (void*)((char*)pClassObject + member.offset);

		const std::string& fieldTypeAsString = member.type->getFieldTypeAsString();
		const std::string& idAsString = std::to_string(uniqueObjectID);

		std::string variableImguiLabel = "##reflected";
		variableImguiLabel.append(member.type->getFieldTypeAsString());
		variableImguiLabel.append(member.name);
		variableImguiLabel.append(idAsString);
		const char* variableImguiLabelCStr = variableImguiLabel.c_str();

		ImGui::Text(member.name);
		ImGui::SameLine();
		switch (member.type->getFieldType())
		{
			case reflect::FieldType::None:
			{
				FD_CORE_ASSERT(false, "Field type not implemented in ThirdParty/Reflection/Reflect.cpp");
				break;
			}
			case reflect::FieldType::Float:
			{
				float floatValue = *((float*)memberPtr);
				if (ImGui::DragFloat(variableImguiLabelCStr, &floatValue))
				{
					*((float*)memberPtr) = floatValue;
				}

				break;
			}
			case reflect::FieldType::Float2:
			{
				/*glm::vec2 float2Value = *((glm::vec2*)memberPtr);
				DrawVec2Control(member.name, float2Value);
				*((glm::vec2*)memberPtr) = float2Value;*/
				FD_CORE_ASSERT(false, "glm::vec2 not implemented in ThirdParty/Reflection/Reflect.cpp");
				break;
			}
			case reflect::FieldType::Float3:
			{
				glm::vec3 float3Value = *((glm::vec3*)memberPtr);
				DrawVec3Control(member.name, float3Value);
				*((glm::vec3*)memberPtr) = float3Value;

				break;
			}
			case reflect::FieldType::Float4:
			{
				glm::vec4 float4Value = *((glm::vec4*)memberPtr);
				DrawVec4Control(member.name, float4Value);
				*((glm::vec4*)memberPtr) = float4Value;
				break;
			}
			case reflect::FieldType::Mat3:
			{
				//glm::mat3 mat3Value = *((glm::mat3*)memberPtr);
				//DrawMat3Control(member.name, mat3Value);
				//*((glm::mat3*)memberPtr) = mat3Value;
				FD_CORE_ASSERT(false, "glm::mat3 not implemented in ThirdParty/Reflection/Reflect.cpp");
				break;
			}
			case reflect::FieldType::Mat4:
			{
				//glm::mat4 mat4Value = *((glm::mat4*)memberPtr);
				//DrawMat4Control(member.name, mat4Value);
				//*((glm::mat4*)memberPtr) = mat4Value;
				FD_CORE_ASSERT(false, "glm::mat4 not implemented in ThirdParty/Reflection/Reflect.cpp");
				break;
			}
			case reflect::FieldType::Int:
			{
				int intValue = *((int*)memberPtr);
				if (ImGui::DragInt(variableImguiLabelCStr, &intValue))
				{
					*((int*)memberPtr) = intValue;
				}
				break;
			}
			case reflect::FieldType::Int2:
			{
				/*glm::ivec2 int2Value = *((glm::ivec2*)memberPtr);
				DrawiVec2Control(member.name, int2Value);
				*((glm::ivec2*)memberPtr) = int2Value;*/
				FD_CORE_ASSERT(false, "glm::ivec2 not implemented in ThirdParty/Reflection/Reflect.cpp");
				break;
			}
			case reflect::FieldType::Int3:
			{
				/*glm::ivec3 int3Value = *((glm::ivec3*)memberPtr);
				DrawiVec3Control(member.name, int3Value);
				*((glm::ivec3*)memberPtr) = int2Value;*/
				FD_CORE_ASSERT(false, "glm::ivec3 not implemented in ThirdParty/Reflection/Reflect.cpp");
				break;
			}
			case reflect::FieldType::Int4:
			{
				/*glm::ivec4 int4Value = *((glm::ivec4*)memberPtr);
				DrawiVec4Control(member.name, int4Value);
				*((glm::ivec4*)memberPtr) = int4Value;*/
				FD_CORE_ASSERT(false, "glm::ivec4 not implemented in ThirdParty/Reflection/Reflect.cpp");
				break;
			}
			case reflect::FieldType::Bool:
			{
				bool boolValue = *((bool*)memberPtr);
				ImGui::Checkbox(variableImguiLabelCStr, &boolValue);
				*((bool*)memberPtr) = boolValue;
				break;
			}
			case reflect::FieldType::String:
			{
				std::string stringValue = *((std::string*)memberPtr);
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), stringValue.c_str());

				if (ImGui::InputText(variableImguiLabelCStr, buffer, sizeof(buffer)))
				{
					*((std::string*)memberPtr) = std::string(buffer);
				}
				break;
			}
			case reflect::FieldType::Colour:
			{
				Colour ColourValue = *((Colour*)memberPtr);
				glm::vec4& colour = ColourValue.GetColour();
				ImGui::ColorEdit4("Colour", glm::value_ptr(colour));
				*((Colour*)memberPtr) = ColourValue;
				break;
			}
			case reflect::FieldType::Texture:
			{
				SharedPtr<Texture2D>* pSharedTextureValue = ((SharedPtr<Texture2D>*)memberPtr);
				ImTextureID texId = (ImTextureID)(UINT_PTR)((*pSharedTextureValue) ? (*pSharedTextureValue)->GetRendererID() : m_pCheckerboardTexture->GetRendererID());
				if (ImGui::ImageButton(texId, { 32, 32 }))
				{
					std::optional<std::string> filepath = FileDialog::OpenFile("Image Files (*.gif;*.jpg;*.jpe;*.png;*.bmp;*.dib;*.tif;*.wmf;*.ras;*.eps;*.pcx;*.pcd;*.tga)\0");
					if (filepath)
					{
						SharedPtr<Texture2D> pSharedTexture = Texture2D::Create(*filepath);
						*pSharedTextureValue = pSharedTexture;
						*((SharedPtr<Texture2D>*)memberPtr) = *pSharedTextureValue;
					}
				}

				if ((*pSharedTextureValue))
				{
					if (ImGui::BeginPopupContextWindow(0, 1, false))
					{
						if (ImGui::MenuItem("Clear Texture"))
						{
							(*pSharedTextureValue).reset();
							(*pSharedTextureValue) = nullptr;
							*((SharedPtr<Texture2D>*)memberPtr) = *pSharedTextureValue;
						}

						ImGui::EndPopup();
					}
				}

				break;
			}
			case reflect::FieldType::Model:
			{
				SharedPtr<Model>* pSharedModelValue = ((SharedPtr<Model>*)memberPtr);
				ImTextureID texId = (ImTextureID)(UINT_PTR)m_pCheckerboardTexture->GetRendererID();
				if (ImGui::ImageButton(texId, { 32, 32 }))
				{
					std::optional<std::string> filepath = FileDialog::OpenFile("Model Files (*.3D;*.3DS; *.3MF; *.AC; *.AC3D; *.ACC; *.AMJ; *.ASE; *.ASK; *.B3D; *.BLEND; *.BVH; *.CMS; *.COB; *.DAE; *.DXF; *.ENFF; *.FBX; *.glTF1.0; *.glTF2.0; *.HMB;* .IFC;*.STEP;*.IRR;*.IRRMESH;*.LWO;*.LWS;*.LXO;*.M3D;*.MD2;*.MD3;*.MD5;*.MDC;*.MDL;*.MESH;*.MESH.XML;*.MOT;*.MS3D;*.NDO;*.NFF;*.OBJ;*.OFF;*.OGEX;*.PLY;*.PMX;*.PRJ;*.Q3O;*.Q3S;*.RAW;*.SCN;*.SIB;*.SMD;*.STP;*.STL;*.TER;*.UC;*.VTA;*.X;*.X3D;*.XGL;*.ZGL;\0");
					if (filepath)
					{
						SharedPtr<Model> pSharedModel = Model::Create(*filepath);
						*pSharedModelValue = pSharedModel;
						*((SharedPtr<Model>*)memberPtr) = *pSharedModelValue;
					}
				}

				if ((*pSharedModelValue))
				{
					if (ImGui::BeginPopupContextWindow(0, 1, false))
					{
						if (ImGui::MenuItem("Clear Model"))
						{
							(*pSharedModelValue).reset();
							(*pSharedModelValue) = nullptr;
							*((SharedPtr<Model>*)memberPtr) = *pSharedModelValue;
						}

						ImGui::EndPopup();
					}
				}

				break;
			}
			case reflect::FieldType::Class:
			{
				// Found a class, so iterate over each of the members again.
				// This will go down each class found recursively.
				reflect::TypeDescriptor_Struct* classType = (reflect::TypeDescriptor_Struct*)member.type;
				IterateAllMembers(pClassObject, *classType, uniqueObjectID);
				break;
			}
			case reflect::FieldType::Vector:
			{
				reflect::TypeDescriptor_StdVector* vectorType = (reflect::TypeDescriptor_StdVector*)member.type;
				reflect::TypeDescriptor* vectorItemTypeDescriptor = vectorType->itemType;
				const reflect::FieldType vectorItemFieldType = vectorType->itemType->getFieldType();
				size_t vectorSize = 0;
				std::string vectorDescription = "";

				switch (vectorItemFieldType)
				{
					case reflect::FieldType::None:
					{
						FD_CORE_ASSERT(false, "std::vector has no type somehow?");
						break;
					}
					case reflect::FieldType::Float:
					{
						bool vectorValueUpdated = false;
						std::vector<float> floatVector = *(std::vector<float>*)memberPtr;
						vectorSize = floatVector.size();
						vectorDescription = std::to_string(vectorSize) + " Elements";

						ImGui::Text(vectorDescription.c_str());

						for (size_t iV = 0; iV < vectorSize; ++iV)
						{
							float& floatValue = floatVector[iV];
							if (ImGui::DragFloat(variableImguiLabelCStr, &floatValue))
							{
								//*((float*)memberPtr) = floatValue;
								vectorValueUpdated = true;
							}
						}

						if (vectorValueUpdated)
						{
							*(std::vector<float>*)memberPtr = floatVector;
						}

						break;
					}
					case reflect::FieldType::Float2:
					{
						std::vector<glm::vec2> float2Vector = *(std::vector<glm::vec2>*)memberPtr;
						break;
					}
					case reflect::FieldType::Float3:
					{
						std::vector<glm::vec3> float3Vector = *(std::vector<glm::vec3>*)memberPtr;
						break;
					}
					case reflect::FieldType::Float4:
					{
						std::vector<glm::vec4> float4Vector = *(std::vector<glm::vec4>*)memberPtr;
						break;
					}
					case reflect::FieldType::Mat3:
					{
						std::vector<glm::mat3> mat3Vector = *(std::vector<glm::mat3>*)memberPtr;
						break;
					}
					case reflect::FieldType::Mat4:
					{
						std::vector<glm::mat4> mat4Vector = *(std::vector<glm::mat4>*)memberPtr;
						break;
					}
					case reflect::FieldType::Int:
					{
						std::vector<int> intVector = *(std::vector<int>*)memberPtr;
						break;
					}
					case reflect::FieldType::Int2:
					{
						std::vector<glm::ivec2> int2Vector = *(std::vector<glm::ivec2>*)memberPtr;
						break;
					}
					case reflect::FieldType::Int3:
					{
						std::vector<glm::ivec3> int3Vector = *(std::vector<glm::ivec3>*)memberPtr;
						break;
					}
					case reflect::FieldType::Int4:
					{
						std::vector<glm::ivec4> int4Vector = *(std::vector<glm::ivec4>*)memberPtr;
						break;
					}
					case reflect::FieldType::Bool:
					{
						std::vector<bool> boolVector = *(std::vector<bool>*)memberPtr;
						break;
					}
					case reflect::FieldType::String:
					{
						std::vector<std::string> stringVector = *(std::vector<std::string>*)memberPtr;
						break;
					}
					case reflect::FieldType::Colour:
					{
						std::vector<Colour> stringVector = *(std::vector<Colour>*)memberPtr;
						break;
					}
					case reflect::FieldType::Texture:
					{
						std::vector<SharedPtr<Texture2D>> textureVector = *(std::vector<SharedPtr<Texture2D>>*)memberPtr;
						break;
					}
					case reflect::FieldType::Model:
					{
						std::vector<SharedPtr<Model>> modelVector = *(std::vector<SharedPtr<Model>>*)memberPtr;
						break;
					}
					case reflect::FieldType::Class:
					{
						std::vector<BaseObject*> classVector = *(std::vector<BaseObject*>*)memberPtr;
						
						vectorSize = classVector.size();
						vectorDescription = std::to_string(vectorSize) + " Elements";

						ImGui::Text(vectorDescription.c_str());

						for (size_t iV = 0; iV < vectorSize; ++iV)
						{
							BaseObject* pBaseObject = classVector[iV];

							// Component specific editor UI
							if(pBaseObject->Is<Component>())
							{
								Component* pComponent = static_cast<Component*>(pBaseObject);
								if (pComponent->Is<IDComponent>() || pComponent->Is<TagComponent>())
									continue;

								const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
								const bool canBeRemoved = !pComponent->Is<TransformComponent>();

								ImVec2 regionContentAvailable = ImGui::GetContentRegionAvail();
								ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
								float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
								bool open = ImGui::TreeNodeEx((void*)pComponent, treeNodeFlags, pComponent->GetTypeDescription().getFullName().c_str());
								ImGui::PopStyleVar();
								bool removingComponent = false;

								if (canBeRemoved)
								{
									ImGui::SameLine(regionContentAvailable.x - lineHeight * 0.5f);
									if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
									{
										ImGui::OpenPopup("ComponentSettings");
									}

									if (ImGui::BeginPopup("ComponentSettings"))
									{
										if (ImGui::MenuItem("Remove Component"))
										{
											removingComponent = true;
										}
										ImGui::EndPopup();
									}
								}

								if (open)
								{
									IterateAllMembers(pBaseObject, pBaseObject->GetTypeDescription(), uniqueObjectID);
									ImGui::TreePop();
								}

								if (removingComponent)
								{
									if (Object* pOwner = static_cast<Object*>(pComponent->GetOwner()))
									{
										if (pOwner->RemoveComponent(pComponent))
										{
											--iV;
										}
									}
								}
							}

							ImGui::Separator();
						}
						
						break;
					}
					case reflect::FieldType::Vector:
					{
						FD_CORE_ASSERT(false, "std::vector<std::vector<T>> is not supported");
						break;
					}
				}

				break;
			}
		}
	}

	void SceneHierarchyPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue /*= 0.0f*/, float columnWidth /*= 100.0f*/)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFont* boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		{
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			// X
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.25f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// Y
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// Z
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
			{
				values.z = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f);
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();
		}
		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFont* boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		{
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			// X
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.25f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// Y
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// Z
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
			{
				values.z = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// W
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("W", buttonSize))
			{
				values.w = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##W", &values.w, 0.1f);
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();
		}
		ImGui::Columns(1);

		ImGui::PopID();
	}
}