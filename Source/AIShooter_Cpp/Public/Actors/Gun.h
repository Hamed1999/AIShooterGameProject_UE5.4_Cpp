// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM(BlueprintType)
	enum class EDamageType : uint8
{
	/** Bullets Damage */
	PointDamage UMETA(DisplayName = "Point Damage"),
	/** Rockets or Missiles Damage */
	RadialDamage UMETA(DisplayName = "Auto Radial Damage"),
	/** Customized Rockets or Missiles Damage */
	RadialDamageWithFalloff UMETA(DisplayName = "Radial Damage With Falloff"),
};

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
	void HideMesh();
private:
	/**
	* Methods
	**/
	void CreateRootComponent();
	void CreateMeshComponent();
	void HandleFiringEffects();
	void FindCameraPoint();
	bool ApplyBulletTrace(FHitResult& BulletHitResults);
	void HandleBodyImpactEffects(FHitResult const& BulletHitResults);
	void HandleSurfaceImpactEffects(FHitResult const& BulletHitResults);
	void HandleApplyDamage(FHitResult const& BulletHitResults, APawn* const& DamagedPawn);
	void HandleRadialDamage(FHitResult const& BulletHitResults);
	/**
	* Global Properties
	*/
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess=true))
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess=true))
		USkeletalMeshComponent* GunMeshComponent;
	// Properties
	FVector CameraLocation = FVector().Zero();
	FRotator CameraRotation = FRotator().ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Range", meta=(AllowPrivateAccess=true))
		float TraceRange = 43000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Base", meta=(AllowPrivateAccess=true, ClampMin = "0"))
		float BaseDamage = 12.f;
	float Damage = BaseDamage;
	bool bHideMesh = false;
	/** Indicates how damage will apply to actors.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Base", meta=(AllowPrivateAccess=tru))
		EDamageType TargetDamageType = EDamageType::PointDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Base", meta=(AllowPrivateAccess=true))
		TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Auto Radial Damage", meta=(AllowPrivateAccess=true))	
		float DamageRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Radial Damage With Falloff", meta=(AllowPrivateAccess=true))	
		float MinimumDamage =0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Radial Damage With Falloff", meta=(AllowPrivateAccess=true))
		float DamageInnerRadius = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Radial Damage With Falloff", meta=(AllowPrivateAccess=true))
		float DamageOuterRadius = 1000.f;
	/** 1 is linear.
	 * Greater than 1 cause damage to drop more rapidly between the inner and outer radii.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Damage | Radial Damage With Falloff", meta=(AllowPrivateAccess=true))
		float DamageFalloff = 1.f;
	// Effects
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
