#include "Reflect.h"

namespace reflect {

	//--------------------------------------------------------
	// A type descriptor for int
	//--------------------------------------------------------

	struct TypeDescriptor_Int : TypeDescriptor {
		TypeDescriptor_Int() : TypeDescriptor{ "int", sizeof(int), FieldType::Int } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "int{" << *(const int*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<int>() {
		static TypeDescriptor_Int typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for std::string
	//--------------------------------------------------------

	struct TypeDescriptor_StdString : TypeDescriptor {
		TypeDescriptor_StdString() : TypeDescriptor{ "std::string", sizeof(std::string), FieldType::String } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "std::string{\"" << *(const std::string*)obj << "\"}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<std::string>() {
		static TypeDescriptor_StdString typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for float
	//--------------------------------------------------------

	struct TypeDescriptor_Float : TypeDescriptor {
		TypeDescriptor_Float() : TypeDescriptor{ "float", sizeof(float), FieldType::Float } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "float{" << *(const float*)obj << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<float>() {
		static TypeDescriptor_Float typeDesc;
		return &typeDesc;
	}

	//--------------------------------------------------------
	// A type descriptor for Bool
	//--------------------------------------------------------

	struct TypeDescriptor_Bool : TypeDescriptor {
		TypeDescriptor_Bool() : TypeDescriptor{ "Bool", sizeof(bool), FieldType::Bool } {
		}
		virtual void dump(const void* obj, int /* unused */) const override {
			std::cout << "Bool{" << ((bool*)obj ? "true" : "false") << "}";
		}
	};

	template <>
	TypeDescriptor* getPrimitiveDescriptor<bool>() {
		static TypeDescriptor_Bool typeDesc;
		return &typeDesc;
	}
}