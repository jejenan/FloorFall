// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FloorFallCharacterBase.generated.h"

UCLASS()
class FLOORFALL_API AFloorFallCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	AFloorFallCharacterBase(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
	

	/** When true, player wants to jump */
	UPROPERTY(BlueprintReadOnly, Category = "Pawn|Character")
	uint32 bPressedTackle : 1;
	/**
	* Tackle key Held Time.
	* This is the time that the player has held the Tackle key, in seconds.
	*/
	UPROPERTY(Transient, BlueprintReadOnly, VisibleInstanceOnly, Category = Character)
	float TackleKeyHoldTime;

	/**
	* The max time the Tackle key can be held.
	* Note that if StopTackleing() is not called before the max Tackle hold time is reached,
	* then the character will carry on receiving vertical velocity. Therefore it is usually
	* best to call StopTackleing() when Tackle input has ceased (such as a button up event).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Character, Meta = (ClampMin = 0.0, UIMin = 0.0))
	float TackleMaxHoldTime;

	/**
	* The max number of Tackles the character can perform.
	* Note that if TackleMaxHoldTime is non zero and StopTackleing is not called, the player
	* may be able to perform and unlimited number of Tackles. Therefore it is usually
	* best to call StopTackleing() when Tackle input has ceased (such as a button up event).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Character)
	int32 TackleMaxCount;

	/**
	* Tracks the current number of Tackles performed.
	* This is incremented in CheckTackleInput, used in CanTackle_Implementation, and reset in OnMovementModeChanged.
	* When providing overrides for these methods, it's recommended to either manually
	* increment / reset this value, or call the Super:: method.
	*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Character")
	int32 TackleCurrentCount;

	DEPRECATED(4.14, "This value is no longer used.")
	uint32 bTackleMaxCountExceeded : 1;

	// Tracks whether or not the character was already Tackleing last frame.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = "Character")
	uint32 bWasTackling : 1;



	/**
	* Make the character Tackle on the next update.
	* If you want your character to Tackle according to the time that the Tackle key is held,
	* then you can set TackleKeyHoldTime to some non-zero value. Make sure in this case to
	* call StopTackleing() when you want the Tackle's z-velocity to stop being applied (such
	* as on a button up event), otherwise the character will carry on receiving the
	* velocity until TackleKeyHoldTime is reached.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Character")
	virtual void Tackle();

	/**
	* Stop the character from Tackleing on the next update.
	* Call this from an input event (such as a button 'up' event) to cease applying
	* Tackle Z-velocity. If this is not called, then Tackle z-velocity will be applied
	* until TackleMaxHoldTime is reached.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Character")
	virtual void StopTackleing();

	/**
	* Check if the character can Tackle in the current state.
	*
	* The default implementation may be overridden or extended by implementing the custom CanTackle event in Blueprints.
	*
	* @Return Whether the character can Tackle in the current state.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Character")
	bool CanTackle() const;


protected:

	/**
	* Customizable event to check if the character can Tackle in the current state.
	* Default implementation returns true if the character is on the ground and not crouching,
	* has a valid CharacterMovementComponent and CanEverTackle() returns true.
	* Default implementation also allows for 'hold to Tackle higher' functionality:
	* As well as returning true when on the ground, it also returns true when GetMaxTackleTime is more
	* than zero and IsTackleing returns true.
	*
	*
	* @Return Whether the character can Tackle in the current state.
	*/

	UFUNCTION(BlueprintNativeEvent, Category = "Pawn|Character", meta = (DisplayName = "CanTackle"))
	bool CanTackleInternal() const;
	virtual bool CanTackleInternal_Implementation() const;
// 
// 	DEPRECATED(4.14, "This function is deprecated. Please use ResetTackleState instead.")
// 		void CheckResetTackleCount()
// 	{
// 		ResetTackleState();
// 	}
// 
	void ResetTackleState();
public:
/**
* True if Tackle is actively providing a force, such as when the Tackle key is held and the time it has been held is less than TackleMaxHoldTime.
* @see CharacterMovement->IsFalling
*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Character")
	virtual bool IsTackleProvidingForce() const;

	/** Event fired when the character has just started Tackleing */
	UFUNCTION(BlueprintNativeEvent, Category = "Pawn|Character")
	void OnTackled();
	virtual void OnTackled_Implementation();

	/** Trigger Tackle if Tackle button has been pressed. */
	virtual void CheckTackleInput(float DeltaTime);

	/** Reset Tackle input state after having checked input. */
	virtual void ClearTackleInput();

	/**
	* Get the maximum Tackle time for the character.
	* Note that if StopTackleing() is not called before the max Tackle hold time is reached,
	* then the character will carry on receiving vertical velocity. Therefore it is usually
	* best to call StopTackleing() when Tackle input has ceased (such as a button up event).
	*
	* @return Maximum Tackle time for the character
	*/
	virtual float GetTackleMaxHoldTime() const;
};
