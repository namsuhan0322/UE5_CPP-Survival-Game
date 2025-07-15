// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEFALL_API IInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetInteractionText();
	FText GetInteractionText_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Interact(class ATFCharacter* Caller);
	void Interact_Implementation(class ATFCharacter* Caller); 
	UFUNCTION(BlueprintNativeEvent)
	bool IsInteracable() const;
	bool IsInteracable_Implementation() const;
};
