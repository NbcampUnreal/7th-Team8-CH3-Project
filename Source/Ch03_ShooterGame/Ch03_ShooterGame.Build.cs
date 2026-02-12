// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ch03_ShooterGame : ModuleRules
{
	public Ch03_ShooterGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput","GameplayTasks" });
    }
}
