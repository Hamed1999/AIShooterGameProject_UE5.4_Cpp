// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Tasks/BTTask_Shoot.h"
#include "Characters/SoldierCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Shoot::UBTTask_Shoot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (AAIController* Controller = OwnerComp.GetAIOwner())
		if (ASoldierCharacter* ShootingCharacter = Cast<ASoldierCharacter>(Controller->GetCharacter()))
		{
			UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
			ASoldierCharacter* TargetSoldier = Cast<ASoldierCharacter>(BlackBoard->GetValueAsObject(FName("Player")));
			if (ShootingCharacter->GetAmmo() <= 0) return EBTNodeResult::Failed;
			if (ShootingCharacter->GetMagAmmo() <= 0) return EBTNodeResult::Failed;
			BlackBoard->SetValueAsFloat(FName("PlayerHealth"), TargetSoldier->GetPlayerHealth());
			ShootingCharacter->Fire();
			BlackBoard->SetValueAsFloat(FName("PlayerHealth"), TargetSoldier->GetPlayerHealth());
			return EBTNodeResult::Succeeded;
		}
	return EBTNodeResult::Failed;
}