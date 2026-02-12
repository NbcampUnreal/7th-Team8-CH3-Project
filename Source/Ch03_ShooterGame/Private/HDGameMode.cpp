// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameMode.h"

void AHDGameMode::StartPlay()
{
    // 부모 클래스의 StartPlay를 호출하여 기본 엔진 로직이 실행되도록 합니다.
    Super::StartPlay();

    // GEngine이 유효한지 체크합니다.
    check(GEngine != nullptr);

    // 디버그 메시지를 5초간 화면에 노란색으로 표시합니다.
    // -1 '키' 값은 새로운 메시지가 이전 메시지를 덮어쓰지 않고 계속 추가되게 합니다.
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is HDGameMode!"));
}