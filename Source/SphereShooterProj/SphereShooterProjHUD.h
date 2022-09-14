// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SphereShooterProjHUD.generated.h"

UCLASS()
class ASphereShooterProjHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASphereShooterProjHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

