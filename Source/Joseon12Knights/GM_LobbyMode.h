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

	void CheckAllPlayersReady();


};
