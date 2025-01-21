// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_CPP_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwnerCharacter(ACharacter* OwnerCharacter);
private:
	UFUNCTION(BlueprintPure)
		float GetPlayerHealth(); 
	 class ASoldierCharacter* PlayerCharacter = nullptr;
};