// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

class UBoxComponent;
class USoundBase;
class UParticleSystem;
class UArrowComponent;

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* ArrowComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ObjectiveMissingSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* LaunchFX = nullptr;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float LaunchForce = 2000.f;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                   const FHitResult& SweepResult);

	void PlayEffects();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
