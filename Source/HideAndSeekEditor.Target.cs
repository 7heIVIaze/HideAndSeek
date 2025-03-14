// Copyright Epic Games, Inc. All Rights Reserved.
// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.ㅎ

using UnrealBuildTool;
using System.Collections.Generic;

public class HideAndSeekEditorTarget : TargetRules
{
	public HideAndSeekEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("HideAndSeek");
	}
}
