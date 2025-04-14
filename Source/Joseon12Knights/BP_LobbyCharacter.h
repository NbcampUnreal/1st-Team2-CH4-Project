#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_LobbyCharacter.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API ABP_LobbyCharacter : public AActor
{
    GENERATED_BODY()

public:
    ABP_LobbyCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable)
    void SetCharacter(const FString& InCharacterID);

    UFUNCTION(BlueprintCallable)
    void SetPlayerIndex(int32 Index);

    UFUNCTION(BlueprintCallable)
    void SetIsLocal(bool bLocal);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* SceneRoot;

    UPROPERTY(EditAnywhere)
    USkeletalMeshComponent* Mesh;


    UPROPERTY(EditAnywhere)
    class UTextRenderComponent* PlayerNoText;

    UPROPERTY(EditAnywhere)
    class UTextRenderComponent* ReadyText;

    FString CharacterID;
    int32 PlayerIndex = -1;
    bool bIsLocalClient = false;
};
