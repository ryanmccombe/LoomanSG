// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatingPawn, bool bMissionSuccess) {
	if (InstigatingPawn) {
		InstigatingPawn->DisableInput(nullptr);
	}

	OnMissionCompleted(InstigatingPawn, bMissionSuccess);
	SwitchToSpectatorCam(InstigatingPawn);
}

void AFPSGameMode::SwitchToSpectatorCam(APawn* InstigatingPawn) const {
	if (!SpectatingViewpointClass) {
		UE_LOG(LogTemp, Warning, TEXT("Spectating viewpoint class is nullptr"))
			return;
	}

	TArray<AActor*> ReturnedActors;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
	if (ReturnedActors.Num() > 0) {
		const auto NewViewTarget = ReturnedActors[0];
		APlayerController* PC = Cast<APlayerController>(InstigatingPawn->GetController());
		if (PC) {
			PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}
}
