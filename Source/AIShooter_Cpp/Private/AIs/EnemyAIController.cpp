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
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 3500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->DetectionByAffiliation = FAISenseAffiliationFilter(true, true, true);
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

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	RunBehaviorTree(BT_Enemy);
	SetTeamId();
}

void AEnemyAIController::OnPlayerSeen(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Error, TEXT("In Sight...."))
		GetBlackboardComponent()->SetValueAsObject(FName("Player"), Player);
		SetFocus(Player);
		// FTimerHandle FireTimerHandle;
		// FTimerDelegate FireTimerDelegate;
		// FireTimerDelegate.BindLambda([&]()
		// {
		// 	SoldierPlayer->Fire();
		// });
		// GetWorldTimerManager().SetTimer(FireTimerHandle, FireTimerDelegate, 0.3,true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Out of Sight !!!!!"))
		GetBlackboardComponent()->ClearValue(FName("Player"));
		SetFocus(nullptr);
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}