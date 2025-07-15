// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TFGameInstance.h"
#include "Game/TFSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>
#include "Logger.h"

// ������ (�ʱ�ȭ ó�� ����)
UTFGameInstance::UTFGameInstance() {}

// ���� ���� ���� (�� SaveGame ��ü ����)
void UTFGameInstance::CreatSaveslot()
{
	SaveGameObject = Cast<UTFSaveGame>(UGameplayStatics::CreateSaveGameObject(UTFSaveGame::StaticClass()));
}

// ���� �� ��� ���� ������ ���͵��� ������ ����
void UTFGameInstance::GetherActorData()
{
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		// ��ȿ���� �ʰų� ���� �������̽��� �������� ���� ���ʹ� ����
		if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
		{
			continue;
		}

		ISaveActorInterface* Inter = Cast<ISaveActorInterface>(Actor);
		if (Inter == nullptr)
		{
			continue;
		}

		// ���� �ĺ��� ������ ����
		FGuid SAI = Inter->GetActorSaveID_Implementation();
		if (!SAI.IsValid())
		{
			continue;
		}

		// ������ ���� ������ ��������
		FSaveActorData SAD = Inter->GetSaveData_Implementation();

		// ���� ��ü ����ȭ (���� ����)
		FMemoryWriter MemWriter(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		// ������ ������Ʈ�鵵 ����
		for (auto ActorComp : Actor->GetComponents())
		{
			if (!ActorComp->Implements<USaveActorInterface>())
			{
				continue;
			}

			ISaveActorInterface* CompInter = Cast<ISaveActorInterface>(ActorComp);
			if (CompInter == nullptr)
			{
				continue;
			}

			FSaveComponentData SCD = CompInter->GetComponentSaveData_Implementation();
			FMemoryWriter CompMemWriter(SCD.ByteData);
			FObjectAndNameAsStringProxyArchive CAr(CompMemWriter, true);
			CAr.ArIsSaveGame = true;
			ActorComp->Serialize(CAr);
			SCD.ComponentClass = ActorComp->GetClass();

			SAD.ComponentData.Add(SCD);
		}

		// ���������� ������ ���� �ʿ� �߰�
		SaveableActorData.Add(SAI, SAD);
	}

	// �÷��̾� ���� �����͵� ����
	GetherPlayerData();
}

// ����� �����ͷκ��� ���� �ε�
void UTFGameInstance::LoadGame()
{
	// ����� ������ ���� ��� ��� �α�
	if (!UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
	{
		Logger::GetInstance()->AddMessage("Load game called with Invalid save game", ERRORLEVEL::EL_WARNING);
		return;
	}

	// ���� ������ Ŭ���� �� �ε�
	SaveableActorData.Empty();
	SaveGameObject = Cast<UTFSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0));
	SaveableActorData = SaveGameObject->GetSaveActorData();
	PlayerData = SaveGameObject->GetPlayerData();

	// ����� ���� �� ������ ���ʹ� �ٽ� ����
	for (TTuple<FGuid, FSaveActorData> SAD : SaveableActorData)
	{
		if (SAD.Value.WasSpawned)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor(SAD.Value.ActorClass->StaticClass(), &SAD.Value.ActorTransform);
			ISaveActorInterface* Inter = Cast<ISaveActorInterface>(SpawnedActor);
			if (Inter == nullptr)
			{
				continue;
			}
			Inter->SetActorGUID(SAD.Key);
		}
	}

	// ���� �� ���� ���� ����
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
		{
			continue;
		}

		ISaveActorInterface* Inter = Cast<ISaveActorInterface>(Actor);
		if (Inter == nullptr)
		{
			continue;
		}

		FGuid SAI = Inter->GetActorSaveID_Implementation();
		if (!SaveableActorData.Find(SAI))
		{
			continue;
		}

		FSaveActorData SAD = SaveableActorData[SAI];
		Actor->SetActorTransform(SAD.ActorTransform);

		// ���� ����
		FMemoryReader MemReader(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		Inter->Execute_UpdateFromSave(Actor);
		// ������Ʈ ����
		for (auto ActorComp : Actor->GetComponents())
		{
			if (!ActorComp->Implements<USaveActorInterface>())
			{
				continue;
			}
			ISaveActorInterface* CompInter = Cast<ISaveActorInterface>(ActorComp);
			if (CompInter == nullptr)
			{
				continue;
			}

			for (auto SCD : SAD.ComponentData)
			{
				/****************************************************************************
				* ������ Ŭ������ ������Ʈ�� ���� �� �ִ� ��쿣 �� ����� ������.
				* ù ��° ��ġ�ϴ� ������Ʈ�� ��� �����͸� �ް� ��.
				* �����ϰ� �Ϸ��� ������Ʈ���� GUID�� �ο��ؼ� ��Ȯ�ϰ� �ĺ��ؾ� ��.
				*****************************************************************************/

				if (SCD.ComponentClass != ActorComp->GetClass())
				{
					continue;
				}

				FMemoryReader CompMemReader(SCD.ByteData);
				FObjectAndNameAsStringProxyArchive CAr(CompMemReader, true);
				CAr.ArIsSaveGame = true;
				ActorComp->Serialize(CAr);

				if (SCD.RawData.IsEmpty())
				{
					break;
				}

				CompInter->SetComponentSaveData_Implementation(SCD);
				break;
			}
		}
	}

	// �÷��̾� ���� ����
	SetPlayerData();
}

