// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/InteractionInterface.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.

FText IInteractionInterface::GetInteractionText_Implementation()
{
	return FText();
}

void IInteractionInterface::Interact_Implementation(ATFCharacter* Caller)
{
	Interact(Caller);
}

bool IInteractionInterface::IsInteracable_Implementation() const
{
	return true;
}
