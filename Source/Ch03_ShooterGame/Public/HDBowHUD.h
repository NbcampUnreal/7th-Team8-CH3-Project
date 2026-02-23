// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
// 공식 문서 표준에 따라 UTexture2D 인식을 위해 class 키워드를 추가했습니다.
#include "HDBowHUD.generated.h"

/**
 * */
UCLASS()
class CH03_SHOOTERGAME_API AHDBowHUD : public AHUD
{
    GENERATED_BODY()

protected:

    // 화면 중앙에 그려집니다.
    // 변수 선언 시 class 키워드를 붙여야 헤더 컴파일 오류를 방지할 수 있습니다.
    UPROPERTY(EditDefaultsOnly)
    class UTexture2D* CrosshairTexture;

public:

    // HUD의 기본 드로 콜입니다.
    virtual void DrawHUD() override;

}; 