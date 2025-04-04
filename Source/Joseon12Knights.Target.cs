

using UnrealBuildTool;
using System.Collections.Generic;

public class Joseon12KnightsTarget : TargetRules
{
	public Joseon12KnightsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Joseon12Knights" } );
	}
}
