// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	void CreatePerceptionComponent();
	void CreateSightSenseConfig();
	void BindOnPlayerSeen();
	AEnemyAIController();
	void SetTeamId();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	/**
	*Methods
	*/
	UFUNCTION(BlueprintType)
		void OnPlayerSeen(AActor* Actor, struct FAIStimulus Stimulus);
	/**
	 * Global Properties
	 */
	APawn* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess=true))
		UBehaviorTree* BT_Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess=true))
		UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess=true))
		FGenericTeamId TeamId = FGenericTeamId(1);
	class ASoldierCharacter* SoldierPlayer;
};
