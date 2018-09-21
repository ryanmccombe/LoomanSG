// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"
#include "Engine/World.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatingPawn, bool bMissionSuccess) {
	if (InstigatingPawn) {
		InstigatingPawn->DisableInput(nullptr);
	}

	auto GS = GetGameState<AFPSGameState>();

	if (GS) {
		GS->MulticastOnMissionComplete(InstigatingPawn, bMissionSuccess);
	}

	OnMissionCompleted(InstigatingPawn, bMissionSuccess);
	SwitchToSpectatorCam(InstigatingPawn);
}

void AFPSGameMode::SwitchToSpectatorCam(APawn* InstigatingPawn) const {
	if (!ensure(SpectatingViewpointClass)) { return; }

	TArray<AActor*> ReturnedActors;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
	if (ReturnedActors.Num() > 0) {
		const auto NewViewTarget = ReturnedActors[0];

		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
			APlayerController* PC = It->Get();
			if (PC) {
				PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
	}
}
