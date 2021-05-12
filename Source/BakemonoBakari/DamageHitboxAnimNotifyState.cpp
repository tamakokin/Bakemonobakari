// Fill out your copyright notice in the Description page of Project Settings.
//プレイヤー攻撃のhitboxを生成
//更新履歴
//2021/05/10 作成者：陳
//

#include "DamageHitboxAnimNotifyState.h"

void UDamageHitboxAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UDamageHitboxAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

void UDamageHitboxAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration);
}

//bool UDamageHitboxAnimNotifyState::CanBePlaced(UAnimSequenceBase* Animation)const
//{
	//if (Animation)
	//{
		//if (Animation->IsA(UAnimSequence::StaticClass()) || Animation->IsA(UAnimMontage::StaticClass()))
		//{
			//return(true);
		//}
	//}
	//return(false);
//}