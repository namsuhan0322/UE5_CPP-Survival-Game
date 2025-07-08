// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Interface/SaveActorInterface.h"
#include "TFSaveGame.generated.h"

/// 게임 저장 데이터를 관리하는 SaveGame 클래스
UCLASS()
class THEFALL_API UTFSaveGame : public USaveGame
{
	GENERATED_BODY()

private:

	// 게임 내 저장 가능한 액터들의 데이터 (GUID 기반으로 관리)
	UPROPERTY()
	TMap<FGuid, FSaveActorData> SaveableActorData;
	// 현재 로드된 레벨 이름 (불러오기 시 사용할 레벨 구분용)
	UPROPERTY()
	FName CurrentlyLoadedLevel = "NONE";
	// 플레이어 관련 저장 데이터
	UPROPERTY()
	FSaveActorData PlayerData;

public:

	/// 저장할 액터 데이터 전체 설정
	void SetSaveActorData(TMap<FGuid, FSaveActorData> Data);
	/// 저장된 액터 데이터 전체 반환
	TMap<FGuid, FSaveActorData> GetSaveActorData();

	/// 현재 레벨 이름 설정
	void SetCurrentLevel(const FName Level);
	/// 저장된 현재 레벨 이름 반환
	FName GetCurrentLevel();

	/// 플레이어 데이터 설정
	void SetPlayerData(FSaveActorData Data);
	/// 저장된 플레이어 데이터 반환
	FSaveActorData GetPlayerData();
};
