#pragma once

#include <assert.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstddef>
#include <map>

namespace reflect {

void InitApplicationReflection();

//--------------------------------------------------------
// Base class of all type descriptors
//--------------------------------------------------------

enum class FieldType
{
	None = 0,
	Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	Bool, 
	String,
	Colour,
	Texture,
	Model,
	ParticleProperties,
	Class,
	Vector
};

struct TypeDescriptor {
    const char* name;
    size_t size;
	FieldType fieldType;

	TypeDescriptor(const char* name, size_t size, FieldType type) : name{ name }, size{ size }, fieldType{ type } {}
    virtual ~TypeDescriptor() {}
    virtual std::string getFullName() const { return name; }
	virtual FieldType getFieldType() const { return fieldType; }
	virtual std::string getFieldTypeAsString() const 
	{
		std::string fieldTypeString{""};
		switch (fieldType)
		{
			case FieldType::None:					fieldTypeString = "None";					break;
			case FieldType::Float:					fieldTypeString = "Float";					break;
			case FieldType::Float2:					fieldTypeString = "Float2";					break;
			case FieldType::Float3:					fieldTypeString = "Float3";					break;
			case FieldType::Float4:					fieldTypeString = "Float4";					break;
			case FieldType::Mat3:					fieldTypeString = "Mat3";					break;
			case FieldType::Mat4:					fieldTypeString = "Mat4";					break;
			case FieldType::Int:					fieldTypeString = "Int";					break;
			case FieldType::Int2:					fieldTypeString = "Int2";					break;
			case FieldType::Int3:					fieldTypeString = "Int3";					break;
			case FieldType::Int4:					fieldTypeString = "Int4";					break;
			case FieldType::Bool:					fieldTypeString = "Bool";					break;
			case FieldType::String:					fieldTypeString = "String";					break;
			case FieldType::Colour:					fieldTypeString = "Colour";					break;
			case FieldType::Texture:				fieldTypeString = "Texture";				break;
			case FieldType::Model:					fieldTypeString = "Model";					break;
			case FieldType::ParticleProperties:		fieldTypeString = "ParticleProperties";		break;
			case FieldType::Class:					fieldTypeString = "Class";					break;
			case FieldType::Vector:					fieldTypeString = "Vector";					break;
			default: break;
		}

		return fieldTypeString;
	}
    virtual void dump(const void* obj, int indentLevel = 0) const = 0;
};

//--------------------------------------------------------
// Finding type descriptors
//--------------------------------------------------------

// Declare the function template that handles primitive types such as int, std::string, etc.:
template <typename T>
TypeDescriptor* getPrimitiveDescriptor();

// A helper class to find TypeDescriptors in different ways:
struct DefaultResolver {
    template <typename T> static char func(decltype(&T::Reflection));
    template <typename T> static int func(...);
    template <typename T>
    struct IsReflected {
        enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
    };

    // This version is called if T has a static member named "Reflection":
    template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
    static TypeDescriptor* get() {
        return &T::Reflection;
    }

    // This version is called otherwise:
    template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
    static TypeDescriptor* get() {
        return getPrimitiveDescriptor<T>();
    }
};

// This is the primary class template for finding all TypeDescriptors:
template <typename T>
struct TypeResolver {
    static TypeDescriptor* get() {
        return DefaultResolver::get<T>();
    }
};

//--------------------------------------------------------
// Type descriptors for user-defined structs/classes
//--------------------------------------------------------

struct TypeDescriptor_Struct : TypeDescriptor {
    struct Member {
        const char* name;
        size_t offset;
		size_t size;
        TypeDescriptor* type;
    };

    std::vector<Member> members;

	TypeDescriptor_Struct() : TypeDescriptor{nullptr, 0, FieldType::Class} {
	}
    TypeDescriptor_Struct(void (*init)(TypeDescriptor_Struct*)) : TypeDescriptor{nullptr, 0, FieldType::Class } {
        init(this);
    }
    TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init) : TypeDescriptor{nullptr, 0, FieldType::Class}, members{init} {
    }
    virtual void dump(const void* obj, int indentLevel) const override {
        std::cout << name << " {" << std::endl;
        for (const Member& member : members) {
            std::cout << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
            member.type->dump((char*) obj + member.offset, indentLevel + 1);
            std::cout << std::endl;
        }
        std::cout << std::string(4 * indentLevel, ' ') << "}";
    }
};

#define REFLECT() \
    friend struct reflect::DefaultResolver; \
    static reflect::TypeDescriptor_Struct Reflection; \
    static void initReflection(reflect::TypeDescriptor_Struct*);

