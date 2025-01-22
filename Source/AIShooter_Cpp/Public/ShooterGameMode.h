// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_CPP_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AShooterGameMode();
	void HandleKills(APawn* KilledPawn);
protected:
	void BeginPlay() override;
private:
	class ASoldierCharacter* FirstPlayerCharacter = nullptr;
};
