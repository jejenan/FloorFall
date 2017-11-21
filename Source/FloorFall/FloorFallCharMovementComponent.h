// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FloorFallCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FLOORFALL_API UFloorFallCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	/**
	*	Apply gravity while the character is actively Tackleing (e.g. holding the Tackle key).
	*	Helps remove frame-rate dependent Tackle height, but may alter base jump height.
	*/
	UPROPERTY(Category = "Character Movement: Tackling", EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	uint32 bApplyGravityWhileTackleing : 1;

	/** Initial velocity (instantaneous vertical acceleration) when jumping. */
	UPROPERTY(Category = "Character Movement: Tackling", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Tackle Z Velocity", ClampMin = "0", UIMin = "0"))
	float TackleZVelocity;

	/** Fraction of TackleZVelocity to use when automatically "Tackleing off" of a base actor that's not allowed to be a base for a character. (For example, if you're not allowed to stand on other players.) */
	UPROPERTY(Category = "Character Movement: Tackling ", EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (ClampMin = "0", UIMin = "0"))
	float TackleOffTackleZFactor;

	/** Used to prevent reentry of JumpOff() */
	UPROPERTY()
	uint32 bPerformingTackleOff : 1;
	/**
	* Perform jump. Called by Character when a jump has been detected because Character->bPressedJump was true. Checks CanJump().
	* Note that you should usually trigger a jump through Character::Jump() instead.
	* @param	bReplayingMoves: true if this is being done as part of replaying moves on a locally controlled client after a server correction.
	* @return	True if the jump was triggered successfully.
	*/
	virtual bool DoTackle(bool bReplayingMoves);


	/** Force this pawn to bounce off its current base, which isn't an acceptable base for it. */
	virtual void TackleOff(AActor* MovementBaseActor);

protected:
	/** Unpack compressed flags from a saved move and set state accordingly. See FSavedMove_Character. */
	virtual void UpdateFromCompressedFlags(uint8 Flags);

};


/** FSavedMove_FloorFallCharacter represents a saved move on the client that has been sent to the server and might need to be played back. */
class FLOORFALL_API FSavedMove_FloorFallCharacter : public FSavedMove_Character
{
public:
// 	FSavedMove_FloorFallCharacter();
// 	virtual ~FSavedMove_FloorFallCharacter();

	uint32 bPressedTackle : 1;	

	float TackleKeyHoldTime;
	int32 TackleMaxCount;
	int32 TackleCurrentCount;
	
	/** Clear saved move properties, so it can be re-used. */
	virtual void Clear();

	/** Called to set up this saved move (when initially created) to make a predictive correction. */
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData);

// 	/** Set the properties describing the position, etc. of the moved pawn at the start of the move. */
// 	virtual void SetInitialPosition(ACharacter* C);

	/** @Return true if this move is an "important" move that should be sent again if not acked by the server */
	virtual bool IsImportantMove(const FSavedMovePtr& LastAckedMove) const;

// 	/** Returns starting position if we were to revert the move, either absolute StartLocation, or StartRelativeLocation offset from MovementBase's current location (since we want to try to move forward at this time). */
// 	virtual FVector GetRevertedLocation() const;
// 
// 	/** Set the properties describing the final position, etc. of the moved pawn. */
// 	virtual void PostUpdate(ACharacter* C, EPostUpdateMode PostUpdateMode);

	/** @Return true if this move can be combined with NewMove for replication without changing any behavior */
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InPawn, float MaxDelta) const;

	/** Called before ClientUpdatePosition uses this SavedMove to make a predictive correction	 */
	virtual void PrepMoveFor(ACharacter* C);

	/** @returns a byte containing encoded special movement information (jumping, crouching, etc.)	 */
	virtual uint8 GetCompressedFlags() const;

	// Bit masks used by GetCompressedFlags() to encode movement information.
	enum CompressedFlagsFloorFall
	{
		FLAG_FF_JumpPressed = 0x01,	// Jump pressed
		FLAG_FF_WantsToCrouch = 0x02,	// Wants to crouch
		FLAG_FF_TACKLE	= 0x04,	// Reserved for future use
		FLAG_FF_Reserved_2 = 0x08,	// Reserved for future use
		// Remaining bit masks are available for custom flags.
		FLAG_FF_Custom_0 = 0x10,
		FLAG_FF_Custom_1 = 0x20,
		FLAG_FF_Custom_2 = 0x40,
		FLAG_FF_Custom_3 = 0x80,
	};
};
