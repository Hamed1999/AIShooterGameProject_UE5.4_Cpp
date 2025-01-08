// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class AISHOOTER_CPP_API AGun : public AActor
{
	GENERATED_BODY()
	
public:
	AGun();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Shoot();
private:
	/**
	* Methods
	**/
	void CreateRootComponent();
	void CreateMeshComponent();
	/**
	* Global Properties
	*/
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess=true))
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess=true))
		USkeletalMeshComponent* GunMeshComponent;
	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun", meta=(AllowPrivateAccess=true))
		float TraceRange = 43000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(AllowPrivateAccess=true))
		USoundBase* FireSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(AllowPrivateAccess=true))
		TArray<UParticleSystem*> FireParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(AllowPrivateAccess=true))
		USoundBase* ImpactSurfaceSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(AllowPrivateAccess=true))
		USoundBase* ImpactBodySound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(AllowPrivateAccess=true))
		TArray<UParticleSystem*> ImpactSurfaceParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(AllowPrivateAccess=true))
		TArray<UParticleSystem*> ImpactBodyParticles;
};
