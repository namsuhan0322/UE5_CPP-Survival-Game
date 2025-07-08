// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/// 문자열을 특정 문자 기준으로 나누어 배열로 반환하는 유틸리티 함수 선언
/// @param Source 나눌 문자열
/// @param split 분할 기준 문자 (기본값: '|')
/// @return 분할된 문자열 조각들의 배열
TArray<FString> ChopString(FString Source, char split = '|');