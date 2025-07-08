// Fill out your copyright notice in the Description page of Project Settings.


#include "TFUtils.h"

/// 문자열을 특정 문자 기준으로 나누어 배열로 반환하는 유틸리티 함수
/// @param Source 나눌 원본 문자열
/// @param split 기준이 되는 문자 (예: ',', ':', '|')
/// @return 분할된 문자열 조각들의 배열
TArray<FString> ChopString(FString Source, char split)
{
	TArray<FString> Ret;    // 결과를 저장할 배열
	FString LSide, RSide;   // 왼쪽/오른쪽 문자열 임시 저장용

	// split 문자를 기준으로 문자열을 계속 분할
	while (Source.Split(&split, &LSide, &RSide))
	{
		Ret.Add(LSide);    // 왼쪽 문자열을 결과 배열에 추가
		Source = RSide;    // 나머지 오른쪽 문자열로 반복 수행
		LSide.Empty();     // 다음 루프를 위해 초기화
	}

	// 마지막 남은 문자열이 있다면 추가
	if (!RSide.IsEmpty())
	{
		Ret.Add(RSide);
	}

	return Ret;
}