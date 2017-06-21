// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "FloorFallGM.generated.h"


UENUM(BlueprintType)
enum class EFloorFallPlayState : uint8
{
	EPlaying,
	ELose,
	EWon,
	EUnknown
};

/**
 * 
 */
UCLASS()
class FLOORFALL_API AFloorFallGM : public AGameMode
{
	GENERATED_BODY()
	

public:
	AFloorFallGM();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	//returns the current playing state
	UFUNCTION(BlueprintPure, Category = "FloorFall")
	EFloorFallPlayState GetCurrentState() const;
	//Sets a new playing state
	void SetCurrentState(EFloorFallPlayState val);

protected:

	//The widget class to use for our HUD screen
 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FloorFall", Meta = (BlueprintProtected = "true"))
 	TSubclassOf<class UUserWidget> HUDWidgetClass;
// 	//The instance of the HUD
 	UPROPERTY()
 	class UUserWidget* CurrentWidget;

private:
	//Keeps track of the current playing state
	UPROPERTY()
	EFloorFallPlayState CurrentState;


	//Handle any function calls that rely upon changing the playing state of our game
	void HandleNewState(EFloorFallPlayState NewState);
};
