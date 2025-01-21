// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Widgets/PlayerHUD.h"

#include "Characters/SoldierCharacter.h"

void UPlayerHUD::SetOwnerCharacter(ACharacter* OwnerCharacter)
{
	PlayerCharacter = Cast<ASoldierCharacter>(OwnerCharacter);
}

float UPlayerHUD::GetPlayerHealth()
{
	if(PlayerCharacter)
		return PlayerCharacter->GetPlayerHealth();
	return 0;
}