// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SaveActorInterface.h"

// 순수 가상(pure virtual)이 아닌 ISaveActorInterface 함수들의 기본 기능을 구현합니다.

// 액터의 저장용 고유 식별자를 반환
// 기본 구현은 ‘무효한(Invalid)’ GUID를 돌려주어, 
// 실제 저장을 원하는 클래스에서 반드시 오버라이드하도록 유도합니다.
FGuid ISaveActorInterface::GetActorSaveID_Implementation()
{
	FGuid Ret;        // 기본 생성 시에는 유효한 값이 아님
	Ret.Invalidate(); // 명시적으로 무효화
	return Ret;
}

// 외부에서 전달된 GUID를 액터에 설정
// 기본 구현은 아무 동작도 하지 않으므로, 필요 시 파생 클래스에서 오버라이드하세요.
void ISaveActorInterface::SetActorGUID_Implementation(const FGuid& NewGuid)
{
	// 구현 없음 (파생 클래스에서 처리)
}

// 액터 자체의 저장 데이터를 반환
// 기본 구현은 ‘빈’ FSaveActorData 구조체를 반환합니다.
FSaveActorData ISaveActorInterface::GetSaveData_Implementation()
{
	return FSaveActorData();
}

// 액터가 보유한 컴포넌트의 저장 데이터를 반환
// 기본 구현은 ‘빈’ FSaveComponentData 구조체를 반환합니다.
FSaveComponentData ISaveActorInterface::GetComponentSaveData_Implementation()
{
	return FSaveComponentData();
}

// 외부에서 전달된 컴포넌트 저장 데이터를 적용
// 기본 구현은 아무 동작도 하지 않으므로, 필요 시 파생 클래스에서 오버라이드하세요.
void ISaveActorInterface::SetComponentSaveData_Implementation(FSaveComponentData Data)
{
	// 구현 없음 (파생 클래스에서 처리)
}