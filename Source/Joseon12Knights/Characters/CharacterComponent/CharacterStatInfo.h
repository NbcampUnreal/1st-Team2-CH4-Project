

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStatInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterStatInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float Defense;


	FCharacterStatInfo()
		: MaxHP(100.f)
		, Attack(10.f)
		, Defense(5.f)
	{
	}
};
