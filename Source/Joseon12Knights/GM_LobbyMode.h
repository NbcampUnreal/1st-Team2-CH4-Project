// GM_LobbyMode.h
#pragma once

#include "CoreMinimal.h"
#include "GM_BaseMode.h"
#include "GM_LobbyMode.generated.h"

UCLASS()
class JOSEON12KNIGHTS_API AGM_LobbyMode : public AGM_BaseMode
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_SetReady(APlayerController* PC);

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	// Ready 상태 체크
	void CheckAllPlayersReady();

	// 클라이언트에서 Ready 요청 시 호출될 함수 (RPC)

};
