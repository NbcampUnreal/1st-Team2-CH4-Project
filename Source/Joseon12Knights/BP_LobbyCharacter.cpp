#include "BP_LobbyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GI_GameCoreInstance.h"
#include "Kismet/GameplayStatics.h"

ABP_LobbyCharacter::ABP_LobbyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;  

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(SceneRoot);  

    PlayerNoText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerNoText"));
    PlayerNoText->SetupAttachment(SceneRoot);

    ReadyText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ReadyText"));
    ReadyText->SetupAttachment(SceneRoot);
}


void ABP_LobbyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 자기 클라이언트가 아니면 감추기
    if (!bIsLocalClient)
    {
        SetActorHiddenInGame(true);
    }

    if (PlayerIndex >= 0)
    {
        PlayerNoText->SetText(FText::FromString(FString::Printf(TEXT("P%d"), PlayerIndex + 1)));
    }
}

void ABP_LobbyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (UGI_GameCoreInstance* GI = Cast<UGI_GameCoreInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        const TArray<bool>& ReadyArray = GI->ReadyArray;
        const bool bReady = ReadyArray.IsValidIndex(PlayerIndex) && ReadyArray[PlayerIndex];

        ReadyText->SetText(FText::FromString(bReady ? TEXT("READY") : TEXT("WAIT")));
        ReadyText->SetTextRenderColor(bReady ? FColor::Green : FColor::Red);
    }
}

void ABP_LobbyCharacter::SetCharacter(const FString& InCharacterID)
{
    CharacterID = InCharacterID;

    USkeletalMesh* LoadedMesh = nullptr;

    if (InCharacterID == "1")
    {
        LoadedMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Characters/SK_Mannequin.SK_Mannequin"));
    }
    else if (InCharacterID == "2")
    {
        LoadedMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Characters/SK_Knight.SK_Knight"));
    }

    if (LoadedMesh)
    {
        Mesh->SetSkeletalMesh(LoadedMesh);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Character mesh for ID %s not found"), *InCharacterID);
    }
}

void ABP_LobbyCharacter::SetPlayerIndex(int32 Index)
{
    PlayerIndex = Index;
}

void ABP_LobbyCharacter::SetIsLocal(bool bLocal)
{
    bIsLocalClient = bLocal;
}
