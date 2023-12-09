using UnrealBuildTool;

public class CqsjFlowMove : ModuleRules
{
    public CqsjFlowMove(ReadOnlyTargetRules Target) : base(Target)
    {
        
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "GameplayTags", 
                "InputCore",
                "CqsjAnim",
                "CqsjRMS",
                "CqsjPowerfulState"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}