#define REFLECT_STRUCT_BEGIN(type) \
    reflect::TypeDescriptor_Struct type::Reflection{type::initReflection}; \
    void type::initReflection(reflect::TypeDescriptor_Struct* typeDesc) { \
        using T = type; \
        typeDesc->name = #type; \
        typeDesc->size = sizeof(T); \
        typeDesc->members = {

#define REFLECT_STRUCT_MEMBER(name) \
            {#name, offsetof(T, name), sizeof(name), reflect::TypeResolver<decltype(T::name)>::get()},

#define REFLECT_STRUCT_END() \
        }; \
    }

// ========================================================================================

template<typename T> void* Constructor() { return (void*)new T(); }
typedef void* (*Constructor_T)();
typedef std::map<std::string, Constructor_T> RegisteredClassMap;

class ClassRegistry
{
public:
	const RegisteredClassMap& GetClassMap() { return m_Classes; };

	template<typename T>
	void Register(const std::string& name)
	{
		if (Get().m_Classes.find(name) == Get().m_Classes.end())
		{
			std::cout << "Registering class " << name << " with ClassRegistry" << std::endl;
			Get().m_Classes.insert(std::make_pair(name, &reflect::Constructor<T>));
		}
	}

	void* Construct(const std::string name)
	{
		RegisteredClassMap::iterator it = Get().m_Classes.find(name);
		if (it == Get().m_Classes.end())
		{
			assert(false);
			return nullptr;
		}
		return it->second();
	}

	static ClassRegistry& Get() 
	{
		static ClassRegistry classRegistry = ClassRegistry();
		return classRegistry;
	}

private:
	RegisteredClassMap m_Classes;
};

#define FD_REFLECT() \
	friend class reflect::ClassRegistry;\
	friend struct reflect::DefaultResolver; \
    static reflect::TypeDescriptor_Struct Reflection; \
    static void initReflection(reflect::TypeDescriptor_Struct*);\
	public: virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const override;

#define FD_REFLECT_CLASS_BEGIN(type) \
    reflect::TypeDescriptor_Struct type::Reflection{type::initReflection}; \
    void type::initReflection(reflect::TypeDescriptor_Struct* typeDesc) { \
        using T = type; \
		reflect::ClassRegistry::Get().Register<type>(#type);\
        typeDesc->name = #type; \
        typeDesc->size = sizeof(T); \
		typeDesc->fieldType = reflect::FieldType::Class;\
        typeDesc->members = {

#define FD_REFLECT_CLASS_MEMBER(name) \
            {#name, offsetof(T, name), sizeof(name), reflect::TypeResolver<decltype(T::name)>::get()},
#define FD_REFLECT_PARENT_CLASS_MEMBER(parentType, name) \
            {#name, offsetof(parentType, name), sizeof(name), reflect::TypeResolver<decltype(parentType::name)>::get()},

#define FD_REFLECT_CLASS_END() \
        }; \
    }

#define FD_REFLECT_BEGIN(type) \
	const reflect::TypeDescriptor_Struct& type::GetTypeDescription() const\
	{\
		reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<type>::get();\
		reflect::TypeDescriptor_Struct* structDesc = static_cast<reflect::TypeDescriptor_Struct*>(typeDesc);\
		return *structDesc;\
	}\
	FD_REFLECT_CLASS_BEGIN(type)
#define FD_REFLECT_MEMBER(name) FD_REFLECT_CLASS_MEMBER(name)
#define FD_REFLECT_PARENT_MEMBER(type, name) FD_REFLECT_PARENT_CLASS_MEMBER(type, name)
#define FD_REFLECT_END() FD_REFLECT_CLASS_END()

enum class VariableFlags
{
	Edit,
	Hidden
};

#define FVARIABLE(VariableFlags)

// ========================================================================================

//--------------------------------------------------------
// Type descriptors for std::vector
//--------------------------------------------------------

struct TypeDescriptor_StdVector : TypeDescriptor {
    TypeDescriptor* itemType;
    size_t (*getSize)(const void*);
    const void* (*getItem)(const void*, size_t);

    template <typename ItemType>
    TypeDescriptor_StdVector(ItemType*, FieldType vectorFieldType = FieldType::Vector)
        : TypeDescriptor{"std::vector<>", sizeof(std::vector<ItemType>), vectorFieldType },
                         itemType{TypeResolver<ItemType>::get()} {
        getSize = [](const void* vecPtr) -> size_t {
            const auto& vec = *(const std::vector<ItemType>*) vecPtr;
            return vec.size();
        };
        getItem = [](const void* vecPtr, size_t index) -> const void* {
            const auto& vec = *(const std::vector<ItemType>*) vecPtr;
            return &vec[index];
        };
    }
    virtual std::string getFullName() const override {
        return std::string("std::vector<") + itemType->getFullName() + ">";
    }
    virtual void dump(const void* obj, int indentLevel) const override {
        size_t numItems = getSize(obj);
        std::cout << getFullName();
        if (numItems == 0) {
            std::cout << "{}";
        } else {
            std::cout << "{" << std::endl;
            for (size_t index = 0; index < numItems; index++) {
                std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
                itemType->dump(getItem(obj, index), indentLevel + 1);
                std::cout << std::endl;
            }
            std::cout << std::string(4 * indentLevel, ' ') << "}";
        }
    }
};

// Partially specialize TypeResolver<> for std::vectors:
template <typename T>
class TypeResolver<std::vector<T>> {
public:
    static TypeDescriptor* get() {
        static TypeDescriptor_StdVector typeDesc{(T*) nullptr};
        return &typeDesc;
    }
};



} // namespace reflect

using namespace reflect;