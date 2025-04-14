using UnrealBuildTool;
using System.IO;

public class Joseon12Knights : ModuleRules
{
    public Joseon12Knights(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore",
            "EnhancedInput",
            "NavigationSystem",  
            "AIModule", 
            "GamePlayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
        
        PublicIncludePaths.AddRange(new string[] {
			Path.Combine(ModuleDirectory, "Characters")
		});
    }
}
