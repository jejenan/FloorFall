// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelMaker.generated.h"

UENUM(BlueprintType)
enum class FloorState : uint8
{
	empty, white, yellow, red
};


UCLASS()
class FLOORFALL_API ALevelMaker : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ALevelMaker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	int NumOfFloorX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	int NumOfFloorY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TSubclassOf<class AFloor> WhatToSpawn;	

	UFUNCTION(BlueprintCallable, Category = "Map")
	void SetFloorState(int x, int y, FloorState val);
	UFUNCTION(BlueprintCallable, Category = "Map")
	void ResetAllFloorState(FloorState floorState);
	UFUNCTION(BlueprintCallable, Category = "Map")
	void CreateMap();
	UFUNCTION(BlueprintCallable, Category = "Map")
	void ClearMap();
private:
	using Map2D = std::map<std::pair<int, int>, FloorState>;
	Map2D FloorMap;

	//Box Component to specify where pickups should be spawned 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* MapArea;
};
