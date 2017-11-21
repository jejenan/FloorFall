// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorFallGameState.h"




AFloorFallGameState::AFloorFallGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	RemainTime = LimitTime = 60.f;

		
}

void AFloorFallGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RemainTime -= DeltaSeconds;
}
