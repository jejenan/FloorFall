// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorFallCharacterBase.h"
#include "FloorFallCharMovementComponent.h"

AFloorFallCharacterBase::AFloorFallCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFloorFallCharMovementComponent>(ACharacter::CharacterMovementComponentName))
{

}

void AFloorFallCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


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

void AFloorFallCharacterBase::Tackle()
{
	bPressedTackle = true;
	TackleKeyHoldTime = 0.0f;
}

void AFloorFallCharacterBase::StopTackleing()
{
	bPressedTackle = false;
	ResetTackleState();
}

bool AFloorFallCharacterBase::CanTackle() const
{
	return CanTackleInternal();
}

bool AFloorFallCharacterBase::CanTackleInternal_Implementation() const
{
	// Ensure the character isn't currently crouched.
	bool bCanTackle = !bIsCrouched;

	// Ensure that the CharacterMovement state is valid
	UFloorFallCharMovementComponent* CharacterMovement = Cast<UFloorFallCharMovementComponent>(GetCharacterMovement());
	bCanTackle &= CharacterMovement &&
		CharacterMovement->IsJumpAllowed() &&
		!CharacterMovement->bWantsToCrouch &&
		// Can only jump from the ground, or multi-jump if already falling.
		(CharacterMovement->IsMovingOnGround() || CharacterMovement->IsFalling());

	if (bCanTackle)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (GetTackleMaxHoldTime() <= 0.0f || !bWasTackling)
		{
			if (TackleCurrentCount == 0 && CharacterMovement->IsFalling())
			{
				bCanTackle = TackleCurrentCount + 1 < TackleMaxCount;
			}
			else
			{
				bCanTackle = TackleCurrentCount < TackleMaxCount;
			}
		}
		else
		{
			// Only consider IsJumpProviding force as long as:
			// A) The jump limit hasn't been met OR
			// B) The jump limit has been met AND we were already jumping
			bCanTackle = (IsTackleProvidingForce()) &&
				(TackleCurrentCount < TackleMaxCount ||
				(bWasTackling && TackleCurrentCount == TackleMaxCount));
		}
	}

	return bCanTackle;
}

void AFloorFallCharacterBase::ResetTackleState()
{
	bWasTackling = false;
	TackleKeyHoldTime = 0.0f;

	UFloorFallCharMovementComponent* CharacterMovement = Cast<UFloorFallCharMovementComponent>(GetCharacterMovement());
	if (CharacterMovement && !CharacterMovement->IsFalling())
	{
		TackleCurrentCount = 0;
	}
}

bool AFloorFallCharacterBase::IsTackleProvidingForce() const
{
	return (bPressedTackle && TackleKeyHoldTime < GetTackleMaxHoldTime());
}

void AFloorFallCharacterBase::OnTackled_Implementation()
{

}

void AFloorFallCharacterBase::CheckTackleInput(float DeltaTime)
{

	UFloorFallCharMovementComponent* CharacterMovement = Cast<UFloorFallCharMovementComponent>(GetCharacterMovement());
	if (CharacterMovement)
	{
		if (bPressedTackle)
		{
			// If this is the first jump and we're already falling,
			// then increment the JumpCount to compensate.
			const bool bFirstTackle = TackleCurrentCount == 0;
			if (bFirstTackle && CharacterMovement->IsFalling())
			{
				TackleCurrentCount++;
			}

			const bool bDidTackle = CanTackle() && CharacterMovement->DoTackle(bClientUpdating);
			if (bDidTackle)
			{
				// Transistion from not (actively) jumping to jumping.
				if (!bWasTackling)
				{
					TackleCurrentCount++;
					OnTackled();
				}
				// Only increment the jump time if successfully jumped and it's
				// the first jump. This prevents including the initial DeltaTime
				// for the first frame of a jump.
				if (!bFirstTackle)
				{
					TackleKeyHoldTime += DeltaTime;
				}
			}

			bWasTackling = bDidTackle;
		}

		// If the jump key is no longer pressed and the character is no longer falling,
		// but it still "looks" like the character was jumping, reset the counters.
		else if (bWasTackling && !CharacterMovement->IsFalling())
		{
			ResetTackleState();
		}
	}
}

void AFloorFallCharacterBase::ClearTackleInput()
{
	// Don't disable bPressedJump right away if it's still held
	if (bPressedTackle && (TackleKeyHoldTime >= GetTackleMaxHoldTime()))
	{
		bPressedTackle = false;
	}
}

float AFloorFallCharacterBase::GetTackleMaxHoldTime() const
{
	return TackleMaxHoldTime;
}

