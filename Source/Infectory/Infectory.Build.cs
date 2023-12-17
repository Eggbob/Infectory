// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Infectory : ModuleRules
{
	public Infectory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicIncludePaths.AddRange(new string[] { "Infectory" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