// �÷��̾� ĳ������ ������ ����
void UTFGameInstance::GetherPlayerData()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ISaveActorInterface* Inter = Cast<ISaveActorInterface>(PlayerCharacter);
	if (Inter == nullptr)
	{
		// ���� �α� ����
		return;
	}

	FSaveActorData SAD = Inter->GetSaveData_Implementation();

	// �÷��̾� ����ȭ
	FMemoryWriter MemWriter(SAD.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	PlayerCharacter->Serialize(Ar);

	// ������Ʈ ����
	for (auto ActorComp : PlayerCharacter->GetComponents())
	{
		if (!ActorComp->Implements<USaveActorInterface>())
		{
			continue;
		}

		ISaveActorInterface* CompInter = Cast<ISaveActorInterface>(ActorComp);
		if (CompInter == nullptr)
		{
			continue;
		}

		FSaveComponentData SCD = CompInter->GetComponentSaveData_Implementation();
		FMemoryWriter CompMemWriter(SCD.ByteData);
		FObjectAndNameAsStringProxyArchive CAr(CompMemWriter, true);
		CAr.ArIsSaveGame = true;
		ActorComp->Serialize(CAr);
		SCD.ComponentClass = ActorComp->GetClass();

		SAD.ComponentData.Add(SCD);
	}
	PlayerData = SAD;
}

// ����� �����ͷ� �÷��̾� ���� ����
void UTFGameInstance::SetPlayerData()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ISaveActorInterface* Inter = Cast<ISaveActorInterface>(PlayerCharacter);
	if (Inter == nullptr)
	{
		// Log Error
		return;
	}

	PlayerCharacter->SetActorTransform(PlayerData.ActorTransform);

	FMemoryReader PCMemReader(PlayerData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(PCMemReader, true);
	Ar.ArIsSaveGame = true;
	PlayerCharacter->Serialize(Ar);

	for (auto ActorComp : PlayerCharacter->GetComponents())
	{
		if (!ActorComp->Implements<USaveActorInterface>())
		{
			continue;
		}
		ISaveActorInterface* CompInter = Cast<ISaveActorInterface>(ActorComp);
		if (CompInter == nullptr)
		{
			continue;
		}

		for (auto SCD : PlayerData.ComponentData)
		{
			// ���� Ŭ���� ������Ʈ ���� �� ���� (�� ����� ����)
			if (SCD.ComponentClass != ActorComp->GetClass())
			{
				continue;
			}

			FMemoryReader CompMemReader(SCD.ByteData);
			FObjectAndNameAsStringProxyArchive CAr(CompMemReader, true);
			CAr.ArIsSaveGame = true;
			ActorComp->Serialize(CAr);

			if (SCD.RawData.IsEmpty())
			{
				break;
			}

			CompInter->SetComponentSaveData_Implementation(SCD);
			break;
		}
	}
}

// �ܺο��� ������ ���� �����͸� �߰��� �� ����
void UTFGameInstance::AddActorData(const FGuid& ActorID, FSaveActorData ActorData)
{
	SaveableActorData.Add(ActorID, ActorData);
}

// Ư�� ���� ���� ������ ��ȯ
FSaveActorData UTFGameInstance::GetActorData(const FGuid& ActorID)
{
	return SaveableActorData[ActorID];
}

// ���߿� ���� ���� �Լ� (���� ������ ���� ���� �� ����)
void UTFGameInstance::DEV_SaveGame()
{
	if (SaveGameObject == nullptr)
	{
		CreatSaveslot();
	}

	GetherActorData();
	SaveGameObject->SetSaveActorData(SaveableActorData);
	SaveGameObject->SetPlayerData(PlayerData);
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameName, 0);
}

// ���߿� ���� �ҷ����� �Լ�
void UTFGameInstance::DEV_LoadGame()
{
	LoadGame();
}