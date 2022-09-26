// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class USphereComponent;

UCLASS()
class TESTTASKSHOOTER_API AGun : public AActor {
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGun();

private:
	void SetupAmmo();

	bool SetupLineTrace(FHitResult& HitResult);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bIsReloading;

	void UpdateAmmo();
	void EnableShooting();

	FTimerHandle DisableHasHitTimerHandle;
	void DisableHasHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* TriggerComponent;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int AmmoCount;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int MaxAvailableAmmo;

	int ReserveAmmo;
	int AvailableAmmo;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float MaxShotRange;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float ShootDelay;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float HitTime;

	UPROPERTY(EditAnywhere, Category = "Reloading")
	float ReloadTime;

	bool bCanShoot;
	bool bHasHit;

public:
	const AController* GetOwnerController() const;

	virtual void Shoot();
	void Reload();

	UFUNCTION(BlueprintCallable)
	int GetReserveAmmo() const;

	UFUNCTION(BlueprintCallable)
	int GetAvailableAmmo() const;

	UStaticMeshComponent* GetMesh() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsReloading() const;

	UFUNCTION(BlueprintCallable)
	bool GetHasHit() const;
};
