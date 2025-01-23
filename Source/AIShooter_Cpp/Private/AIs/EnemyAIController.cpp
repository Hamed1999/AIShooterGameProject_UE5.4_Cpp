// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "AIs/EnemyAIController.h"

#include "Actors/Gun.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SoldierCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

void AEnemyAIController::CreatePerceptionComponent()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("Perception Component"));
	CreateSightSenseConfig();
}

void AEnemyAIController::CreateSightSenseConfig()
{
	UAISenseConfig_Sight* SightConfig;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
	SightConfig->SightRadius = 5000.0f;
	SightConfig->LoseSightRadius = 10000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true,
		false, false); 
	AIPerceptionComponent->ConfigureSense(*SightConfig);
}

void AEnemyAIController::BindOnPlayerSeen()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPlayerSeen);
}

void AEnemyAIController::SetBehaviorTree()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/AI/BT_Enemy"));
	if (BehaviorTreeRef.Succeeded())
		BT_Enemy = BehaviorTreeRef.Object.Get();
}

AEnemyAIController::AEnemyAIController()
{
	SetBehaviorTree();
	CreatePerceptionComponent();
	SetTeamId();
	BindOnPlayerSeen();
}

void AEnemyAIController::SetTeamId()
{
	if(Cast<ASoldierCharacter>(GetCharacter()))
	{
		TeamId = Cast<ASoldierCharacter>(GetCharacter())->GetGenericTeamId();
	}
	SetGenericTeamId(TeamId);
}

ASoldierCharacter* AEnemyAIController::GetSoldier()
{
	if(ASoldierCharacter* SoldierCharacter = Cast<ASoldierCharacter>(GetCharacter()))
		return SoldierCharacter;
	return nullptr;
}

void AEnemyAIController::SetInitBlackboardValues()
{
	if(!GetBlackboardComponent()) return;
	GetBlackboardComponent()->SetValueAsVector(FName("FirstLocation"), GetPawn()->GetActorLocation());
	GetBlackboardComponent()->SetValueAsVector(FName("Destination"), DestinationLocation);
	GetBlackboardComponent()->SetValueAsFloat(FName("ReloadTime"), GetSoldier()->GetActiveGun()->ReloadTime);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	SetTeamId();
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
	{
		if (BT_Enemy)
		{
			RunBehaviorTree(BT_Enemy);
			if (GetPawn())
				SetInitBlackboardValues();
		}
	});
     GetWorldTimerManager().SetTimer(TimerHandle,TimerDelegate,2,false);
}

void AEnemyAIController::OnPlayerSeen(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed()  && GetBlackboardComponent())
	{
		if (ASoldierCharacter* SoldierPlayer = Cast<ASoldierCharacter>(Actor))
		{
			if (float Health = SoldierPlayer->GetPlayerHealth() > 0)
			{
				GetBlackboardComponent()->SetValueAsObject(FName("Player"), SoldierPlayer);
				GetBlackboardComponent()->SetValueAsFloat(FName("PlayerHealth"), Health);
				SetFocus(SoldierPlayer);
			}
			else
				GetBlackboardComponent()->ClearValue(FName("Player"));
		}
	}
	else if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->ClearValue(FName("Player"));
		ClearFocus(EAIFocusPriority::Gameplay);
		GetBlackboardComponent()->SetValueAsVector(FName("LastSeenLocation"), Actor->GetActorLocation());
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}