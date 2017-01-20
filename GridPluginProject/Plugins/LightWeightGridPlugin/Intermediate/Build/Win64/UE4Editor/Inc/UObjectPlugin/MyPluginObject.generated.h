// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UOBJECTPLUGIN_MyPluginObject_generated_h
#error "MyPluginObject.generated.h already included, missing '#pragma once' in MyPluginObject.h"
#endif
#define UOBJECTPLUGIN_MyPluginObject_generated_h

#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_14_GENERATED_BODY \
	friend UOBJECTPLUGIN_API class UScriptStruct* Z_Construct_UScriptStruct_FMyPluginStruct(); \
	UOBJECTPLUGIN_API static class UScriptStruct* StaticStruct();


#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_RPC_WRAPPERS
#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_RPC_WRAPPERS_NO_PURE_DECLS
#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUMyPluginObject(); \
	friend UOBJECTPLUGIN_API class UClass* Z_Construct_UClass_UMyPluginObject(); \
	public: \
	DECLARE_CLASS(UMyPluginObject, UObject, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/UObjectPlugin"), NO_API) \
	DECLARE_SERIALIZER(UMyPluginObject) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_INCLASS \
	private: \
	static void StaticRegisterNativesUMyPluginObject(); \
	friend UOBJECTPLUGIN_API class UClass* Z_Construct_UClass_UMyPluginObject(); \
	public: \
	DECLARE_CLASS(UMyPluginObject, UObject, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/UObjectPlugin"), NO_API) \
	DECLARE_SERIALIZER(UMyPluginObject) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UMyPluginObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UMyPluginObject) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMyPluginObject); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMyPluginObject); \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UMyPluginObject(const UMyPluginObject& InCopy); \
public:


#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UMyPluginObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UMyPluginObject(const UMyPluginObject& InCopy); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMyPluginObject); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMyPluginObject); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UMyPluginObject)


#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_24_PROLOG
#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_RPC_WRAPPERS \
	GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_INCLASS \
	GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
	GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_INCLASS_NO_PURE_DECLS \
	GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h_27_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class MyPluginObject."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID GridLibraryProject_Plugins_UObjectPlugin_Source_UObjectPlugin_Classes_MyPluginObject_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
