// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

/**
* CH4
* - 게임을 만드는 작업 = 레벨을 구성하는 작업 + 게임플레이 설계하는 작업
* - 게임 플레임워크: 게임의 규칙을 관리하는 게임 모드 + 플레이어가 조종하는 액터인 폰
*
* 게임 규칙
* - 무형적인 요소
* - 심판에 해당하는 역할
*
* 언리얼의 게임모드
* - 게임의 틀을 잡아주는 역할
* - 플레이어가 입장할 때마다 플레이어를 점검, 게임에 입장한 플레이어가 조종할 액터를 생성해 전달
* - 플레이어가 조종할 수 있는 액터: 폰
* 
