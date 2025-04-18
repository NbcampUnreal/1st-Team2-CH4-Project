#include "BTTask_DeadAI.h"
#include "AIController.h"
#include "PlayerCharacter.h" // Dead() 정의된 곳
#include "GameFramework/Pawn.h"

UBTTask_DeadAI::UBTTask_DeadAI()
{
	NodeName = TEXT("Dead AI");
}

EBTNodeResult::Type UBTTask_DeadAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("🟣 BTTask_DeadAI: ExecuteTask() 호출됨"));

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ AIController 없음"));
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ AI Pawn 없음"));
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Warning, TEXT("✅ AI Pawn: %s"), *AIPawn->GetClass()->GetName());

	APlayerCharacter* Char = Cast<APlayerCharacter>(AIPawn);
	if (!Char)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 캐릭터 Cast<APlayerCharacter> 실패"));
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Warning, TEXT("🟢 캐릭터 캐스팅 성공, Dead() 호출 시도"));
	Char->Dead(); // Dead 함수 안에도 로그 추가해줘야 함

	return EBTNodeResult::Succeeded;
}