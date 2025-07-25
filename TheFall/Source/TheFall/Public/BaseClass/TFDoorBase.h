// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/TFActor.h"
#include "Interface/InteractionInterface.h"
#include "TFDoorBase.generated.h"

/**
 * 
 */
UCLASS()
class THEFALL_API ATFDoorBase : public ATFActor, public IInteractionInterface
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, meta = (AllowPrivateAccess = "true"))
	bool bIsOpen = false;

public:
	ATFDoorBase();

	FText GetInteractionText_Implementation();
	void Interact_Implementation(class ATFCharacter* Caller);
	bool IsInteracable_Implementation() const;
};
