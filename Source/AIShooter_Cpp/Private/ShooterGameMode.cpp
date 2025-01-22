// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "ShooterGameMode.h"
#include "ShooterPlayerController.h"
#include "Characters/SoldierCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AShooterGameMode::AShooterGameMode()
{
	ConstructorHelpers::FClassFinder<ASoldierCharacter> SoldierCharacterClassRef(TEXT("/Game/Blueprints/SoldierCharacter/BP_SoldierCharacter"));
	if (SoldierCharacterClassRef.Succeeded())
		DefaultPawnClass = SoldierCharacterClassRef.Class.Get();
	PlayerControllerClass = AShooterPlayerController::StaticClass();
}

void AShooterGameMode::HandleKills(APawn* KilledPawn)
{
	AController* Controller = KilledPawn->GetController();
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->GameHasEnded();
	}
	else
	{
		for (auto Soldier : TActorRange<ASoldierCharacter>(GetWorld()))
			if (Soldier->GetGenericTeamId() != FirstPlayerCharacter->GetGenericTeamId())
				if (!Soldier->IsDead()) return;
		FirstPlayerCharacter->GetController()->GameHasEnded(FirstPlayerCharacter, true);
	}
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	FirstPlayerCharacter = Cast<ASoldierCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if(FirstPlayerCharacter) FirstPlayerCharacter->bIsLeader = true;
}
