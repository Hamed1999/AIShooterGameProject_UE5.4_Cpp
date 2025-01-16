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
	AEnemyAIController();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destination")
		FVector DestinationLocation = FVector(-118.164852,4731.627344,134.400094);
protected:
	virtual void BeginPlay() override;
	/**
	*Methods
	*/
	UFUNCTION(BlueprintType)
		void OnPlayerSeen(AActor* Actor, struct FAIStimulus Stimulus);
	void CreatePerceptionComponent();
	void CreateSightSenseConfig();
	void BindOnPlayerSeen();
	void SetTeamId();
	void SetBlackboardLocationValues();
	/**
	 * Global Properties
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess=true))
		UBehaviorTree* BT_Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess=true))
		UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess=true))
		FGenericTeamId TeamId = FGenericTeamId(1);
	class ASoldierCharacter* SoldierPlayer;
	
};
