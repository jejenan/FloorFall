// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FloorFallGameState.generated.h"


UCLASS()
class FLOORFALL_API AFloorFallGameState : public AGameState
{
	GENERATED_BODY()

public:
	AFloorFallGameState();
	

	//The widget class to use for our HUD screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorFall")
	float RemainTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorFall")
	float LimitTime;

	
};
