// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TFTreeBase.h"
#include "BaseClass/TFPickupActorBase.h"
#include "Engine/DamageEvents.h"

ATFTreeBase::ATFTreeBase()
{
	TreeStumpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Trump Mesh"));
	TreeStumpMesh->SetupAttachment(RootComponent);
	TreeStumpMesh->bHiddenInGame = true;
	TreeStumpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MainTreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Tree Mesh"));
	MainTreeMesh->SetupAttachment(TreeStumpMesh);
	TreeStumpMesh->bHiddenInGame = false;
	TreeStumpMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
	
void ATFTreeBase::SetHarvestState()
{
	if (!bIsHarvested) return;

	MainTreeMesh->DestroyComponent();
	TreeStumpMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TreeStumpMesh->bHiddenInGame = false;
	TreeStumpMesh->SetVisibility(true, true);
	MarkComponentsRenderStateDirty();
}

void ATFTreeBase::Harvest()
{
	bIsHarvested = true;
	SetHarvestState();
	SpawnPickups();
	OnHarvestedBP();
}

void ATFTreeBase::SpawnPickups()
{
	if (!IsValid(LogPickupActor)) return;

	for (int i = 0; i < NumberOfLogsToSpawn; i++)
	{
		FVector TreeLoc = this->GetActorLocation();
		FTransform SpawnTrans = SpawnActorTransforms[i];
		SpawnTrans.SetLocation(SpawnTrans.GetLocation() + TreeLoc);
		ATFPickupActorBase* Log = GetWorld()->SpawnActor<ATFPickupActorBase>(LogPickupActor, SpawnTrans);
		if (IsValid(Log))
		{
			Log->SetActorTransform(SpawnTrans);
			OnHarvestedBP();
		}
	}
}

void ATFTreeBase::OnHarvestedBP_Implementation()
{
	OnHarvestedBP();
}

FSaveActorData ATFTreeBase::GetSaveData_Implementation()
{
	FSaveActorData Ret;
	Ret.ActorTransform = this->GetActorTransform();
	Ret.ActorClass = this->GetClass();
	Ret.WasSpawned = this->bWasSpawned;
	return Ret;
}

void ATFTreeBase::UpdateFromSave_Implementation()
{
	SetHarvestState();
}

float ATFTreeBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsHarvested || !DamageCauser->Tags.Contains("HarvestTree")) return 0.0f;

	Health -= DamageAmount;
	if (Health > 0.0f) return 0.0f;

	Harvest();
	return 0.0f;
}