// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LightWeightGridLibrary : ModuleRules
{
	public LightWeightGridLibrary(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		//PublicIncludePaths.AddRange(
        //    new string[]{
        //        "LightWeightGridLibrary/Public"
        //    });
 
        //PrivateIncludePaths.AddRange(
        //    new string[] {
		//		"LightWeightGridLibrary/Private"
		//	});


	}
}
