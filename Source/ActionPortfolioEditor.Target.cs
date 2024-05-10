// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ActionPortfolioEditorTarget : TargetRules
{
	public ActionPortfolioEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.AddRange(new string[] { "ActionPortfolio", "DialogueRuntime", "DialogueEditor", "WorldCustomEventRuntime", "WorldCustomEventUncooked", "CustomInputSettingRuntime", "CustomInputSettingEditor" });
	}
}
