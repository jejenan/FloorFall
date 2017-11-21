// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorFallGM.h"
#include "FloorFall.h"

#include "Blueprint/UserWidget.h"


AFloorFallGM::AFloorFallGM()
{
// 	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
// 	if (PlayerPawnBPClass.Class != NULL)
// 	{
// 		DefaultPawnClass = PlayerPawnBPClass.Class;
// 	}
	PrimaryActorTick.bCanEverTick = true;

}

void AFloorFallGM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AFloorFallGM::BeginPlay()
{
	Super::BeginPlay();

// 	if (HUDWidgetClass != nullptr)
// 	{
// 		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
// 		if (CurrentWidget != nullptr)
// 		{
// 			CurrentWidget->AddToViewport();
// 		}
// 	}

}

EFloorFallPlayState AFloorFallGM::GetCurrentState() const
{
	return CurrentState;
}

void AFloorFallGM::SetCurrentState(EFloorFallPlayState val)
{
	CurrentState = val;
}

void AFloorFallGM::HandleNewState(EFloorFallPlayState NewState)
{

}
