// Fill out your copyright notice in the Description page of Project Settings.
#include "FloorFall.h"
#include "LevelMaker.h"
#include "Components/BoxComponent.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALevelMaker::ALevelMaker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MapArea = CreateDefaultSubobject<UBoxComponent>(_T("MapArea"));
	RootComponent = MapArea;

	NumOfFloorX = 30;
	NumOfFloorY = 30;

}

// Called when the game starts or when spawned
void ALevelMaker::BeginPlay()
{
	Super::BeginPlay();

	ResetFloorState(FloorState::white);
	CreateMap();
}

// Called every frame
void ALevelMaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelMaker::SetFloorState(int x, int y, FloorState val)
{
	FloorMap[std::make_pair(x, y)] = val;
}

void ALevelMaker::ResetFloorState(FloorState floorState)
{

	for (int j = 0; j < NumOfFloorY; j++)
	{
		for (int i = 0; i < NumOfFloorX; i++)
		{
			FloorMap.emplace(std::make_pair(j, i), floorState);
		}

	}
}

void ALevelMaker::CreateMap()
{
	//if we have set something to spawn;
	if (WhatToSpawn != NULL)
	{
		//check for a valid world
		UWorld* const World = GetWorld();
		if (World)
		{			
			//Floor Size 계산
			FVector TargetFloorExtent(MapArea->Bounds.BoxExtent.X / NumOfFloorX,
				MapArea->Bounds.BoxExtent.Y / NumOfFloorY, 
				MapArea->Bounds.BoxExtent.Z);

			int NumOfMax = FMath::Max<int>(NumOfFloorX, NumOfFloorY);
			for (auto& floor : FloorMap)
			{
				int y = floor.first.first;
				int x = floor.first.second;
				//floor.second

				//set the spawn parameters
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				SpawnParams.Name = FName(WhatToSpawn->GetFName(), x + y*NumOfMax);
				//Get a location to spawn at
				FVector SpawnLocation( 0.f, 0.f, 0.f);

				//Get a random rotation for the spawned item
				FRotator SpawnRotation;
				SpawnRotation.Yaw = 0.f;//*360.0f;
				SpawnRotation.Pitch = 0.f; //FMath::FRand()*360.0f;
				SpawnRotation.Roll = 0.f; //FMath::FRand()*360.0f;

				//Spawn 위치 계산
				SpawnLocation.X = MapArea->Bounds.Origin.X - MapArea->Bounds.BoxExtent.X + TargetFloorExtent.X*2*x;
				SpawnLocation.Y = MapArea->Bounds.Origin.Y - MapArea->Bounds.BoxExtent.Y + TargetFloorExtent.Y*2*y;
				SpawnLocation.Z = MapArea->Bounds.Origin.Z;

				//spawn the pickup
				AActor* const SpawnedFloor = World->SpawnActor<AActor>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

				//Floor mesh Scale 계산
				FVector FloorOrigin;
				FVector FloorBoundsExtent;
				SpawnedFloor->GetActorBounds(true, FloorOrigin, FloorBoundsExtent);		
				FVector RelativeScale = (TargetFloorExtent-2.f) / FloorBoundsExtent;

				//Set the Floor Scale 
				SpawnedFloor->SetActorRelativeScale3D(RelativeScale);

				// NumOfFloorX ,NumOfFloorY에 맞춰서 Floor 크기 조절		

				FVector Origin;
				FVector BoundsExtent;
				SpawnedFloor->GetActorBounds(true, Origin, BoundsExtent);
				UE_LOG(FloorFallDebug, Log, _T("TargetFloorOrigin: %s, TargetFloorSize: %s"), *SpawnLocation.ToString(), *TargetFloorExtent.ToString());
				UE_LOG(FloorFallDebug, Log, _T("ActorBounds Origin: %s, Extends: %s"), *Origin.ToString(), *BoundsExtent.ToString());
// 					UE_LOG(FloorFallDebug, Log, _T("ActorScale3D: %s"), *SpawnedFloor->GetActorScale3D().ToString());
			

			
			}
		}
	}
}

