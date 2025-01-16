// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "AIs/EnemyAIController.h"

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

AEnemyAIController::AEnemyAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/AI/BT_Enemy"));
	if (BehaviorTreeRef.Succeeded())
		BT_Enemy = BehaviorTreeRef.Object.Get();
	CreatePerceptionComponent();
	BindOnPlayerSeen();
}

void AEnemyAIController::SetTeamId()
{
	if(Cast<ASoldierCharacter>(GetCharacter()))
	{
		SoldierPlayer = Cast<ASoldierCharacter>(GetCharacter());
		TeamId = Cast<ASoldierCharacter>(GetCharacter())->GetGenericTeamId();
	}
		
	SetGenericTeamId(TeamId);
}

void AEnemyAIController::SetBlackboardLocationValues()
{
	GetBlackboardComponent()->SetValueAsVector(FName("FirstLocation"), GetPawn()->GetActorLocation());
	GetBlackboardComponent()->SetValueAsVector(FName("Destination"), DestinationLocation);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BT_Enemy);
	SetTeamId();
	SetBlackboardLocationValues();
	
}

void AEnemyAIController::OnPlayerSeen(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		if (Cast<ASoldierCharacter>(Actor)->GetPlayerHealth() > 0)
		{
			GetBlackboardComponent()->SetValueAsObject(FName("Player"), Actor);
			SetFocus(Actor);
		}
		else
			GetBlackboardComponent()->ClearValue(FName("Player"));
	}
	else
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