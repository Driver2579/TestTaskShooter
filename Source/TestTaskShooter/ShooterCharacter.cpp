// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"

#include "Gun.h"
#include "Misc/OutputDeviceNull.h"

AShooterCharacter::AShooterCharacter() {
	bUseControllerRotationYaw = true;

}

void AShooterCharacter::BeginPlay() {
	Super::BeginPlay();

}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShooterCharacter::PullTheTrigger);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::ReloadMagazine);
}

void AShooterCharacter::PullTheTrigger() {
	if (Gun) {
		Gun->Shoot();
	}
}

void AShooterCharacter::ReloadMagazine() {
	if (Gun) {
		Gun->Reload();
	}
}

bool AShooterCharacter::SetGun(AGun* NewGun) {
	bool bSucces = false;

	if (Gun) return bSucces;

	Gun = NewGun;

	if (Gun) {
		UStaticMeshComponent* GunMesh = Gun->GetMesh();

		bSucces = GunMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		
		if (bSucces) {
			Gun->SetOwner(this);
			GunMesh->SetSimulatePhysics(false);
			
			AddGunHUD();
		}
	}

	return bSucces;
}

const AGun* AShooterCharacter::GetGun() const {
	return Gun;
}