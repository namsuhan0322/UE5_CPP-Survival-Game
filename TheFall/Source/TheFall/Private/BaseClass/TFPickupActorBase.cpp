// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFPickupActorBase.h"

ATFPickupActorBase::ATFPickupActorBase()
{
	WorldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("World Mesh"));
	WorldMesh->SetupAttachment(RootComponent);
}

FText ATFPickupActorBase::GetInteractionText_Implementation()
{
	return InteractionText;
}

void ATFPickupActorBase::Interact_Implementation(class ATFCharacter* Caller)
{

}

bool ATFPickupActorBase::IsInteracable_Implementation() const
{
	return true;
}