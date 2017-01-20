// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Boilerplate C++ definitions for a single module.
	This is automatically generated by UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "Private/UObjectPluginPrivatePCH.h"
#include "UObjectPlugin.generated.dep.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCode1UObjectPlugin() {}
class UScriptStruct* FMyPluginStruct::StaticStruct()
{
	extern UOBJECTPLUGIN_API class UPackage* Z_Construct_UPackage__Script_UObjectPlugin();
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern UOBJECTPLUGIN_API class UScriptStruct* Z_Construct_UScriptStruct_FMyPluginStruct();
		extern UOBJECTPLUGIN_API uint32 Get_Z_Construct_UScriptStruct_FMyPluginStruct_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FMyPluginStruct, Z_Construct_UPackage__Script_UObjectPlugin(), TEXT("MyPluginStruct"), sizeof(FMyPluginStruct), Get_Z_Construct_UScriptStruct_FMyPluginStruct_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FMyPluginStruct(FMyPluginStruct::StaticStruct, TEXT("/Script/UObjectPlugin"), TEXT("MyPluginStruct"), false, nullptr, nullptr);
static struct FScriptStruct_UObjectPlugin_StaticRegisterNativesFMyPluginStruct
{
	FScriptStruct_UObjectPlugin_StaticRegisterNativesFMyPluginStruct()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("MyPluginStruct")),new UScriptStruct::TCppStructOps<FMyPluginStruct>);
	}
} ScriptStruct_UObjectPlugin_StaticRegisterNativesFMyPluginStruct;
	void UMyPluginObject::StaticRegisterNativesUMyPluginObject()
	{
	}
	IMPLEMENT_CLASS(UMyPluginObject, 2961236640);
#if USE_COMPILED_IN_NATIVES
// Cross Module References
	COREUOBJECT_API class UClass* Z_Construct_UClass_UObject();

	UOBJECTPLUGIN_API class UScriptStruct* Z_Construct_UScriptStruct_FMyPluginStruct();
	UOBJECTPLUGIN_API class UClass* Z_Construct_UClass_UMyPluginObject_NoRegister();
	UOBJECTPLUGIN_API class UClass* Z_Construct_UClass_UMyPluginObject();
	UOBJECTPLUGIN_API class UPackage* Z_Construct_UPackage__Script_UObjectPlugin();
	UScriptStruct* Z_Construct_UScriptStruct_FMyPluginStruct()
	{
		UPackage* Outer = Z_Construct_UPackage__Script_UObjectPlugin();
		extern uint32 Get_Z_Construct_UScriptStruct_FMyPluginStruct_CRC();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("MyPluginStruct"), sizeof(FMyPluginStruct), Get_Z_Construct_UScriptStruct_FMyPluginStruct_CRC(), false);
		if (!ReturnStruct)
		{
			ReturnStruct = new(EC_InternalUseOnlyConstructor, Outer, TEXT("MyPluginStruct"), RF_Public|RF_Transient|RF_MarkAsNative) UScriptStruct(FObjectInitializer(), NULL, new UScriptStruct::TCppStructOps<FMyPluginStruct>, EStructFlags(0x00000001));
			UProperty* NewProp_TestString = new(EC_InternalUseOnlyConstructor, ReturnStruct, TEXT("TestString"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(TestString, FMyPluginStruct), 0x0010000000000000);
			ReturnStruct->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnStruct->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnStruct, TEXT("ModuleRelativePath"), TEXT("Classes/MyPluginObject.h"));
			MetaData->SetValue(ReturnStruct, TEXT("ToolTip"), TEXT("Example UStruct declared in a plugin module"));
			MetaData->SetValue(NewProp_TestString, TEXT("ModuleRelativePath"), TEXT("Classes/MyPluginObject.h"));
#endif
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FMyPluginStruct_CRC() { return 3704503415U; }
	UClass* Z_Construct_UClass_UMyPluginObject_NoRegister()
	{
		return UMyPluginObject::StaticClass();
	}
	UClass* Z_Construct_UClass_UMyPluginObject()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UObject();
			Z_Construct_UPackage__Script_UObjectPlugin();
			OuterClass = UMyPluginObject::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20000080;


PRAGMA_DISABLE_DEPRECATION_WARNINGS
				UProperty* NewProp_MyStruct = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("MyStruct"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(CPP_PROPERTY_BASE(MyStruct, UMyPluginObject), 0x0040000000000000, Z_Construct_UScriptStruct_FMyPluginStruct());
PRAGMA_ENABLE_DEPRECATION_WARNINGS
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("MyPluginObject.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Classes/MyPluginObject.h"));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("Example of declaring a UObject in a plugin module"));
				MetaData->SetValue(NewProp_MyStruct, TEXT("ModuleRelativePath"), TEXT("Classes/MyPluginObject.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UMyPluginObject(Z_Construct_UClass_UMyPluginObject, &UMyPluginObject::StaticClass, TEXT("UMyPluginObject"), false, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMyPluginObject);
	UPackage* Z_Construct_UPackage__Script_UObjectPlugin()
	{
		static UPackage* ReturnPackage = NULL;
		if (!ReturnPackage)
		{
			ReturnPackage = CastChecked<UPackage>(StaticFindObjectFast(UPackage::StaticClass(), NULL, FName(TEXT("/Script/UObjectPlugin")), false, false));
			ReturnPackage->SetPackageFlags(PKG_CompiledIn | 0x00000080);
			FGuid Guid;
			Guid.A = 0x85C7F4A1;
			Guid.B = 0x67940BFF;
			Guid.C = 0x00000000;
			Guid.D = 0x00000000;
			ReturnPackage->SetGuid(Guid);

		}
		return ReturnPackage;
	}
#endif

PRAGMA_ENABLE_DEPRECATION_WARNINGS
