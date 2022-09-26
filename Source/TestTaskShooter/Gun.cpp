// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"

#include "Components/SphereComponent.h"

#include "ShooterCharacter.h"

// Sets default values
AGun::AGun() {
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Gun");
	MeshComponent->SetupAttachment(RootComponent);

	TriggerComponent = CreateDefaultSubobject<USphereComponent>("Trigger");
	TriggerComponent->SetupAttachment(MeshComponent);

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnOverlapBegin);
	
	AmmoCount = 30;
	MaxAvailableAmmo = 10;

	MaxShotRange = 10000;
	ShootDelay = 0.5;
	HitTime = 0.25;

	bIsReloading = false;

	ReloadTime = 1;

	bCanShoot = true;
	bHasHit = false;
}

// Called when the game starts or when spawned
void AGun::BeginPlay() {
	Super::BeginPlay();

	SetupAmmo();

	//ReloadTime *= 1000;
}

void AGun::SetupAmmo() {
	ReserveAmmo = AmmoCount;

	if (MaxAvailableAmmo <= AmmoCount) {
		AvailableAmmo = MaxAvailableAmmo;
		ReserveAmmo -= AvailableAmmo;
	}

	else {
		AvailableAmmo = AmmoCount;
		ReserveAmmo = 0;
	}
}

const AController* AGun::GetOwnerController() const {
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn) {
		return OwnerPawn->GetController();
	}

	return nullptr;
}

bool AGun::SetupLineTrace(FHitResult& HitResult) {
	const AController* OwnerController = GetOwnerController();

	if (OwnerController) {
		FVector Location;
		FRotator Rotation;

		OwnerController->GetPlayerViewPoint(Location, Rotation);

		FVector End = Location + Rotation.Vector() * MaxShotRange;

		FCollisionQueryParams Params;

		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		return GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Location,
			End,
			ECC_GameTraceChannel1,
			Params
		);
	}

	return false;
}

void AGun::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (GetOwner() == nullptr) {
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
			AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
			
			if (Character) {
				MeshComponent->SetSimulatePhysics(false);

				bool bSucces = Character->SetGun(this);

				if (bSucces) {
					TriggerComponent->DestroyComponent();
				}

				else {
					MeshComponent->SetSimulatePhysics(true);
				}
			}
		}
	}
}

void AGun::Shoot() {
	if (bCanShoot && !bIsReloading && AvailableAmmo > 0) {
		bCanShoot = false;

		--AvailableAmmo;

		FHitResult HitResult;

		if (SetupLineTrace(HitResult)) {
			AActor* HitActor = HitResult.GetActor();

			if (HitActor->ActorHasTag("Target")) {
				bHasHit = true;

				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, this, &AGun::DisableHasHit, HitTime, false);
			}
		}

		GetWorldTimerManager().SetTimer(DisableHasHitTimerHandle, this, &AGun::EnableShooting, ShootDelay, false);
	}

	else if (AvailableAmmo <= 0) {
		Reload();
	}
}

void AGun::Reload() {
	if (!bIsReloading && ReserveAmmo > 0 && AvailableAmmo < MaxAvailableAmmo) {
		bIsReloading = true;

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AGun::UpdateAmmo, ReloadTime, false);
	}
}

void AGun::UpdateAmmo() {
	int NewReserveAmmo = MaxAvailableAmmo - AvailableAmmo;

	if (ReserveAmmo >= NewReserveAmmo) {
		ReserveAmmo -= NewReserveAmmo;
		AvailableAmmo = MaxAvailableAmmo;
	}

	else {
		AvailableAmmo += ReserveAmmo;
		ReserveAmmo = 0;
	}

	bIsReloading = false;
}

void AGun::EnableShooting() {
	bCanShoot = true;
}

void AGun::DisableHasHit() {
	bHasHit = false;
}

int AGun::GetReserveAmmo() const {
	return ReserveAmmo;
}

int AGun::GetAvailableAmmo() const {
	return AvailableAmmo;
}

UStaticMeshComponent* AGun::GetMesh() const {
	return MeshComponent;
}

bool AGun::GetIsReloading() const {
	return bIsReloading;
}

bool AGun::GetHasHit() const {
	return bHasHit;
}