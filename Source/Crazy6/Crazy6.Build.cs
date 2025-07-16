// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;


public class Crazy6 : ModuleRules
{
	public Crazy6(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"Niagara",
            "PhysicsCore",
			"UMG",
			"GameplayCameras",
            "Chaos",
            "ChaosSolverEngine",
            "GeometryCollectionEngine",
            "FieldSystemEngine",
            "ChaosCaching",
			"Networking",
			"AnimGraphRuntime",
			"OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemSteam",
            "MediaAssets",
            "Paper2D",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate",
			"SlateCore",
 
        });
        
        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        
    }
}
