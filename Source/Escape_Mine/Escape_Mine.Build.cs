// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Escape_Mine : ModuleRules
{
	public Escape_Mine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		
	}
}
