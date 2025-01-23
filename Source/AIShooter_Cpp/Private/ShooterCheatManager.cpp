// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "ShooterCheatManager.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SoldierCharacter.h"

void UShooterCheatManager::HackGun(int Ammo)
{
	SetSoldier();
	if (Soldier)
		Soldier->SetAmmo(Ammo);
}

void UShooterCheatManager::HackHealth(float NewHealth)
{
	SetSoldier();
	if (Soldier)
		Soldier->SetHealth(NewHealth);
}

void UShooterCheatManager::SetSoldier()
{
	APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(this, 0);
	Soldier = Cast<ASoldierCharacter>(PlayerCharacter);
}
