// Copyright 2016 Cullen Sarles, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class GridLibraryProjectEditorTarget : TargetRules
{
	public GridLibraryProjectEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "LightWeightGridLibrary", "GridLibraryProject" } );
	}
}
