// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HideAndSeek : ModuleRules
{
	public HideAndSeek(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "Niagara", "ApplicationCore", "MoviePlayer" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "InputCore", "EnhancedInput", "RHI", "HeadMountedDisplay" });
	}
}
