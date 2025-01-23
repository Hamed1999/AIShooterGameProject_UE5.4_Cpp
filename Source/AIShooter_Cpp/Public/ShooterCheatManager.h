// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "ShooterCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_CPP_API UShooterCheatManager : public UCheatManager
{
	GENERATED_BODY()
private:
	UFUNCTION(Exec)
		void HackGun(int Ammo = 999);
	UFUNCTION(Exec)
		void HackHealth(float NewHealth = 999);
	void SetSoldier();
	class ASoldierCharacter* Soldier = nullptr;
};
