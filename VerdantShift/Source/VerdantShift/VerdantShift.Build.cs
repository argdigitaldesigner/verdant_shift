// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VerdantShift : ModuleRules
{
	public VerdantShift(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
		PrivateDependencyModuleNames.Add("CableComponent");

		PublicIncludePaths.AddRange(new string[] {
			"VerdantShift",
			"VerdantShift/Variant_Platforming",
			"VerdantShift/Variant_Platforming/Animation",
			"VerdantShift/Variant_Combat",
			"VerdantShift/Variant_Combat/AI",
			"VerdantShift/Variant_Combat/Animation",
			"VerdantShift/Variant_Combat/Gameplay",
			"VerdantShift/Variant_Combat/Interfaces",
			"VerdantShift/Variant_Combat/UI",
			"VerdantShift/Variant_SideScrolling",
			"VerdantShift/Variant_SideScrolling/AI",
			"VerdantShift/Variant_SideScrolling/Gameplay",
			"VerdantShift/Variant_SideScrolling/Interfaces",
			"VerdantShift/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
