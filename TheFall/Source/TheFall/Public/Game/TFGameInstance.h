// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveActorInterface.h"
#include "TFGameInstance.generated.h"

/**
 * 게임 전역에서 유지되는 저장 관리용 GameInstance 클래스
 */
UCLASS()
class THEFALL_API UTFGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	// 저장 가능한 액터들의 데이터 (GUID로 관리)
	UPROPERTY()
	TMap<FGuid, FSaveActorData> SaveableActorData;

	// SaveGame 오브젝트 인스턴스
	UPROPERTY()
	class UTFSaveGame* SaveGameObject = nullptr;

	// 저장 슬롯 이름 (기본값: "DEFAULT")
	UPROPERTY()
	FString SaveGameName = TEXT("DEFAULT");

	// 현재 로드된 레벨 이름
	UPROPERTY()
	FName CurrentlyLoadedLevel = "NONE";

	// 플레이어 저장 데이터
	UPROPERTY()
	FSaveActorData PlayerData;

	// 생성자 (내부에서만 사용됨)
	UTFGameInstance();

	// 저장 슬롯 생성
	void CreatSaveslot();
	// 씬에 존재하는 저장 대상 액터들의 데이터를 수집
	void GetherActorData();
	// 저장된 게임 데이터를 불러오기
	void LoadGame();
	// 플레이어 저장 데이터를 수집
	void GetherPlayerData();
	// 저장된 플레이어 데이터를 적용
	void SetPlayerData();

public:

	/// 저장 가능한 액터 데이터를 추가 (GUID 기준)
	UFUNCTION(BlueprintCallable)
	void AddActorData(const FGuid& ActorID, FSaveActorData ActorData);
	/// 특정 액터의 저장 데이터를 반환
	UFUNCTION(BlueprintCallable)
	FSaveActorData GetActorData(const FGuid& ActorID);

	/// 개발용 저장 호출 함수 (디버그용)
	UFUNCTION(BlueprintCallable)
	void DEV_SaveGame();
	/// 개발용 로드 호출 함수 (디버그용)
	UFUNCTION(BlueprintCallable)
	void DEV_LoadGame();
};
