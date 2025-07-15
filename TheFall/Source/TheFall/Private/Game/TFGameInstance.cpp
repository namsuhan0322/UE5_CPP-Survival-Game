// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TFGameInstance.h"
#include "Game/TFSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>
#include "Logger.h"

// 생성자 (초기화 처리 없음)
UTFGameInstance::UTFGameInstance() {}

// 저장 슬롯 생성 (빈 SaveGame 객체 생성)
void UTFGameInstance::CreatSaveslot()
{
	SaveGameObject = Cast<UTFSaveGame>(UGameplayStatics::CreateSaveGameObject(UTFSaveGame::StaticClass()));
}

// 월드 내 모든 저장 가능한 액터들의 데이터 수집
void UTFGameInstance::GetherActorData()
{
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		// 유효하지 않거나 저장 인터페이스를 구현하지 않은 액터는 무시
		if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
		{
			continue;
		}

		ISaveActorInterface* Inter = Cast<ISaveActorInterface>(Actor);
		if (Inter == nullptr)
		{
			continue;
		}

		// 고유 식별자 없으면 무시
		FGuid SAI = Inter->GetActorSaveID_Implementation();
		if (!SAI.IsValid())
		{
			continue;
		}

		// 액터의 저장 데이터 가져오기
		FSaveActorData SAD = Inter->GetSaveData_Implementation();

		// 액터 자체 직렬화 (상태 저장)
		FMemoryWriter MemWriter(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		// 액터의 컴포넌트들도 저장
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

		// 최종적으로 데이터 저장 맵에 추가
		SaveableActorData.Add(SAI, SAD);
	}

	// 플레이어 관련 데이터도 수집
	GetherPlayerData();
}

// 저장된 데이터로부터 게임 로딩
void UTFGameInstance::LoadGame()
{
	// 저장된 슬롯이 없을 경우 경고 로그
	if (!UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
	{
		Logger::GetInstance()->AddMessage("Load game called with Invalid save game", ERRORLEVEL::EL_WARNING);
		return;
	}

	// 기존 데이터 클리어 후 로딩
	SaveableActorData.Empty();
	SaveGameObject = Cast<UTFSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0));
	SaveableActorData = SaveGameObject->GetSaveActorData();
	PlayerData = SaveGameObject->GetPlayerData();

	// 저장된 정보 중 스폰된 액터는 다시 생성
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

	// 월드 내 액터 상태 복원
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

		// 액터 복원
		FMemoryReader MemReader(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		Inter->Execute_UpdateFromSave(Actor);
		// 컴포넌트 복원
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
				* 동일한 클래스의 컴포넌트가 여러 개 있는 경우엔 이 방식은 위험함.
				* 첫 번째 일치하는 컴포넌트가 모든 데이터를 받게 됨.
				* 안전하게 하려면 컴포넌트에도 GUID를 부여해서 정확하게 식별해야 함.
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

	// 플레이어 상태 복원
	SetPlayerData();
}

// 플레이어 캐릭터의 데이터 수집
void UTFGameInstance::GetherPlayerData()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ISaveActorInterface* Inter = Cast<ISaveActorInterface>(PlayerCharacter);
	if (Inter == nullptr)
	{
		// 오류 로그 가능
		return;
	}

	FSaveActorData SAD = Inter->GetSaveData_Implementation();

	// 플레이어 직렬화
	FMemoryWriter MemWriter(SAD.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	PlayerCharacter->Serialize(Ar);

	// 컴포넌트 저장
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

// 저장된 데이터로 플레이어 상태 복원
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
			// 동일 클래스 컴포넌트 복원 시 주의 (위 설명과 동일)
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

// 외부에서 저장할 액터 데이터를 추가할 수 있음
void UTFGameInstance::AddActorData(const FGuid& ActorID, FSaveActorData ActorData)
{
	SaveableActorData.Add(ActorID, ActorData);
}

// 특정 액터 저장 데이터 반환
FSaveActorData UTFGameInstance::GetActorData(const FGuid& ActorID)
{
	return SaveableActorData[ActorID];
}

// 개발용 수동 저장 함수 (슬롯 없으면 새로 생성 후 저장)
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

// 개발용 수동 불러오기 함수
void UTFGameInstance::DEV_LoadGame()
{
	LoadGame();
}