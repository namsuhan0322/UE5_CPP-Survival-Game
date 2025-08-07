// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFRegrowingHarvestActorBase.h"

ATFRegrowingHarvestActorBase::ATFRegrowingHarvestActorBase()
{

}

void ATFRegrowingHarvestActorBase::ResetHarvest()
{
	bIsHarvested = false;
	ItemCount = ItemRegrowthAmount;
	HarvestMesh->bHiddenInGame = false;
	MarkComponentsRenderStateDirty();
}

void ATFRegrowingHarvestActorBase::BeginPlay()
{

}

void ATFRegrowingHarvestActorBase::OnDayChange()
{
	DaysSinceLastHarvest++;
	if (DaysSinceLastHarvest < NumberOfDaysToRegrow)
	{
		return;
	}

	ResetHarvest();
}