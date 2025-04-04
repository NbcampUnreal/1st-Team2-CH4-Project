

using UnrealBuildTool;
using System.Collections.Generic;

public class Joseon12KnightsEditorTarget : TargetRules
{
	public Joseon12KnightsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Joseon12Knights" } );
	}
}
