// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestTaskShooterCharacter.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class TESTTASKSHOOTER_API AShooterCharacter : public ATestTaskShooterCharacter {
	GENERATED_BODY()
	
public:
	AShooterCharacter();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	void PullTheTrigger();
	void ReloadMagazine();

	AGun* Gun;

public:
	bool SetGun(AGun* NewGun);

	UFUNCTION(BlueprintCallable)
	const AGun* GetGun() const;

	UFUNCTION(BlueprintImplementableEvent)
	void AddGunHUD();
};