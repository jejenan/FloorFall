// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorFallCharacterBase.h"
#include "FloorFallCharMovementComponent.h"

AFloorFallCharacterBase::AFloorFallCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFloorFallCharMovementComponent>(ACharacter::CharacterMovementComponentName))
{

}

// Called when the game starts or when spawned
void AFloorFallCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloorFallCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFloorFallCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

