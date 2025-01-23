// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Tasks/BTTask_Reload.h"

#include "AIController.h"
#include "Characters/SoldierCharacter.h"

UBTTask_Reload::UBTTask_Reload(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Reload Gun";
}

EBTNodeResult::Type UBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if(ASoldierCharacter* SoldierCharacter = Cast<ASoldierCharacter>(Controller->GetCharacter()))
	{
		SoldierCharacter->Reload();
		if (SoldierCharacter->GetAmmo() > 0) return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}