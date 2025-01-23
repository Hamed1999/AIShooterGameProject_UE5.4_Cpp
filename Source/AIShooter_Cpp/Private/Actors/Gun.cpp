// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Actors/Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/DamageEvents.h"
#include "Characters/SoldierCharacter.h"

#define OUT

void AGun::SetAmmo(int Ammo)
{
	if (Ammo > CurrentMagAmmo)
		CurrentAmmo = Ammo;
	else
	{
		CurrentAmmo = Ammo;
		CurrentMagAmmo = Ammo;
	}
}

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
	CurrentAmmo = MaxAmmo;
	CurrentMagAmmo = MaxMagAmmo;
}


void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::HandleFiringEffects()
{

	FName TargetSocket;
	USkeletalMeshComponent* TargetMeshComponent;
	FRotator TargetRotator = FRotator(0.f);
	if(bHideMesh)
	{
		TargetMeshComponent = Cast<ASoldierCharacter>(GetOwner())->GetMesh();
		TargetSocket = FName("gun_barrel");
		TargetRotator = FRotator(-90.f, -90 , 0);
	}
	else
	{
		TargetMeshComponent = GunMeshComponent;
		TargetSocket = FName("MuzzleFlash");
	}
	if(FireSound)
		UGameplayStatics::SpawnSoundAttached(FireSound, TargetMeshComponent, TargetSocket, FVector(0), TargetRotator);
	for (UParticleSystem* FireParticle : FireParticles)
	{
		if(FireParticle)
			UGameplayStatics::SpawnEmitterAttached(FireParticle, TargetMeshComponent, TargetSocket, FVector(0), TargetRotator);
	}
}

void AGun::FindCameraPoint()
{
	if (APawn* OwnerPawn =Cast<APawn>(GetOwner()))
		if (AController* Controller = OwnerPawn->GetController())
			Controller->GetPlayerViewPoint(OUT CameraLocation, OUT CameraRotation);
	// DrawDebugCamera(GetWorld(), CameraLocation, CameraRotation, 90, 2, FColor::Red, false, 1);
}

void AGun::SetCollisionIgnoredActors(FCollisionQueryParams& Params)
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	IgnoreActors.Add(GetOwner());
	Params.AddIgnoredActors(IgnoreActors);
}

bool AGun::ApplyBulletTrace(FHitResult& BulletHitResults)
{
	FVector TraceEnd = CameraLocation + TraceRange * CameraRotation.Vector();
	FCollisionQueryParams Params;
	SetCollisionIgnoredActors(OUT Params);
	bool HitSomething = GetWorld()->LineTraceSingleByChannel(BulletHitResults, CameraLocation,
		TraceEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
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
		UGameplayStatics::ApplyPointDamage(DamagedPawn, Damage, ShotDirection, BulletHitResults,
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
	if (CurrentMagAmmo <= 0) return;
	HandleFiringEffects();
	FindCameraPoint();
	FHitResult BulletHitResults;
	if(ApplyBulletTrace(BulletHitResults))
	{
		CurrentMagAmmo--;
		if(APawn* DamagedPawn = Cast<APawn>(BulletHitResults.GetActor())) // Modify Later
		{
			HandleBodyImpactEffects(BulletHitResults);
			if (BulletHitResults.BoneName == "head")
				Damage = 500;
			else
				Damage = BaseDamage;
			HandleApplyDamage(BulletHitResults, DamagedPawn);
		}
		else /** If impactes with anything except Pawns or Characters such as Soldier Character*/
		{
			HandleSurfaceImpactEffects(BulletHitResults);
			HandleRadialDamage(BulletHitResults);
		}
	}
}

void AGun::HideMesh()
{
	bHideMesh = true;
	GunMeshComponent->SetVisibility(false);
}

bool AGun::Reload()
{
	if (CurrentAmmo == 0) return false;
    	int dif = MaxMagAmmo - CurrentMagAmmo;
    	if (dif == 0) return true;
    	if (CurrentAmmo >= dif)
    	{
    		CurrentAmmo -= dif;
    		CurrentMagAmmo = MaxMagAmmo;
    	}
    	else
    	{
    		CurrentMagAmmo += CurrentAmmo;
    		CurrentAmmo = 0;
    	}
    	return true;
}