// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorFallCharMovementComponent.h"
#include "FloorFallCharacterBase.h"



void FSavedMove_FloorFallCharacter::Clear()
{
	FSavedMove_Character::Clear();
	bPressedTackle = false;
}

void FSavedMove_FloorFallCharacter::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	if (C)
	{
		AFloorFallCharacterBase *NewCharacter = Cast<AFloorFallCharacterBase>(C);
		bPressedTackle = NewCharacter->bPressedTackle;
		TackleKeyHoldTime = NewCharacter->TackleKeyHoldTime;
		TackleMaxCount = NewCharacter->TackleMaxCount;

		// CheckJumpInput will increment JumpCurrentCount.
		// Therefore, for replicated moves we want it to set it at 1 less to properly
		// handle the change.
		TackleCurrentCount = NewCharacter->TackleCurrentCount > 0 ? NewCharacter->TackleCurrentCount - 1 : 0;

		//MovementMode = Character->GetCharacterMovement()->PackNetworkMovementMode();
	}
}

bool FSavedMove_FloorFallCharacter::IsImportantMove(const FSavedMovePtr& LastAckedMove) const
{
	const  TSharedPtr<FSavedMove_FloorFallCharacter> FloorFallLastAckedMove = StaticCastSharedPtr<FSavedMove_FloorFallCharacter>(LastAckedMove);
	
	if (bPressedTackle != FloorFallLastAckedMove->bPressedTackle)
		return true;
	else
		return FSavedMove_Character::IsImportantMove(LastAckedMove);
}

bool FSavedMove_FloorFallCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InPawn, float MaxDelta) const
{
	const  TSharedPtr<FSavedMove_FloorFallCharacter> FloorFallNewMove = StaticCastSharedPtr<FSavedMove_FloorFallCharacter>(NewMove);

	if (FSavedMove_Character::CanCombineWith(NewMove, InPawn, MaxDelta))
	{
		if (bPressedTackle != FloorFallNewMove->bPressedTackle)	//if (bPressedTackle || FloorFallNewMove->bPressedTackle)
		{
			return false;
		}
		else 
			return true;
	}
	else
		return false;
}

void FSavedMove_FloorFallCharacter::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	if (C)
	{
		AFloorFallCharacterBase *NewCharacter = Cast<AFloorFallCharacterBase>(C);
		if (NewCharacter)
		{
			NewCharacter->TackleKeyHoldTime = TackleKeyHoldTime;
			NewCharacter->TackleMaxCount = TackleMaxCount;
			NewCharacter->TackleCurrentCount = TackleCurrentCount;
		}
	}
}

uint8 FSavedMove_FloorFallCharacter::GetCompressedFlags() const
{
	uint8 Result = 0;

	if (bPressedJump)
	{
		Result |= FLAG_FF_JumpPressed;
	}

	if (bWantsToCrouch)
	{
		Result |= FLAG_FF_WantsToCrouch;
	}

	if (bPressedTackle)
	{
		Result |= FLAG_FF_TACKLE;
	}
	return Result;
}

bool UFloorFallCharMovementComponent::DoTackle(bool bReplayingMoves)
{
	if (CharacterOwner)
	{
		AFloorFallCharacterBase *NewCharacter = Cast<AFloorFallCharacterBase>(CharacterOwner);
		if (NewCharacter && NewCharacter->CanTackle())
		{
			// Don't jump if we can't move up/down.
			if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
			{
				Velocity.Z = TackleZVelocity;
				SetMovementMode(MOVE_Falling);
				return true;
			}
		}
	}

	

	return false;
}

void UFloorFallCharMovementComponent::TackleOff(AActor* MovementBaseActor)
{
	if (!bPerformingTackleOff)
	{
		bPerformingTackleOff = true;
		if (CharacterOwner)
		{
			AFloorFallCharacterBase *NewCharacter = Cast<AFloorFallCharacterBase>(CharacterOwner);

			if (NewCharacter)
			{
				const float MaxSpeed = GetMaxSpeed() * 0.85f;
				Velocity += MaxSpeed * GetBestDirectionOffActor(MovementBaseActor);
				if (Velocity.Size2D() > MaxSpeed)
				{
					Velocity = MaxSpeed * Velocity.GetSafeNormal();
				}
				Velocity.Z = TackleOffTackleZFactor * JumpZVelocity;
				SetMovementMode(MOVE_Falling);
			}
			bPerformingTackleOff = false;
		}
	}
}

void UFloorFallCharMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	if (CharacterOwner)
	{
		AFloorFallCharacterBase *NewCharacterOwner = Cast<AFloorFallCharacterBase>(CharacterOwner);
		const bool bWasTackle = NewCharacterOwner->bPressedTackle;

		NewCharacterOwner->bPressedTackle = ((Flags & FSavedMove_FloorFallCharacter::FLAG_FF_TACKLE) != 0);

		// Reset JumpKeyHoldTime when player presses Jump key on server as well.
		if (!bWasTackle && NewCharacterOwner->bPressedTackle)
		{
			NewCharacterOwner->bPressedTackle = false;
			NewCharacterOwner->TackleKeyHoldTime = 0.0f;
		}
	}
}
