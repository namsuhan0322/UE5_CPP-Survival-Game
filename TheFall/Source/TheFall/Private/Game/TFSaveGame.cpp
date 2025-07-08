// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TFSaveGame.h"
#include "Interface/SaveActorInterface.h"

/// 저장할 액터 데이터들을 설정
void UTFSaveGame::SetSaveActorData(TMap<FGuid, FSaveActorData> Data)
{
	SaveableActorData = Data;
}

/// 저장된 액터 데이터들을 반환
TMap<FGuid, FSaveActorData> UTFSaveGame::GetSaveActorData()
{
	return SaveableActorData;
}

/// 현재 로드된 레벨 이름을 저장
void UTFSaveGame::SetCurrentLevel(const FName Level)
{
	CurrentlyLoadedLevel = Level;
}

/// 현재 저장된 레벨 이름을 반환
FName UTFSaveGame::GetCurrentLevel()
{
	return CurrentlyLoadedLevel;
}

/// 플레이어 관련 데이터를 저장
void UTFSaveGame::SetPlayerData(FSaveActorData Data)
{
	PlayerData = Data;
}

/// 저장된 플레이어 데이터를 반환
FSaveActorData UTFSaveGame::GetPlayerData()
{
	return PlayerData;
}