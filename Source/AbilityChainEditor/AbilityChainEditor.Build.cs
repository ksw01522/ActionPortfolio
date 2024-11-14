// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AbilityChainEditor : ModuleRules
{
	public AbilityChainEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = true;
		ShadowVariableWarningLevel = WarningLevel.Error;

        PublicIncludePaths.AddRange(new string[]
        {
            
        });

        PrivateIncludePaths.AddRange(new string[]
		{
			"AbilityChainRuntime/public",
			"AbilityChainEditor/private",
			"AbilityChainEditor/public",
            "AbilityChainEditor/private/GraphEditor"
        });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd"});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{	
				"AssetTools",
                "Slate",
				"SlateCore",
				"AbilityChainRuntime",
				"InputCore",
				"GraphEditor",
				"PropertyEditor",
				"EditorStyle",
				"Kismet",
				"KismetWidgets",
				"ApplicationCore",
				"ToolMenus",
				"UMG",
				"RenderCore"
			});

	}
}
