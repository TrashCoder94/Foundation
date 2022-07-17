#include "fdpch.h"
#include "Foundation/Objects/BaseObject.h"

namespace Foundation
{
	BaseObject::BaseObject() :
		m_pOwner(nullptr)
	{}

	BaseObject::~BaseObject()
	{
		m_pOwner = nullptr;
	}

	void BaseObject::Copy(BaseObject* pSourceObject)
	{
		if (!pSourceObject)
		{
			FD_CORE_LOG_ERROR("Base Object tried to copy another base object which was null!");
			return;
		}

		const reflect::TypeDescriptor_Struct& sourceTypeDescriptor = pSourceObject->GetTypeDescription();
		const reflect::TypeDescriptor_Struct& destinationTypeDescriptor = GetTypeDescription();
		for (size_t iM = 0; iM < sourceTypeDescriptor.members.size(); ++iM)
		{
			const reflect::TypeDescriptor_Struct::Member& sourceMember = sourceTypeDescriptor.members[iM];
			const reflect::TypeDescriptor_Struct::Member& destinationMember = destinationTypeDescriptor.members[iM];

			void* sourceMemberPtr = (void*)((char*)pSourceObject + sourceMember.offset);
			void* destinationMemberPtr = (void*)((char*)this + destinationMember.offset);
			//void* newMemberPtr = malloc(sourceMember.size);
			if (destinationMemberPtr)
			{
				memcpy(destinationMemberPtr, sourceMemberPtr, sourceMember.size);
				//memcpy(newMemberPtr, sourceMemberPtr, sourceMember.size);
				//*(char*)destinationMemberPtr = *(char*)newMemberPtr;
				//memcpy(destinationMemberPtr, newMemberPtr, sourceMember.size);
				//destinationMemberPtr = newMemberPtr;
				//free(newMemberPtr);
			}
		}
	}
}