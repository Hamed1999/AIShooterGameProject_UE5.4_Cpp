// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Actors/Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

#define OUT

void AGun::CreateRootComponent()
{
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;
}

void AGun::CreateMeshComponent()
{
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Gun Mesh"));
	GunMeshComponent->SetupAttachment(RootComponent);
}

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateRootComponent();
	CreateMeshComponent();
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}


void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::HandleFiringEffects()
{
	if(FireSound)
		UGameplayStatics::SpawnSoundAttached(FireSound, GunMeshComponent, FName("MuzzleFlash"));
	for (UParticleSystem* FireParticle : FireParticles)
	{
		if(FireParticle)
			UGameplayStatics::SpawnEmitterAttached(FireParticle, GunMeshComponent, FName("MuzzleFlash"));
	}
}

void AGun::FindCameraPoint()
{
	if (APawn* OwnerPawn =Cast<APawn>(GetOwner()))
		OwnerPawn->GetController()->GetPlayerViewPoint(OUT CameraLocation, OUT CameraRotation);
	// DrawDebugCamera(GetWorld(), CameraLocation, CameraRotation, 90, 2, FColor::Red, false, 1);
}

bool AGun::ApplyBulletTrace(FHitResult& BulletHitResults)
{
	FVector TraceEnd = CameraLocation + TraceRange * CameraRotation.Vector();
	bool HitSomething = GetWorld()->LineTraceSingleByChannel(BulletHitResults, CameraLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1);
	// DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Yellow, false, 1, 0, 2);
	return HitSomething;
}

void AGun::HandleBodyImpactEffects(FHitResult const& BulletHitResults)
{
	if(ImpactBodySound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactBodySound, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
	for (UParticleSystem* ImpactBodyParticle : ImpactBodyParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBodyParticle, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
	}
}

void AGun::HandleSurfaceImpactEffects(FHitResult const& BulletHitResults)
{
	if(ImpactSurfaceSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSurfaceSound, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
	for (UParticleSystem* ImpactSurfaceParticle : ImpactSurfaceParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactSurfaceParticle, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
	}
}

/** Applies Damage */
void AGun::HandleApplyDamage(FHitResult const& BulletHitResults, APawn* const& DamagedPawn)
{
	if (TargetDamageType == EDamageType::PointDamage)
	{
		FVector ShotVector = BulletHitResults.ImpactPoint - CameraLocation;
		FVector ShotDirection = ShotVector.GetSafeNormal();
		AController* OwnerInstigator = Cast<APawn>(GetOwner())->GetController();
		UGameplayStatics::ApplyPointDamage(DamagedPawn, BaseDamage, ShotDirection, BulletHitResults,
			OwnerInstigator, this, DamageTypeClass);
	}
	else
		HandleRadialDamage(BulletHitResults);
}

void AGun::HandleRadialDamage(FHitResult const& BulletHitResults)
{
	TArray<AActor*> IgnoredActors;
	AController* OwnerInstigator = Cast<APawn>(GetOwner())->GetController();
	if (TargetDamageType == EDamageType::RadialDamage)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, BulletHitResults.ImpactPoint, DamageRadius,
			DamageTypeClass, IgnoredActors, this, OwnerInstigator);
	}
	else if (TargetDamageType == EDamageType::RadialDamageWithFalloff)
	{
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), BaseDamage, MinimumDamage,
			BulletHitResults.ImpactPoint, DamageInnerRadius, DamageOuterRadius,
			DamageFalloff, DamageTypeClass, IgnoredActors, this, OwnerInstigator);
	}
}

void  AGun::Shoot()
{
	HandleFiringEffects();
	FindCameraPoint();
	FHitResult BulletHitResults;
	if(ApplyBulletTrace(BulletHitResults))
	{
		if(APawn* DamagedPawn = Cast<APawn>(BulletHitResults.GetActor())) // Modify Later
		{
			HandleBodyImpactEffects(BulletHitResults);
			HandleApplyDamage(BulletHitResults, DamagedPawn);
		}
		else /** If impactes with anything except Pawns or Characters such as Soldier Character*/
		{
			HandleSurfaceImpactEffects(BulletHitResults);
			HandleRadialDamage(BulletHitResults);
		}
	}
}
