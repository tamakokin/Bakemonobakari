// Fill out your copyright notice in the Description page of Project Settings.
//プレイヤー攻撃のhitboxを生成
//更新履歴
//2021/05/10 作成者：陳
//
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DamageHitboxAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class BAKEMONOBAKARI_API UDamageHitboxAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

	//virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;

};
