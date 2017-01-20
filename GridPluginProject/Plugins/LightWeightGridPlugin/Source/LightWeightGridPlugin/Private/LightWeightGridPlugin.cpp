// Copyright 2017 Cullen Sarles, Inc. All Rights Reserved.

#include "LightWeightGridPluginPCH.h"

class FLightWeightGridPlugin : public ILightWeightGridPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FLightWeightGridPlugin, LightWeightGridPlugin)



void FLightWeightGridPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FLightWeightGridPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



