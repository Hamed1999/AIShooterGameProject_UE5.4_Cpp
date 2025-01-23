// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Reload.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_CPP_API UBTTask_Reload : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Reload(const FObjectInitializer& ObjectInitializer);
private:
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
