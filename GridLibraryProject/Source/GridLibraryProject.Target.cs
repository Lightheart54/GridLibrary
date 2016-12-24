// Copyright 2016 Cullen Sarles, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class GridLibraryProjectTarget : TargetRules
{
	public GridLibraryProjectTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
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
		OutExtraModuleNames.AddRange( new string[] { "GridLibraryProject", "LightWeightGridLibrary" } );
	}
}
