#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GM_LobbyMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_LobbyMode : public AGM_BaseMode
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

    UFUNCTION(BlueprintCallable, Server, Reliable)
    void Server_UpdatePlayers(int32 NumPlayers);

    UFUNCTION(Client, Reliable)
    void Client_UpdatePlayers(int32 NumPlayers);

    UFUNCTION(Server, Reliable)
    void Server_SetReady(APlayerController* PC);

    UFUNCTION(BlueprintCallable)
    void StartMatch();  // 맵 이동 처리

protected:
    UPROPERTY()
    class UGI_GameCoreInstance* GameInstance;

    UPROPERTY(EditAnywhere, Category = "Lobby")
    TMap<FString, TSubclassOf<AActor>> CharacterBPMap;  // ✅ 캐릭터 ID → 블루프린트

    UPROPERTY()
    TArray<AActor*> SpawnedLobbyCharacters;

    int32 SpawnedCount = 0;

    bool bStartClosed = false;
};
