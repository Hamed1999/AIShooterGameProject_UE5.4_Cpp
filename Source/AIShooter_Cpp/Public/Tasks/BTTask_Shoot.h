// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_CPP_API UBTTask_Shoot : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Shoot(const FObjectInitializer& ObjectInitializer);
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
