#include "fdpch.h"
#include "Reflect.h"
#include "Foundation/Components/Component.h"
#include "Foundation/Core/Base.h"
#include "Foundation/Core/Types.h"
#include "Foundation/Core/UUID.h"
#include "Foundation/Renderer/Model.h"
#include "Foundation/Renderer/Texture.h"
#include "Foundation/SpaceInvadersTest/BulletObject.h"
#include <glm/glm.hpp>

namespace reflect {

	//--------------------------------------------------------
	// A type descriptor for UUID
	//--------------------------------------------------------

	struct TypeDescriptor_UUID : TypeDescriptor {
		TypeDescriptor_UUID() : TypeDescriptor{ "UUID", sizeof(Foundation::UUID), FieldType::Int } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "UUID{" << *(const Foundation::UUID*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::UUID>() {
		static TypeDescriptor_UUID typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Float3
	//--------------------------------------------------------

	struct TypeDescriptor_Float3 : TypeDescriptor {
		TypeDescriptor_Float3() : TypeDescriptor{ "Float3", sizeof(glm::vec3), FieldType::Float3 } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const glm::vec3* float3 = (const glm::vec3*)obj;
			std::cout << "Float3{" << float3->x << ", " << float3->y << ", " << float3->z << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<glm::vec3>() {
		static TypeDescriptor_Float3 typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Float4
	//--------------------------------------------------------

	struct TypeDescriptor_Float4 : TypeDescriptor {
		TypeDescriptor_Float4() : TypeDescriptor{ "Float4", sizeof(glm::vec4), FieldType::Float4 } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const glm::vec4* float4 = (const glm::vec4*)obj;
			std::cout << "Float4{" << float4->x << ", " << float4->y << ", " << float4->z << ", " << float4->w << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<glm::vec4>() {
		static TypeDescriptor_Float4 typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Colour
	//--------------------------------------------------------

	struct TypeDescriptor_Colour : TypeDescriptor {
		TypeDescriptor_Colour() : TypeDescriptor{ "Colour", sizeof(Foundation::Colour), FieldType::Colour } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const Foundation::Colour* pColour = (const Foundation::Colour*)obj;
			std::cout << "Colour{" << pColour->r() << ", " << pColour->g() << ", " << pColour->b() << ", " << pColour->a() << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::Colour>() {
		static TypeDescriptor_Colour typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Texture2D
	//--------------------------------------------------------

	struct TypeDescriptor_Texture : TypeDescriptor {
		TypeDescriptor_Texture() : TypeDescriptor{ "Texture", sizeof(Foundation::Texture2D), FieldType::Texture } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const Foundation::SharedPtr<Foundation::Texture2D>* pTexture = (const Foundation::SharedPtr<Foundation::Texture2D>*)obj;
			std::cout << "Texture{" << (*pTexture)->GetRendererID() << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::SharedPtr<Foundation::Texture2D>>() {
		static TypeDescriptor_Texture typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Model
	//--------------------------------------------------------

	struct TypeDescriptor_Model : TypeDescriptor {
		TypeDescriptor_Model() : TypeDescriptor{ "Model", sizeof(Foundation::Model), FieldType::Model } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const Foundation::SharedPtr<Foundation::Model>* pModel = (const Foundation::SharedPtr<Foundation::Model>*)obj;
			std::cout << "Model{" << (*pModel)->GetFilepath() << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::SharedPtr<Foundation::Model>>() {
		static TypeDescriptor_Model typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Component
	//--------------------------------------------------------

	struct TypeDescriptor_Component : TypeDescriptor_Struct {
		TypeDescriptor_Component() : TypeDescriptor_Struct{ "Component", sizeof(Foundation::Component), {} } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const Foundation::Component* pComponent = (const Foundation::Component*)obj; 
			std::cout << "Component{" << name << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::Component>() {
		static TypeDescriptor_Component typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Component Pointer
	//--------------------------------------------------------

	struct TypeDescriptor_ComponentPointer : TypeDescriptor_Struct {
		TypeDescriptor_ComponentPointer() : TypeDescriptor_Struct{ "Component*", sizeof(Foundation::Component), {} } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const Foundation::Component* pComponent = (const Foundation::Component*)obj;
			std::cout << "Component*{" << name << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::Component*>() {
		static TypeDescriptor_ComponentPointer typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for BaseObject Pointer
	//--------------------------------------------------------

	struct TypeDescriptor_BaseObjectPointer : TypeDescriptor_Struct {
		TypeDescriptor_BaseObjectPointer() : TypeDescriptor_Struct{ "BaseObject*", sizeof(Foundation::BaseObject*), {} } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const Foundation::BaseObject* pBaseObject = (const Foundation::BaseObject*)obj;
			std::cout << "BaseObject*{" << name << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::BaseObject*>() {
		static TypeDescriptor_BaseObjectPointer typeDesc;
		return &typeDesc;
	}

	struct TypeDescriptor_BulletObjectPointer : TypeDescriptor_Struct {
		TypeDescriptor_BulletObjectPointer() : TypeDescriptor_Struct{ "BulletObject*", sizeof(Foundation::BulletObject*), {} } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			const Foundation::BulletObject* pBulletObject = (const Foundation::BulletObject*)obj;
			std::cout << "BulletObject*{" << name << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<Foundation::BulletObject*>() {
		static TypeDescriptor_BulletObjectPointer typeDesc;
		return &typeDesc;
	}
}