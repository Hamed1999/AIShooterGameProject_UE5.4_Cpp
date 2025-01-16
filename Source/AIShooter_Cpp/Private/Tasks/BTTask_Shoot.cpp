// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Tasks/BTTask_Shoot.h"
#include "Characters/SoldierCharacter.h"
#include "AIController.h"

UBTTask_Shoot::UBTTask_Shoot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (AAIController* Controller = OwnerComp.GetAIOwner())
		if (ASoldierCharacter* ShootingCharacter = Cast<ASoldierCharacter>(Controller->GetPawn()))
		{
			ShootingCharacter->Fire();
			return EBTNodeResult::Succeeded;
		}
	return EBTNodeResult::Failed;
}