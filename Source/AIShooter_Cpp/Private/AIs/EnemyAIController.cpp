// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "AIs/EnemyAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyAIController::AEnemyAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/AI/BT_Enemy"));
	if (BehaviorTreeRef.Succeeded())
	{
		BT_Enemy = BehaviorTreeRef.Object.Get();
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	RunBehaviorTree(BT_Enemy);
	
	
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(LineOfSightTo(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("In Sight...."))
		GetBlackboardComponent()->SetValueAsObject(FName("Player"), Player);
		SetFocus(Player);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Out of Sight !!!!!"))
		GetBlackboardComponent()->ClearValue(FName("Player"));
		SetFocus(nullptr);
	}
}