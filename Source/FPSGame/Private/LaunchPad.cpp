// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"


// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.f));
	RootComponent = OverlapComp;

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::HandleOverlap);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("UArrowComponent"));
	ArrowComp->SetupAttachment(OverlapComp);
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	FVector Impulse = ArrowComp->GetForwardVector() * LaunchForce;

	auto PhysicsComponent = Cast<UStaticMeshComponent>(OtherComponent);
	if (PhysicsComponent) {
		PhysicsComponent->AddImpulse(Impulse, FName(), true);
		PlayEffects();
		return;
	}

	auto Character = Cast<ACharacter>(OtherActor);
	if (Character) {
		Character->LaunchCharacter(Impulse, true, true);
		PlayEffects();
	}
}

void ALaunchPad::PlayEffects() {
	if (!LaunchFX) { return; }
	UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
}
