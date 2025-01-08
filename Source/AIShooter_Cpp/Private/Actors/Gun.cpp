// Copyright Hamed Salmanizadegan _ Email: hsalmanizadegan@gmail.com All Rights Reserved.


#include "Actors/Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

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

void  AGun::Shoot()
{
	if(FireSound)
		UGameplayStatics::SpawnSoundAttached(FireSound, GunMeshComponent, FName("MuzzleFlash"));

	for (UParticleSystem* FireParticle : FireParticles)
	{
		if(FireParticle)
			UGameplayStatics::SpawnEmitterAttached(FireParticle, GunMeshComponent, FName("MuzzleFlash"));
	}
	
	FVector CameraLocation = FVector().Zero();
	FRotator CameraRotation = FRotator().ZeroRotator;
	if (APawn* OwnerPawn =Cast<APawn>(GetOwner()))
		OwnerPawn->GetController()->GetPlayerViewPoint(OUT CameraLocation, OUT CameraRotation);
	// DrawDebugCamera(GetWorld(), CameraLocation, CameraRotation, 90, 2, FColor::Red, false, 1);
	FHitResult BulletHitResults;
	FVector TraceEnd = CameraLocation + TraceRange * CameraRotation.Vector();
	// DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Yellow, false, 1, 0, 2);
if(GetWorld()->LineTraceSingleByChannel(BulletHitResults, CameraLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1))
{
	if(Cast<APawn>(BulletHitResults.GetActor())) // Modify Later
	{
		if(ImpactBodySound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactBodySound, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
		for (UParticleSystem* ImpactBodyParticle : ImpactBodyParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBodyParticle, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
		}
	}
	else
	{
		if(ImpactSurfaceSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSurfaceSound, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
		for (UParticleSystem* ImpactSurfaceParticle : ImpactSurfaceParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactSurfaceParticle, BulletHitResults.ImpactPoint, BulletHitResults.ImpactNormal.Rotation());
		}
	}
}
}

