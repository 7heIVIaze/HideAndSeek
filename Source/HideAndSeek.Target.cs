// Copyright Epic Games, Inc. All Rights Reserved.
// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HideAndSeekTarget : TargetRules
{
	public HideAndSeekTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("HideAndSeek");
	}
}
