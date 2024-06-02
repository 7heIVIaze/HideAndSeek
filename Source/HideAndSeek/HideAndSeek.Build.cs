// Copyright Epic Games, Inc. All Rights Reserved.
// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

using UnrealBuildTool;

public class HideAndSeek : ModuleRules
{
	public HideAndSeek(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput", 
			"UMG", 
			"NavigationSystem", 
			"AIModule", 
			"GameplayTasks", 
			"Niagara", 
			"ApplicationCore", 
			"MoviePlayer", 
			"StoveSDKPlugin", 
			"OnlineSubsystemSteam", 
			"OnlineSubsystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate", 
			"SlateCore", 
			"InputCore", 
			"EnhancedInput", 
			"RHI", 
			"HeadMountedDisplay", 
			"StoveSDKPlugin" 
		});
	}
}
